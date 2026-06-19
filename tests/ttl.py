import time

from juniper.cache_policies import TTLCache
from juniper.data_structures import RHMap, SkipListSet


c = TTLCache(
        3,
        default_ttl=0.5,
        map_impl=RHMap,
        expiry_impl=SkipListSet
)

c.put(1, "a")

time.sleep(0.7)

assert c.get(1) is None
assert 1 not in c



c.put(2, "b")

time.sleep(0.3)

c.put(2, "bb")

time.sleep(0.3)

assert c.get(2) == "bb"


print("TTL OK")
