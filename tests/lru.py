from juniper.cache_policies import LRUCache
from juniper.data_structures import RHMap


c = LRUCache(3, map_impl=RHMap)

c.put(1, 1)
c.put(2, 2)
c.put(3, 3)

c.get(1)

c.put(4, 4)

assert 1 in c
assert 2 not in c
assert 3 in c
assert 4 in c


print("LRU OK")
