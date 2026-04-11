from juniper import HashMap

m = HashMap()

m["a"] = 100
m["b"] = 200
m["c"] = 300

print("=== keys ===")
for k in m.keys():
    print(k)

print("=== values ===")
for v in m.values():
    print(v)

print("=== items ===")
for k, v in m.items():
    print(k, v)

print("=== default iter ===")
for k in m:
    print(k)

print("len:", len(m))

del m["b"]

print("after delete:")
for k, v in m.items():
    print(k, v)

print("len:", len(m))
