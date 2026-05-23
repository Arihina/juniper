"""
ARC cache policy.

Adaptive Replacement Cache (Megiddo & Modha, 2003): адаптивный гибрид
recency (LRU) и frequency (LFU). Кэш разделён на четыре LRU-списка:

    T1 — недавно использованные ОДИН раз (recency)
    T2 — недавно использованные несколько раз (frequency)
    B1 — призрачный список ключей, выселенных из T1 (только ключи, без values)
    B2 — призрачный список ключей, выселенных из T2 (только ключи, без values)

Реальные данные кэша = T1 ∪ T2, размер ≤ capacity.
B1 и B2 — это «история промахов», их размер тоже ≤ capacity.
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

        self._size = 0

    # ---------- основной API ----------

    def get(self, key: Any) -> Optional[Any]:
        if not self._location.contains(key):
            self._record_miss()
            return None
        list_id, node = self._location.get(key)
        if list_id == _B1 or list_id == _B2:
            # Призрачный hit — данных нет, для пользователя это miss.
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
                
            if list_id == _B1:
                 # Сигнал «recency полезнее»: увеличиваем p.
                delta = max(1, self._n_b2 // self._n_b1) if self._n_b1 > 0 else 1
                self._p = min(self._capacity_int(), self._p + delta)
                if self._n_t1 + self._n_t2 >= self._capacity_int():
                    self._replace(in_b2=False)
                # Снять ключ из B1.
                self._b1.unlink_node(node)
                self._n_b1 -= 1
            else:  # _B2
                # Сигнал «frequency полезнее»: уменьшаем p.
                delta = max(1, self._n_b1 // self._n_b2) if self._n_b2 > 0 else 1
                self._p = max(0, self._p - delta)
                if self._n_t1 + self._n_t2 >= self._capacity_int():
                    self._replace(in_b2=True)
                self._b2.unlink_node(node)
                self._n_b2 -= 1
                # Hit в призраке (B1 или B2): адаптация p и подтяжка ключа в T2.
            # Подтянутый из призрака ключ кладём в T2 (как «вновь обнаруженный
            # с историей»).
            new_node = self._t2.push_front_node(key)
            self._location.put(key, (_T2, new_node))
            self._values.put(key, value)

            self._n_t2 += 1
            self._size += 1

            return

        # Полный miss: ключ не встречался ни в кэше, ни в истории.
        c = self._capacity_int()
        if self._n_t1 + self._n_b1 == c:
            # Случай «B1 заполнен»: освобождаем место в T1 ∪ B1.
            if self._n_t1 < c:
                # Можем сжать B1.
                self._evict_b1_lru()
                self._replace(in_b2=False)
            else:
                # T1 уже = c, B1 пуст или ужат: выселяем LRU из T1.
                self._evict_t1_lru_full()
        else:
            total = self._n_t1 + self._n_t2 + self._n_b1 + self._n_b2
            if total >= c:
                if total == 2 * c:
                    # B2 переполнен — сжимаем.
                    self._evict_b2_lru()
                self._replace(in_b2=False)

        # Вставляем новый ключ в MRU(T1).
        new_node = self._t1.push_front_node(key)
        self._location.put(key, (_T1, new_node))
        self._values.put(key, value)
        self._n_t1 += 1
        self._size += 1

    def evict(self) -> Optional[Any]:
        """
        Ручное выселение одного ключа из реального кэша (T1 ∪ T2).
        Возвращает ключ или None, если кэш пуст. Призраки B1/B2 не трогаются.
        """
        if self._n_t1 + self._n_t2 == 0:
            return None
        # Используем тот же _replace, что и put-путь, но с флагом, требующим
        # реального выселения (а не подтяжки в призрак): _replace всегда
        # двигает ключ в призрак, что нам как раз и нужно — он уйдёт из T1∪T2.
        # in_b2=False — стандартный выбор: выселяем по правилу p vs |T1|.
        victim = self._replace(in_b2=False, return_victim=True)
        return victim

    # ---------- ARC-механика ----------

    def _capacity_int(self) -> int:
        return self.capacity

    def _replace(self, in_b2: bool, return_victim: bool = False):
        """
        Каноническая операция REPLACE из ARC: переместить один ключ
        из T1 или T2 в соответствующий призрак (B1 или B2),
        освободив место в реальном кэше.

        Выбор источника:
            - если |T1| > 0 И (|T1| > p ИЛИ (in_b2 И |T1| == p)) — взять из T1
            - иначе — взять из T2
        """
        n_t1 = self._n_t1
        if n_t1 > 0 and (n_t1 > self._p or (in_b2 and n_t1 == self._p)):
            # Снимаем LRU из T1, кладём в MRU(B1).
            node = self._t1.pop_back_node()
            victim_key = node.value
            self._n_t1 -= 1
            new_node = self._b1.push_front_node(victim_key)
            self._n_b1 += 1
            self._location.put(victim_key, (_B1, new_node))
            self._values.remove(victim_key)
            self._size -= 1
        else:
            # Снимаем LRU из T2, кладём в MRU(B2).
            node = self._t2.pop_back_node()
            victim_key = node.value
            self._n_t2 -= 1
            new_node = self._b2.push_front_node(victim_key)
            self._n_b2 += 1
            self._location.put(victim_key, (_B2, new_node))
            self._values.remove(victim_key)
            self._size -= 1

        if return_victim:
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
        self._size -= 1

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
        self._n_t1 = 0
        self._n_t2 = 0
        self._n_b1 = 0
        self._n_b2 = 0
        self._size = 0
        self._n_t1 = self._n_t2 = self._n_b1 = self._n_b2 = 0
        