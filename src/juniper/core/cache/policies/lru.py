"""
LRU cache policy.

Least Recently Used: при переполнении выселяется ключ, к которому дольше
всех не обращались. Каноническая стратегия для большинства кэшей с
выраженной локальностью доступа.

Когда применяется:
    - паттерны доступа с временной локальностью (недавно использованное
      с большой вероятностью понадобится снова);
    - кэш HTTP-ответов, кэш страниц БД, кэш скомпилированных запросов;
    - дефолтный выбор, если про паттерн нагрузки ничего не известно.

Семантика:
    - put нового ключа: вставка в front; если переполнение — выселяется back.
    - put существующего ключа: обновить value и переместить в front (touch).
    - get (hit): переместить в front (touch).
    - evict: снять ключ из back (самый давно использованный).

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


class LRUCache(CachePolicy):
    """
    Least-Recently-Used cache.

    Внутреннее устройство:
        _nodes:  key -> DListNode (handle на узел в _order)
        _values: key -> value
        _order:  DList ключей. Front = самый недавно использованный.
                 Back = самый давно использованный (кандидат на вытеснение).
                 В value узла хранится сам key — это нужно для evict, чтобы
                 узнать, какой ключ удалять из map.
        _size:   ручной счётчик (нет O(1) len() в juniper map).

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

    # ---------- основной API ----------

    def get(self, key: Any) -> Optional[Any]:
        if not self._nodes.contains(key):
            self._record_miss()
            return None
        self._record_hit()
        # Touch: ключ становится самым недавно использованным.
        node = self._nodes.get(key)
        self._order.move_to_front_node(node)
        return self._values.get(key)

    def put(self, key: Any, value: Any) -> None:
        if self._nodes.contains(key):
            # Обновление существующего: обновить value и сделать touch.
            self._values.put(key, value)
            node = self._nodes.get(key)
            self._order.move_to_front_node(node)
            return

        # Новый ключ. Вытесняем ДО вставки — единообразно с MRU и
        # без промежуточного состояния size == capacity + 1.
        # В отличие от MRU, в LRU порядок не критичен по корректности
        # (вставка во front, вытеснение из back — противоположные концы).
        if self._size >= self.capacity:
            self.evict()

        node = self._order.push_front_node(key)
        self._nodes.put(key, node)
        self._values.put(key, value)
        self._size += 1

    def evict(self) -> Optional[Any]:
        if self._order.is_empty():
            return None
        # Снимаем back — самый давно использованный ключ.
        node = self._order.pop_back_node()
        evicted_key = node.value
        self._nodes.remove(evicted_key)
        self._values.remove(evicted_key)
        self._size -= 1
        return evicted_key

    # ---------- вспомогательные ----------

    def __contains__(self, key: Any) -> bool:
        return self._nodes.contains(key)

    def __len__(self) -> int:
        return self._size

    def clear(self) -> None:
        self._nodes.clear()
        self._values.clear()
        self._order.clear()
        self._size = 0