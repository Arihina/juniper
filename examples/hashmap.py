from junipre import _native

map_capsule = _native.hashmap_create()

_native.hashmap_put(map_capsule, "a", 100)
_native.hashmap_put(map_capsule, "b", 200)

print(_native.hashmap_get(map_capsule, "a"))
print(_native.hashmap_get(map_capsule, "b"))
print(_native.hashmap_get(map_capsule, "c"))

_native.hashmap_remove(map_capsule, "a")
print(_native.hashmap_get(map_capsule, "a"))
