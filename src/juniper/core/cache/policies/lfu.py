# juniper/core/cache/policies/lfu.py
"""
LFU cache policy.

Least Frequently Used: при переполнении выселяется ключ с наименьшей
частотой обращений. При равенстве частот используется LRU-правило
(выселяется самый давно использованный среди равночастотных).

Реализация O(1) на всех операциях по схеме Shim/Lee/Park (2010):
    - бакеты «freq -> DList ключей одной частоты»;
    - указатель _min_freq на наименьшую непустую частоту;
    - O(1) переход ключа между бакетами через DListNode handles.

Когда применяется:
    - паттерны доступа со стабильно «горячим» подмножеством, где
      частота — лучший индикатор ценности, чем недавность;
    - кэши, где есть «вечные хиты» (популярные товары/страницы),
      которые LRU может выселить из-за временного простоя.

Семантика:
    - put нового ключа: вставка с freq=1; если переполнение — выселить
      из бакета _min_freq (back = самый давний LRU-кандидат).
    - put существующего ключа: обновить value и повысить частоту.
    - get (hit): повысить частоту.
    - evict: снять back из бакета _min_freq.

Сложность:
    get / put / evict — O(1).
"""

from typing import Any, Optional, Type

from ..base import CachePolicy
from ....data_structures import DList, SwissTable, RHMap, BTHashMap


_ALLOWED_MAPS = (SwissTable, RHMap, BTHashMap)


def _check_map_impl(map_impl: Type) -> None:
    if map_impl not in _ALLOWED_MAPS:
        names = ", ".join(m.__name__ for m in _ALLOWED_MAPS)
        raise TypeError(
            f"map_impl must be one of: {names}; got {map_impl!r}"
        )


class LFUCache(CachePolicy):
    """
    Least-Frequently-Used cache with LRU tiebreaker.

    Внутреннее устройство:
        _values:  key -> value
        _freq:    key -> int (текущая частота)
        _node:    key -> DListNode (handle на узел в бакете _freq[key])
        _buckets: freq -> DList ключей одной частоты.
                  Front = самый недавно попавший в бакет (свежий по LRU).
                  Back  = самый давно попавший (кандидат на вытеснение).
                  В value узла хранится сам key.
        _min_freq: наименьшая непустая частота. Поддерживается только
                   на горячем пути (put/get); после evict при опустошении
                   бакета НЕ пересчитывается, потому что:
                     - либо следующая операция — put, которая сбросит min_freq=1;
                     - либо кэш пуст и min_freq=0 (специальное значение).
        _size:     ручной счётчик размера кэша.

    Args:
        capacity: Максимальное число записей. Должно быть > 0.
        map_impl: Класс hashmap из juniper.data_structures.
                  Один из: SwissTable, RHMap, BTHashMap. По умолчанию SwissTable.
                  Применяется ко всем внутренним map.
    """

    def __init__(self, capacity: int, map_impl: Type = SwissTable):
        super().__init__(capacity)
        _check_map_impl(map_impl)
        self._map_impl = map_impl

        self._values = map_impl()
        self._freq = map_impl()
        self._node = map_impl()
        self._buckets = map_impl()       # freq -> DList
        self._min_freq = 0
        self._size = 0

    # ---------- основной API ----------

    def get(self, key: Any) -> Optional[Any]:
        if not self._values.contains(key):
            self._record_miss()
            return None
        self._record_hit()
        self._touch(key)
        return self._values.get(key)

    def put(self, key: Any, value: Any) -> None:
        if self._values.contains(key):
            # Обновление: значение + повышение частоты.
            self._values.put(key, value)
            self._touch(key)
            return

        # Новый ключ. Освобождаем место ДО вставки.
        if self._size >= self.capacity:
            self.evict()

        # Вставка в бакет freq=1.
        bucket = self._get_or_create_bucket(1)
        node = bucket.push_front_node(key)

        self._values.put(key, value)
        self._freq.put(key, 1)
        self._node.put(key, node)
        self._min_freq = 1
        self._size += 1

    def evict(self) -> Optional[Any]:
        if self._size == 0:
            return None

        # Берём бакет минимальной частоты и снимаем back (LRU среди равночастотных).
        bucket = self._buckets.get(self._min_freq)
        victim_node = bucket.pop_back_node()
        victim_key = victim_node.value

        # Если бакет опустел — удаляем его. _min_freq НЕ пересчитываем
        # (см. комментарий к классу).
        if bucket.is_empty():
            self._buckets.remove(self._min_freq)

        self._values.remove(victim_key)
        self._freq.remove(victim_key)
        self._node.remove(victim_key)
        self._size -= 1

        if self._size == 0:
            # Кэш пуст — сбросим маркер; следующий put выставит min_freq=1.
            self._min_freq = 0

        return victim_key

    # ---------- внутреннее ----------

    def _touch(self, key: Any) -> None:
        """Повысить частоту ключа на 1, переместив его в бакет freq+1."""
        old_freq = self._freq.get(key)
        new_freq = old_freq + 1

        # Снять узел из старого бакета.
        old_bucket = self._buckets.get(old_freq)
        old_node = self._node.get(key)
        old_bucket.unlink_node(old_node)

        # Если старый бакет опустел — убрать его.
        if old_bucket.is_empty():
            self._buckets.remove(old_freq)
            # Если это был бакет min_freq — минимум сдвигается на new_freq,
            # потому что мы прямо сейчас положим этот ключ в new_freq.
            if old_freq == self._min_freq:
                self._min_freq = new_freq

        # Положить в новый бакет (front — самый свежий по LRU-tiebreaker).
        new_bucket = self._get_or_create_bucket(new_freq)
        new_node = new_bucket.push_front_node(key)

        self._freq.put(key, new_freq)
        self._node.put(key, new_node)

    def _get_or_create_bucket(self, freq: int) -> DList:
        if self._buckets.contains(freq):
            return self._buckets.get(freq)
        bucket = DList()
        self._buckets.put(freq, bucket)
        return bucket

    # ---------- вспомогательные ----------

    def __contains__(self, key: Any) -> bool:
        return self._values.contains(key)

    def __len__(self) -> int:
        return self._size

    def clear(self) -> None:
        self._values.clear()
        self._freq.clear()
        self._node.clear()
        self._buckets.clear()
        self._min_freq = 0
        self._size = 0
