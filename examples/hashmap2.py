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


print("\n=== MUTATION DURING ITER (INSERT) ===")
try:
    for k in m:
        print("iter:", k)
        m["new_key"] = 999 
except Exception as e:
    print("caught:", type(e), e)


print("\n=== MUTATION DURING ITER (DELETE) ===")
try:
    for k in m:
        print("iter:", k)
        del m[k]
except Exception as e:
    print("caught:", type(e), e)


print("\n=== MUTATION DURING ITEMS ITER ===")
try:
    for k, v in m.items():
        print(k, v)
        m["x"] = 1
except Exception as e:
    print("caught:", type(e), e)


print("\n=== FINAL STATE ===")
for k, v in m.items():
    print(k, v)

print("len:", len(m))
