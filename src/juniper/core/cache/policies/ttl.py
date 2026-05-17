"""
TTL cache policy.

Time-To-Live: каждая запись имеет срок годности. По истечении срока запись
становится «протухшей» и при следующем обращении удаляется (lazy expiration).

Это самостоятельная политика, а не обёртка: TTL — единственный критерий
выселения. При переполнении capacity выселяется запись с НАИБЛИЖАЙШИМ
expiry (ближе всего к моменту истечения), что естественно сочетается
с TTL-семантикой.

Когда применяется:
    - кэширование данных, у которых есть фактический срок актуальности
      (DNS-ответы, сессии, API rate-limit окна);
    - кэши, где важно строгое отсутствие устаревших данных;
    - rate limiting, где expiry задаёт длину окна.

Семантика:
    - Время измеряется через time.monotonic() — не зависит от системных часов.
    - put нового ключа: устанавливает expiry = now + ttl.
    - put существующего ключа: обновляет value И expiry (рестарт TTL).
    - get (hit для непротухшего): возвращает value, expiry НЕ обновляется.
    - get для протухшего: lazy-удаление, возврат None, miss.
    - evict: при переполнении выселяет ключ с min expiry (включая
      возможные протухшие — они уйдут первыми при purge).
    - __contains__: возвращает False для протухших, БЕЗ побочных эффектов.

Сложность:
    get / put / evict — O(log n) из-за работы с expiry-set.
    purge — амортизированно O(log n) (каждый ключ покидает структуру 1 раз).

Требования к ключам:
    Ключи должны поддерживать __lt__ (для tiebreaker'а в expiry-set,
    где элементы — кортежи (expiry, key)). Числа, строки, кортежи из
    таковых — подходят. Произвольные объекты без __lt__ — нет.

Args:
    capacity: Максимальное число записей. Должно быть > 0.
    default_ttl: TTL по умолчанию в секундах (float). Используется,
                 когда put вызывается без явного ttl. Должен быть > 0.
    map_impl: Класс hashmap. Один из SwissTable, RHMap, BTHashMap.
    expiry_impl: Класс sorted-set. Один из SkipListSet, RBSet, AVLSet,
                 BTreeSet. Все четыре дают O(log n) на наших операциях.
"""

import time
from typing import Any, Optional, Type

from ..base import CachePolicy
from ....data_structures import (
    SwissTable, RHMap, BTHashMap,
    SkipListSet, RBSet, AVLSet, BTreeSet,
)


_ALLOWED_MAPS = (SwissTable, RHMap, BTHashMap)
_ALLOWED_EXPIRY_SETS = (SkipListSet, RBSet, AVLSet, BTreeSet)


def _check_map_impl(map_impl: Type) -> None:
    if map_impl not in _ALLOWED_MAPS:
        names = ", ".join(m.__name__ for m in _ALLOWED_MAPS)
        raise TypeError(
            f"map_impl must be one of: {names}; got {map_impl!r}"
        )


def _check_expiry_impl(expiry_impl: Type) -> None:
    if expiry_impl not in _ALLOWED_EXPIRY_SETS:
        names = ", ".join(s.__name__ for s in _ALLOWED_EXPIRY_SETS)
        raise TypeError(
            f"expiry_impl must be one of: {names}; got {expiry_impl!r}"
        )


class TTLCache(CachePolicy):
    """
    TTL cache with lazy expiration.

    Внутреннее устройство:
        _entries:   key -> (value, expiry: float)
                    Хранение expiry прямо в записи нужно, чтобы при
                    перезаписи / явном удалении ключа точно знать,
                    какую пару (expiry, key) удалять из _expiry_set.
        _expiry_set: упорядоченный set из (expiry, key).
                     Сортировка лексикографическая: сначала по expiry,
                     при равенстве — по key (отсюда требование __lt__
                     к ключам).
        _size:      ручной счётчик.

    Args:
        capacity: Максимальное число записей. Должно быть > 0.
        default_ttl: TTL по умолчанию в секундах. Должен быть > 0.
        map_impl: SwissTable | RHMap | BTHashMap.
        expiry_impl: SkipListSet | RBSet | AVLSet | BTreeSet.
    """

    def __init__(
        self,
        capacity: int,
        default_ttl: float,
        map_impl: Type = SwissTable,
        expiry_impl: Type = SkipListSet,
    ):
        super().__init__(capacity)
        if default_ttl <= 0:
            raise ValueError("default_ttl must be > 0")
        _check_map_impl(map_impl)
        _check_expiry_impl(expiry_impl)

        self._default_ttl = default_ttl
        self._map_impl = map_impl
        self._expiry_impl = expiry_impl

        self._entries = map_impl()        # key -> (value, expiry)
        self._expiry_set = expiry_impl()  # (expiry, key)
        self._size = 0

        # Стратегия peek_min подбирается один раз: разные expiry-set
        # имеют (или не имеют) peek_min с разной семантикой.
        self._peek_min = self._make_peek_min(self._expiry_set)

    @staticmethod
    def _make_peek_min(expiry_set):
        """
        Вернуть функцию `() -> Optional[tuple]`, которая возвращает
        минимальный элемент expiry_set или None для пустого, без мутации.

        Мы выбираем стратегию один раз, чтобы не делать hasattr на каждом вызове.
        """
        if hasattr(expiry_set, "peek_min"):
            # peek_min есть. Семантика «бросает на пустом» (как у SkipListSet)
            # или «возвращает None» — оборачиваем в try/except, чтобы покрыть оба.
            def peek():
                try:
                    return expiry_set.peek_min()
                except (KeyError, IndexError):
                    return None
            return peek
        else:
            # peek_min нет — fallback через pop + add.
            # Дороже на O(log n) лишних операций, но корректно.
            def peek():
                try:
                    item = expiry_set.pop()
                except (KeyError, IndexError):
                    return None
                expiry_set.add(item)
                return item
            return peek

    # ---------- основной API ----------

    def get(self, key: Any) -> Optional[Any]:
        # Сначала чистим протухшее: возможно, наш ключ как раз среди них.
        self._purge_expired()

        if not self._entries.contains(key):
            self._record_miss()
            return None
        self._record_hit()
        value, _expiry = self._entries.get(key)
        return value

    def put(self, key: Any, value: Any, ttl: Optional[float] = None) -> None:
        if ttl is None:
            ttl = self._default_ttl
        elif ttl <= 0:
            raise ValueError("ttl must be > 0")

        # Чистим протухшее ДО проверки переполнения — может, место освободится.
        self._purge_expired()

        now = time.monotonic()
        new_expiry = now + ttl

        if self._entries.contains(key):
            # Перезапись: убрать старую (expiry, key) из set, поставить новую.
            _old_value, old_expiry = self._entries.get(key)
            self._expiry_set.discard((old_expiry, key))
            self._entries.put(key, (value, new_expiry))
            self._expiry_set.add((new_expiry, key))
            return

        # Новый ключ. Если переполнение (после purge всё ещё) — выселить.
        if self._size >= self.capacity:
            self.evict()

        self._entries.put(key, (value, new_expiry))
        self._expiry_set.add((new_expiry, key))
        self._size += 1

    def evict(self) -> Optional[Any]:
        if self._size == 0:
            return None
        # Снимаем минимальный (expiry, key) — самый близкий к истечению
        # (или уже протухший, что тоже корректно).
        expiry_key = self._expiry_set.pop()
        _expiry, victim_key = expiry_key
        self._entries.remove(victim_key)
        self._size -= 1
        return victim_key

    # ---------- TTL-специфичное ----------

    def _purge_expired(self) -> None:
        """
        Удалить все записи, у которых expiry <= now.

        Амортизированная сложность O(log n) на одну удалённую запись.
        Вызывается на каждой публичной операции (get/put/evict/__contains__/__len__),
        чтобы поддерживать инвариант «в _entries нет протухших».
        """
        now = time.monotonic()
        # Цикл: пока минимум в expiry-set протух, снимаем его.
        # _peek_min инкапсулирует разницу между разными expiry-set'ами:
        # для тех, у кого есть peek_min — прямое чтение; для остальных —
        # fallback через pop+add.
        while True:
            head = self._peek_min()
            if head is None:
                return
            head_expiry, head_key = head
            if head_expiry > now:
                # Минимум не протух — значит, ничего больше не протухло.
                return
            # Протух — снимаем.
            self._expiry_set.pop()
            # _entries может уже не содержать ключ, если кто-то его удалил
            # отдельно (но в текущем API такого пути нет; на всякий случай).
            if self._entries.contains(head_key):
                self._entries.remove(head_key)
                self._size -= 1

    def _is_expired(self, expiry: float) -> bool:
        return expiry <= time.monotonic()

    # ---------- вспомогательные ----------

    def __contains__(self, key: Any) -> bool:
        # БЕЗ побочных эффектов: не вызываем _purge_expired.
        # Если ключ есть, но протух — отвечаем False, оставляя удаление
        # ленивой логике ближайшего get/put.
        if not self._entries.contains(key):
            return False
        _value, expiry = self._entries.get(key)
        return not self._is_expired(expiry)

    def __len__(self) -> int:
        # Чтобы len был «правдивым», чистим протухшее перед ответом.
        self._purge_expired()
        return self._size

    def clear(self) -> None:
        self._entries.clear()
        self._expiry_set.clear()
        self._size = 0
