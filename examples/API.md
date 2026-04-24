# Juniper Data Structures API


## SList

| Method | Signature | Description |
|---|---|---|
| `clear` | `()` | Remove all elements. |
| `index` | `()` | Return index of first occurrence; raise ValueError if missing. |
| `insert` | `()` | Insert an element at the given index. |
| `pop_back` | `()` | Remove and return the last element. |
| `pop_front` | `()` | Remove and return the first element. |
| `push_back` | `()` | Add an element to the back. |
| `push_front` | `()` | Add an element to the front. |
| `remove` | `()` | Remove first occurrence of value; raise ValueError if missing. |
| `reverse` | `()` | Reverse the list in-place. |

## DList

| Method | Signature | Description |
|---|---|---|
| `clear` | `()` | Remove all elements. |
| `index` | `()` | Return index of first occurrence; raise ValueError if missing. |
| `insert` | `()` | Insert an element at the given index. |
| `pop_back` | `()` | Remove and return the last element. |
| `pop_front` | `()` | Remove and return the first element. |
| `push_back` | `()` | Add an element to the back. |
| `push_front` | `()` | Add an element to the front. |
| `remove` | `()` | Remove first occurrence; raise ValueError if missing. |
| `reverse` | `()` | Reverse the list in-place. |

## HashMap

| Method | Signature | Description |
|---|---|---|
| `items` | `()` | Return items iterator |
| `keys` | `()` | Return keys iterator |
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
| `pop` | `()` | Remove and return the smallest element. |
| `remove` | `()` | Remove an element; raise KeyError if missing. |
