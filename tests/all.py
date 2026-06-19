from juniper.cache_policies import *
from juniper.data_structures import RHMap, SkipListSet

def smoke(policy):

    policy.put(1, "a")
    policy.put(2, "b")

    assert len(policy) == 2

    assert policy.get(1) == "a"

    policy.put(3, "c")

    assert len(policy) <= policy.capacity

    policy.evict()

    assert len(policy) <= policy.capacity



smoke(FIFOCache(2, RHMap))
smoke(LRUCache(2, RHMap))
smoke(MRUCache(2, RHMap))
smoke(LFUCache(2, RHMap))
smoke(TTLCache(2, default_ttl=0.5, map_impl=RHMap, expiry_impl=SkipListSet))
smoke(ARCCache(2, RHMap))

print("Smoke tests OK")
