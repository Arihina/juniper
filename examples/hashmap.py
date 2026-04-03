from juniper import HashMap

m = HashMap()

m["a"] = 100
m["b"] = 200

print(m["a"])
print(m["b"])
print(m["c"])

del m["a"]

print(m["a"])
