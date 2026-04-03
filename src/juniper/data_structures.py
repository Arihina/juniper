"""
Public high-level API for Juniper data structures.
This module re-exports C extension classes.
"""

from ._native import SList as _SList, HashMap as _HashMap


class SList(_SList):
    """Singly linked list implemented in C."""
    pass


class HashMap(_HashMap):
    """Hash map implemented in C."""
    pass


__all__ = ["SList", "HashMap"]
