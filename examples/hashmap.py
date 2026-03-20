from junipre import data_structures

map_capsule = data_structures.hashmap_create()

data_structures.hashmap_put(map_capsule, "a", 100)
data_structures.hashmap_put(map_capsule, "b", 200)

print(data_structures.hashmap_get(map_capsule, "a"))
print(data_structures.hashmap_get(map_capsule, "b"))
print(data_structures.hashmap_get(map_capsule, "c"))

data_structures.hashmap_remove(map_capsule, "a")
print(data_structures.hashmap_get(map_capsule, "a"))
