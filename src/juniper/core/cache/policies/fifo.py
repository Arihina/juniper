"""
FIFO cache policy.

Самая простая стратегия вытеснения: первым пришёл — первым ушёл.
В отличие от LRU, обращение к ключу через get() НЕ продлевает ему жизнь.
Перезапись существующего ключа через put() тоже не двигает его в очереди —
меняется только value.

Используется, когда:
    - локальность доступа неважна или отсутствует;
    - нужна предсказуемая стоимость операций без работы с DList handles
      на горячем пути get();
    - простая модель «buffer of recent insertions».
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


class FIFOCache(CachePolicy):
    """
    First-In-First-Out cache.

    Сложность:
        get  — O(1)
        put  — O(1) амортизированно
        evict — O(1)

    Внутреннее устройство:
        _nodes:  key -> DListNode
                 Хранит handle на узел в _order, чтобы при evict узнать,
                 какой ключ удалять; при FIFO handle напрямую не используется
                 на горячем пути, но он нужен для будущих расширений
                 (например, явный delete(key) — снять узел через unlink_node).
        _values: key -> value
                 Отдельная map для значений. Раздельные _nodes и _values
                 чуть дороже по памяти, но дешевле по CPU: на каждый get не
                 распаковываем кортеж (value, node).
        _order:  DList ключей. head = самый старый, tail = самый свежий.
                 В value узла лежит сам key — это нужно для evict.
        _size:   Ручной счётчик. Используем его, потому что у map в juniper
                 нет O(1) len(); если len() появится — можно убрать.

    Args:
        capacity: Максимальное число записей. Должно быть > 0.
        map_impl: Класс hashmap из juniper.data_structures.
                  Один из: SwissTable, RHMap, BTHashMap.
                  По умолчанию SwissTable.
    """

    def __init__(self, capacity: int, map_impl: Type = SwissTable):
        super().__init__(capacity)
        _check_map_impl(map_impl)
        self._map_impl = map_impl

        self._nodes = map_impl()
        self._values = map_impl()
        self._order = DList()
        self._size = 0


    def get(self, key: Any) -> Optional[Any]:
        if not self._nodes.contains(key):
            self._record_miss()
            return None
        self._record_hit()
        return self._values.get(key)

    def put(self, key: Any, value: Any) -> None:
        if self._nodes.contains(key):
            self._values.put(key, value)
            return

        if self._size >= self.capacity:
            self.evict()

        node = self._order.push_back_node(key)
        self._nodes.put(key, node)
        self._values.put(key, value)
        self._size += 1

    def evict(self) -> Optional[Any]:
        if self._order.is_empty():
            return None
        node = self._order.pop_front_node()
        evicted_key = node.value
        self._nodes.remove(evicted_key)
        self._values.remove(evicted_key)
        self._size -= 1
        return evicted_key

    def __contains__(self, key: Any) -> bool:
        return self._nodes.contains(key)

    def __len__(self) -> int:
        return self._size

    def clear(self) -> None:
        self._nodes.clear()
        self._values.clear()
        self._order.clear()
        self._size = 0