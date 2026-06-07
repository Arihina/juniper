"""
ARC cache policy.

Adaptive Replacement Cache (Megiddo & Modha, 2003): адаптивный гибрид
recency (LRU) и frequency (LFU). Кэш разделён на четыре LRU-списка:

    T1 — недавно использованные ОДИН раз (recency)
    T2 — недавно использованные несколько раз (frequency)
    B1 — призрачный список ключей, выселенных из T1 (только ключи, без values)
    B2 — призрачный список ключей, выселенных из T2 (только ключи, без values)

Реальные данные кэша = T1 ∪ T2, размер ≤ capacity.
B1 и B2 — это «история промахов». Истинные инварианты каталога ARC:
    |T1| + |T2| ≤ c          (резидентное множество ≤ ёмкости)
    |T1| + |B1| ≤ c          (список L1 = recency)
    |T1|+|T2|+|B1|+|B2| ≤ 2c (весь каталог)
То есть отдельные |B1|, |B2| НЕ обязаны быть ≤ c — ограничен лишь весь каталог.
Параметр p ∈ [0, capacity] — целевой размер T1; адаптируется в реальном
времени по сигналам из B1 (увеличить p) и B2 (уменьшить p).

Сложность:
    get / put / evict — O(1).
"""

from typing import Any, Optional, Type

from ..base import CachePolicy
from ....data_structures import DList, SwissTable, RHMap, BTHashMap


_ALLOWED_MAPS = (SwissTable, RHMap, BTHashMap)

# Маркеры списков — храним их в map, чтобы за O(1) узнать, где сейчас ключ.
_T1, _T2, _B1, _B2 = 1, 2, 3, 4


def _check_map_impl(map_impl: Type) -> None:
    if map_impl not in _ALLOWED_MAPS:
        names = ", ".join(m.__name__ for m in _ALLOWED_MAPS)
        raise TypeError(
            f"map_impl must be one of: {names}; got {map_impl!r}"
        )


class ARCCache(CachePolicy):
    """
    Adaptive Replacement Cache.

    Внутреннее устройство:
        _values:    key -> value (только для ключей в T1 ∪ T2)
        _location:  key -> (list_id, DListNode)
                    list_id ∈ {_T1, _T2, _B1, _B2}, node — handle в соответствующем DList.
        _t1, _t2, _b1, _b2: DList ключей. Front = MRU, Back = LRU.
        _p: целевой размер T1 (адаптивный).

    Размер резидентного кэша (|T1|+|T2|) — производная величина: единственный
    источник истины — счётчики регионов _n_t1.._n_b2 (у DList нет O(1) len()).

    Args:
        capacity: Максимальное число записей в реальном кэше (T1 ∪ T2). > 0.
        map_impl: SwissTable | RHMap | BTHashMap.
    """

    def __init__(self, capacity: int, map_impl: Type = SwissTable):
        super().__init__(capacity)
        _check_map_impl(map_impl)
        self._map_impl = map_impl

        self._values = map_impl()
        self._location = map_impl()
        self._t1 = DList()
        self._t2 = DList()
        self._b1 = DList()
        self._b2 = DList()
        self._p = 0
        # Размеры списков ведём явно — нет O(1) len() у DList в juniper.
        self._n_t1 = 0
        self._n_t2 = 0
        self._n_b1 = 0
        self._n_b2 = 0

    # _size — производная величина, а не отдельный счётчик (устраняем дублирование
    # с _n_t1+_n_t2, которое могло разъезжаться). Единственный источник истины —
    # счётчики регионов. setter-заглушка поглощает `self._size = 0` из базового
    # CachePolicy.__init__, не давая ему завести независимое поле.
    @property
    def _size(self) -> int:
        return self._n_t1 + self._n_t2

    @_size.setter
    def _size(self, value: int) -> None:
        pass

    # ---------- основной API ----------

    def get(self, key: Any) -> Optional[Any]:
        # Чистое чтение. Призрачный hit (B1/B2) для пользователя — это miss:
        # данных нет. p здесь НЕ адаптируется намеренно: адаптация p в ARC
        # привязана к подтяжке ключа в T2 (происходит на пути put), а get ничего
        # не вставляет. Адаптировать p без последующего REPLACE/промоушена —
        # значило бы рассинхронизировать целевой размер T1 с реальным каталогом.
        if not self._location.contains(key):
            self._record_miss()
            return None
        list_id, node = self._location.get(key)
        if list_id == _B1 or list_id == _B2:
            self._record_miss()
            return None
        # Реальный hit: T1 или T2. Перемещаем в MRU(T2).
        self._record_hit()
        if list_id == _T1:
            self._t1.unlink_node(node)
            self._n_t1 -= 1
        else:
            self._t2.unlink_node(node)
            self._n_t2 -= 1
        new_node = self._t2.push_front_node(key)
        self._location.put(key, (_T2, new_node))
        self._n_t2 += 1
        return self._values.get(key)

    def put(self, key: Any, value: Any) -> None:
        c = self.capacity

        if self._location.contains(key):
            list_id, node = self._location.get(key)

            if list_id == _T1 or list_id == _T2:
                # Hit в реальном кэше: обновить value и продвинуть в MRU(T2).
                self._values.put(key, value)
                if list_id == _T1:
                    self._t1.unlink_node(node)
                    self._n_t1 -= 1
                else:
                    self._t2.unlink_node(node)
                    self._n_t2 -= 1
                new_node = self._t2.push_front_node(key)
                self._location.put(key, (_T2, new_node))
                self._n_t2 += 1
                return

            # Hit в призраке (B1 или B2): адаптируем p, снимаем ключ из призрака,
            # освобождаем место в реальном кэше через REPLACE и подтягиваем ключ
            # в MRU(T2) как «вновь обнаруженный с историей».
            if list_id == _B1:
                # Сигнал «recency полезнее»: увеличиваем p.
                delta = max(1, self._n_b2 // self._n_b1) if self._n_b1 > 0 else 1
                self._p = min(c, self._p + delta)
                # Снимаем ключ из B1 ДО REPLACE: канонический порядок и нулевой
                # риск пересечения снимаемого узла с выбором жертвы в _replace.
                self._b1.unlink_node(node)
                self._n_b1 -= 1
                if self._n_t1 + self._n_t2 >= c:
                    self._replace(in_b2=False)
            else:  # _B2
                # Сигнал «frequency полезнее»: уменьшаем p.
                delta = max(1, self._n_b1 // self._n_b2) if self._n_b2 > 0 else 1
                self._p = max(0, self._p - delta)
                # Снимаем ключ из B2 ДО REPLACE (in_b2=True!): иначе REPLACE,
                # пополнив B2 новой жертвой, мешается с тем же списком, из
                # которого мы сейчас вынимаем узел.
                self._b2.unlink_node(node)
                self._n_b2 -= 1
                if self._n_t1 + self._n_t2 >= c:
                    self._replace(in_b2=True)

            new_node = self._t2.push_front_node(key)
            self._location.put(key, (_T2, new_node))
            self._values.put(key, value)
            self._n_t2 += 1
            return

        # Полный miss: ключ не встречался ни в кэше, ни в истории.
        if self._n_t1 + self._n_t2 == c:
            # Резидентное множество ПОЛНО — освобождаем слот через REPLACE.
            # Чистый путь ARC (инвариант: при total ≥ c всегда |T1|+|T2| == c).
            if self._n_t1 + self._n_b1 == c:
                # L1 = T1 ∪ B1 упёрся в потолок |T1|+|B1| ≤ c.
                if self._n_t1 < c:
                    # В B1 есть что выселить — сжимаем B1, затем REPLACE.
                    self._evict_b1_lru()
                    self._replace(in_b2=False)
                else:
                    # T1 == c, B1 пуст: реально выселяем LRU из T1 без призрака.
                    self._evict_t1_lru_full()
            else:
                total = self._n_t1 + self._n_t2 + self._n_b1 + self._n_b2
                if total == 2 * c:
                    # Весь каталог ограничен 2c. На потолке перед REPLACE
                    # обязаны сжать историю — выселяем LRU из B2.
                    self._evict_b2_lru()
                self._replace(in_b2=False)
        else:
            # В резидентном множестве есть место — выселять данные не нужно.
            # Это и нормальный «прогрев» (призраки пусты), и состояние после
            # ручного evict() (резидент < c, но история осталась). Достаточно
            # поддержать инварианты каталога при добавлении ключа в T1:
            if self._n_t1 + self._n_b1 >= c and self._n_b1 > 0:
                self._evict_b1_lru()          # держим |T1|+|B1| ≤ c
            if (self._n_t1 + self._n_t2 + self._n_b1 + self._n_b2) >= 2 * c \
                    and self._n_b2 > 0:
                self._evict_b2_lru()          # держим весь каталог ≤ 2c

        # Вставляем новый ключ в MRU(T1).
        new_node = self._t1.push_front_node(key)
        self._location.put(key, (_T1, new_node))
        self._values.put(key, value)
        self._n_t1 += 1

    def evict(self) -> Optional[Any]:
        """
        Ручное выселение одного ключа из реального кэша (T1 ∪ T2): ключ удаляется
        ПОЛНОСТЬЮ (из values и location), призрак НЕ создаётся. Возвращает
        выселенный ключ или None, если реальный кэш пуст.

        В отличие от REPLACE на пути put (демоут реального ключа в призрак ради
        истории промахов), ручной evict — это сжатие резидентного множества:
        создавать призрак здесь было бы ошибкой — он стал бы «фантомом» при
        |T1|+|T2| < c. Уменьшение резидента ниже ёмкости временно ослабляет
        инвариант «при total ≥ c всегда |T1|+|T2| == c»; структура остаётся
        согласованной, а кэш до-наполняется штатно на последующих put.
        """
        if self._n_t1 + self._n_t2 == 0:
            return None
        # Выбор жертвы — то же правило p vs |T1|, что и в REPLACE.
        if self._n_t1 > 0 and self._n_t1 > self._p:
            node = self._t1.pop_back_node()
            self._n_t1 -= 1
        elif self._n_t2 > 0:
            node = self._t2.pop_back_node()
            self._n_t2 -= 1
        else:
            node = self._t1.pop_back_node()
            self._n_t1 -= 1
        victim_key = node.value
        self._location.remove(victim_key)
        self._values.remove(victim_key)
        return victim_key

    # ---------- ARC-механика ----------

    def _replace(self, in_b2: bool) -> Any:
        """
        Каноническая операция REPLACE из ARC: переместить один ключ из T1 или T2
        в соответствующий призрак (B1 или B2), освободив слот реального кэша.
        Возвращает перемещённый ключ (без bool-флага return_victim — путь put
        результат игнорирует, а вызывающему он ничего не стоит).

        Выбор источника:
            - если |T1| > 0 И (|T1| > p ИЛИ (in_b2 И |T1| == p)) — взять из T1;
            - иначе — взять из T2.
        """
        n_t1 = self._n_t1
        take_t1 = n_t1 > 0 and (n_t1 > self._p or (in_b2 and n_t1 == self._p))
        # Защита от пустого источника (достижимо лишь во внеполосных состояниях
        # после ручного evict): если выбранный список пуст — берём из другого.
        if take_t1 and self._n_t1 == 0:
            take_t1 = False
        elif not take_t1 and self._n_t2 == 0:
            take_t1 = self._n_t1 > 0

        if take_t1:
            # Снимаем LRU из T1, кладём в MRU(B1).
            node = self._t1.pop_back_node()
            victim_key = node.value
            self._n_t1 -= 1
            new_node = self._b1.push_front_node(victim_key)
            self._n_b1 += 1
            self._location.put(victim_key, (_B1, new_node))
            self._values.remove(victim_key)
        else:
            # Снимаем LRU из T2, кладём в MRU(B2).
            node = self._t2.pop_back_node()
            victim_key = node.value
            self._n_t2 -= 1
            new_node = self._b2.push_front_node(victim_key)
            self._n_b2 += 1
            self._location.put(victim_key, (_B2, new_node))
            self._values.remove(victim_key)

        return victim_key

    def _evict_b1_lru(self) -> None:
        node = self._b1.pop_back_node()
        ghost_key = node.value
        self._n_b1 -= 1
        self._location.remove(ghost_key)

    def _evict_b2_lru(self) -> None:
        node = self._b2.pop_back_node()
        ghost_key = node.value
        self._n_b2 -= 1
        self._location.remove(ghost_key)

    def _evict_t1_lru_full(self) -> None:
        """
        Особый путь, когда |T1| == c, |B1| == 0: B1 не из чего сжимать,
        и нужно реально выселить LRU из T1, удалив его без призрака.
        """
        node = self._t1.pop_back_node()
        victim_key = node.value
        self._n_t1 -= 1
        self._location.remove(victim_key)
        self._values.remove(victim_key)

    # ---------- вспомогательные ----------

    def __contains__(self, key: Any) -> bool:
        if not self._location.contains(key):
            return False
        list_id, _node = self._location.get(key)
        # Призраки не считаются содержимым кэша.
        return list_id == _T1 or list_id == _T2

    def __len__(self) -> int:
        # Размер «реального» кэша: T1 ∪ T2.
        return self._n_t1 + self._n_t2

    def clear(self) -> None:
        self._values.clear()
        self._location.clear()
        self._t1.clear()
        self._t2.clear()
        self._b1.clear()
        self._b2.clear()
        self._p = 0
        self._n_t1 = self._n_t2 = self._n_b1 = self._n_b2 = 0
        