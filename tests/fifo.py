from juniper.cache_policies import FIFOCache
from juniper.data_structures import RHMap


c = FIFOCache(3, map_impl=RHMap)

c.put(1, "a")
c.put(2, "b")
c.put(3, "c")

assert c.get(1) == "a"

c.put(4, "d")

assert 1 not in c
assert 2 in c
assert 3 in c
assert 4 in c


c.put(2, "bb")
assert c.get(2) == "bb"

c.put(5, "e")

assert 2 not in c
assert 3 in c
assert 4 in c
assert 5 in c


print("FIFO OK")
