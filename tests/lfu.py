from juniper.cache_policies import LFUCache
from juniper.data_structures import RHMap


c = LFUCache(3, map_impl=RHMap)

c.put(1, 1)
c.put(2, 2)
c.put(3, 3)


c.get(1)
c.get(1)

c.get(2)


c.put(4, 4)


assert 1 in c
assert 2 in c
assert 3 not in c
assert 4 in c


print("LFU OK")
