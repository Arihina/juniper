"""
Public high-level API for Juniper cache policies.
This module re-exports cache policy implementations.
"""
from .core.cache.policies import (
    ARCCache as _ARCCache,
    FIFOCache as _FIFOCache,
    LFUCache as _LFUCache,
    LRUCache as _LRUCache,
    MRUCache as _MRUCache,
    TTLCache as _TTLCache,
)


class ARCCache(_ARCCache):
    """Adaptive Replacement Cache policy."""
    pass


class FIFOCache(_FIFOCache):
    """First-In-First-Out cache policy."""
    pass


class LFUCache(_LFUCache):
    """Least-Frequently-Used cache policy with LRU tiebreaker."""
    pass


class LRUCache(_LRUCache):
    """Least-Recently-Used cache policy."""
    pass


class MRUCache(_MRUCache):
    """Most-Recently-Used cache policy."""
    pass


class TTLCache(_TTLCache):
    """Time-To-Live cache policy with lazy expiration."""
    pass


__all__ = ["ARCCache", "FIFOCache", "LFUCache", "LRUCache", "MRUCache", "TTLCache"]
