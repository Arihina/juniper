"""
Public high-level API for Juniper data structures.
This module re-exports C extension classes.
"""

from ._native import SList as _SList, HashMap as _HashMap, RBSet as _RBSet


class SList(_SList):
    """Singly linked list implemented in C."""
    pass


class HashMap(_HashMap):
    """Hash map implemented in C."""
    pass


class RBSet(_RBSet):
    """Sorted set based on a red-black tree, implemented in C."""
    pass
 
 
__all__ = ["SList", "HashMap", "RBSet"]

