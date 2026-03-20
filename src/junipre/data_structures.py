from . import _native


# --- SList ---
def slist_create():
    return _native.create()

def slist_push_front(lst, value):
    return _native.push_front(lst, value)

def slist_pop_front(lst):
    return _native.pop_front(lst)


# --- HashMap ---
def hashmap_create():
    return _native.hashmap_create()

def hashmap_put(map_obj, key, value):
    return _native.hashmap_put(map_obj, key, value)

def hashmap_get(map_obj, key):
    return _native.hashmap_get(map_obj, key)

def hashmap_remove(map_obj, key):
    return _native.hashmap_remove(map_obj, key)
