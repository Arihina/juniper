"""
MRU cache policy.

Most Recently Used: при переполнении выселяется НЕДАВНО использованный ключ.
Это противоположность LRU.

Когда применяется:
    - последовательное сканирование больших датасетов: только что прочитанная
      страница в ближайшее время точно не понадобится, а старые — могут;
    - циклические рабочие нагрузки, где недавно тронутый ресурс — наименее
      ценный для удержания;
    - в составе адаптивных стратегий (как один из режимов).

Семантика:
    - put нового ключа: вставка в front; если переполнение — ВЫТЕСНЕНИЕ
      выполняется ДО вставки, чтобы не выселить только что вставленный ключ.
    - put существующего ключа: обновить value и переместить в front (touch).
    - get (hit): переместить в front (touch).
    - evict: снять ключ из front (самый недавно использованный).

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


class MRUCache(CachePolicy):
    """
    Most-Recently-Used cache.

    Внутреннее устройство:
        _nodes:  key -> DListNode (handle на узел в _order)
        _values: key -> value
        _order:  DList ключей. Front = самый недавно использованный (MRU,
                 кандидат на вытеснение). Back = самый давно использованный.
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

        # Новый ключ. КРИТИЧНО: evict ДО вставки.
        # Иначе выселится только что вставленный ключ (он окажется во front)
        # и put превратится в no-op.
        if self._size >= self.capacity:
            self.evict()

        node = self._order.push_front_node(key)
        self._nodes.put(key, node)
        self._values.put(key, value)
        self._size += 1

    def evict(self) -> Optional[Any]:
        if self._order.is_empty():
            return None
        # Снимаем front — самый недавно использованный ключ.
        node = self._order.pop_front_node()
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
