# Juniper Data Structures API


## SList

| Method | Signature | Description |
|---|---|---|
| `clear` | `()` | Remove all elements. |
| `index` | `()` | Return index of first occurrence; raise ValueError if missing. |
| `insert` | `()` | Insert an element at the given index. |
| `is_empty` | `()` | Return True if list is empty. |
| `pop_back` | `()` | Remove and return the last element. |
| `pop_front` | `()` | Remove and return the first element. |
| `push_back` | `()` | Add an element to the back. |
| `push_front` | `()` | Add an element to the front. |
| `remove` | `()` | Remove first occurrence of value; raise ValueError if missing. |
| `reverse` | `()` | Reverse the list in-place. |

## DList

| Method | Signature | Description |
|---|---|---|
| `back_node` | `()` | Return handle to tail node, or None if empty. |
| `clear` | `()` | Remove all elements. |
| `front_node` | `()` | Return handle to head node, or None if empty. |
| `index` | `()` | Return index of first occurrence; raise ValueError if missing. |
| `insert` | `()` | Insert an element at the given index. |
| `is_empty` | `()` | Return True if list is empty. |
| `move_to_back_node` | `()` | O(1) move node handle to back. |
| `move_to_front` | `()` | Move element at given index to the front. |
| `move_to_front_node` | `()` | O(1) move node handle to front. |
| `pop_back` | `()` | Remove and return the last element. |
| `pop_back_node` | `()` | Unlink tail node and return its (detached) handle. |
| `pop_front` | `()` | Remove and return the first element. |
| `pop_front_node` | `()` | Unlink head node and return its (detached) handle. |
| `push_back` | `()` | Add an element to the back. |
| `push_back_node` | `()` | Insert value at back, return DListNode handle. |
| `push_front` | `()` | Add an element to the front. |
| `push_front_node` | `()` | Insert value at front, return DListNode handle. |
| `remove` | `()` | Remove first occurrence; raise ValueError if missing. |
| `reverse` | `()` | Reverse the list in-place. |
| `unlink_node` | `()` | Unlink node by handle, return its value. Handle becomes detached. |

## HashMap

| Method | Signature | Description |
|---|---|---|
| `get` | `()` | get(key, default=None) |
| `items` | `()` | Return items iterator |
| `keys` | `()` | Return keys iterator |
| `pop` | `()` | pop(key[, default]) |
| `put` | `()` | put(key, value) -> old value or None |
| `remove` | `()` | remove(key) -> old value or None |
| `values` | `()` | Return values iterator |

## RHMap

| Method | Signature | Description |
|---|---|---|
| `clear` | `()` | Remove all entries. |
| `contains` | `()` | Return True if key exists. |
| `get` | `()` | Get value by key with optional default. |
| `items` | `()` | Return an iterator over (key, value) pairs. |
| `keys` | `()` | Return an iterator over keys. |
| `pop` | `()` | Remove key and return value; return default if given, else KeyError. |
| `put` | `()` | Insert or update key/value pair. |
| `remove` | `()` | Remove key; raise KeyError if missing. |
| `values` | `()` | Return an iterator over values. |

## RBSet

| Method | Signature | Description |
|---|---|---|
| `add` | `()` | Add an element to the set. |
| `clear` | `()` | Remove all elements. |
| `contains` | `()` | Return True if element is in the set. |
| `discard` | `()` | Remove an element if present (no error if missing). |
| `peek_min` | `()` | Return the smallest element without removing it. |
| `pop` | `()` | Remove and return the smallest element. |
| `remove` | `()` | Remove an element; raise KeyError if missing. |

## AVLSet

| Method | Signature | Description |
|---|---|---|
| `add` | `()` | Add an element to the set. |
| `clear` | `()` | Remove all elements. |
| `contains` | `()` | Return True if element is in the set. |
| `discard` | `()` | Remove an element if present (no error if missing). |
| `height` | `()` | Return the height of the AVL tree. |
| `peek_min` | `()` | Return the smallest element without removing it. |
| `pop` | `()` | Remove and return the smallest element. |
| `remove` | `()` | Remove an element; raise KeyError if missing. |

## BTreeSet

| Method | Signature | Description |
|---|---|---|
| `add` | `()` | Add an element to the set. |
| `clear` | `()` | Remove all elements. |
| `contains` | `()` | Return True if element is in the set. |
| `discard` | `()` | Remove an element if present (no error if missing). |
| `height` | `()` | Return the height of the B-tree. |
| `order` | `()` | Return the order (branching factor) of the B-tree. |
| `peek_min` | `()` | Return the smallest element without removing it. |
| `pop` | `()` | Remove and return the smallest element. |
| `remove` | `()` | Remove an element; raise KeyError if missing. |

## BTHashMap

| Method | Signature | Description |
|---|---|---|
| `clear` | `()` | Remove all entries. |
| `contains` | `()` | Return True if key exists. |
| `get` | `()` | Get value by key with optional default. |
| `items` | `()` | Return an iterator over (key, value) pairs. |
| `keys` | `()` | Return an iterator over keys. |
| `pop` | `()` | Remove key and return value; return default if given, else KeyError. |
| `put` | `()` | Insert or update key/value pair. |
| `remove` | `()` | Remove key; raise KeyError if missing. |
| `values` | `()` | Return an iterator over values. |

## SwissTable

| Method | Signature | Description |
|---|---|---|
| `clear` | `()` | Remove all entries. |
| `contains` | `()` | Return True if key exists. |
| `get` | `()` | Get value by key with optional default. |
| `items` | `()` | Return an iterator over (key, value) pairs. |
| `keys` | `()` | Return an iterator over keys. |
| `pop` | `()` | Remove key and return value; return default if given, else KeyError. |
| `put` | `()` | Insert or update key/value pair. |
| `remove` | `()` | Remove key; raise KeyError if missing. |
| `values` | `()` | Return an iterator over values. |

## BSTSet

| Method | Signature | Description |
|---|---|---|
| `add` | `()` | Add an element to the set. |
| `clear` | `()` | Remove all elements. |
| `contains` | `()` | Return True if element is in the set. |
| `discard` | `()` | Remove an element if present (no error if missing). |
| `height` | `()` | Return the height of the tree. |
| `peek_min` | `()` | Return the smallest element without removing it. |
| `pop` | `()` | Remove and return the smallest element. |
| `remove` | `()` | Remove an element; raise KeyError if missing. |

## SkipListSet

| Method | Signature | Description |
|---|---|---|
| `add` | `()` | Add an element to the set. |
| `clear` | `()` | Remove all elements. |
| `contains` | `()` | Return True if element is in the set. |
| `discard` | `()` | Remove an element if present (no error if missing). |
| `height` | `()` | Return the current max level of the skip list. |
| `is_empty` | `()` | Return True if set is empty. |
| `peek_min` | `()` | Return the smallest element without removing it. |
| `pop` | `()` | Remove and return the smallest element. |
| `remove` | `()` | Remove an element; raise KeyError if missing. |