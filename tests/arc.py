from juniper.cache_policies import ARCCache
from juniper.data_structures import RHMap


c = ARCCache(2, map_impl=RHMap)

c.put(1, "a")
c.put(2, "b")

c.get(1)


c.put(3, "c")


assert len(c) == 2


assert c.get(2) is None


c.put(2, "bb")


assert 2 in c
assert c.get(2) == "bb"


print("ARC OK")
