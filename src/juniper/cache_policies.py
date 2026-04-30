# from juniper.core.cache import LRUCache, LFUCache, TTLCache, FIFOCache

# def make_cache(strategy: str = "lru", capacity: int = 128, **kwargs):
#     strategies = {
#         "lru":  LRUCache,
#         "lfu":  LFUCache,
#         "ttl":  TTLCache,
#         "fifo": FIFOCache,
#     }
#     if strategy not in strategies:
#         raise ValueError(f"Unknown strategy: {strategy!r}. Choose from {list(strategies)}")
#     return strategies[strategy](capacity, **kwargs)