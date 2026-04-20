"""
Public high-level API for Juniper data structures.
This module re-exports C extension classes.
"""

from ._native import (
    SList as _SList,
    DList as _DList,
    HashMap as _HashMap,
    RHMap as _RHMap,
    RBSet as _RBSet,
    AVLSet as _AVLSet,
    BTreeSet as _BTreeSet,
    BTHashMap as _BTHashMap,
    SwissTable as _SwissTable,
    BSTSet as _BSTSet,
    SkipListSet as _SkipListSet,
)


class SList(_SList):
    """Singly linked list implemented in C."""
    pass

class DList(_DList):
    """Doubly linked list implemented in C."""
    pass

class HashMap(_HashMap):
    """Hash map implemented in C."""
    pass


class RHMap(_RHMap):
    """Robin Hood hash map (open addressing) implemented in C."""
    pass

class RBSet(_RBSet):
    """Sorted set based on a red-black tree, implemented in C."""
    pass

class AVLSet(_AVLSet):
    """Sorted set based on an AVL tree, implemented in C."""
    pass

class BTreeSet(_BTreeSet):
    """Sorted set based on a B-tree, implemented in C."""
    pass

class BTHashMap(_BTHashMap):
    """Hash map with B-tree buckets, implemented in C."""
    pass

class SwissTable(_SwissTable):
    """Swiss Table hash map (open addressing with control bytes), implemented in C."""
    pass

class BSTSet(_BSTSet):
    """Sorted set based on an unbalanced binary search tree, implemented in C."""
    pass
 
 
class SkipListSet(_SkipListSet):
    """Sorted set based on a skip list, implemented in C."""
    pass


__all__ = ["SList", "DList", "HashMap", "RHMap", "RBSet", "AVLSet", "BTreeSet", "BTHashMap", "SwissTable", "BSTSet", "SkipListSet"]
