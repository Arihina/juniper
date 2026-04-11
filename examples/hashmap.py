from juniper import HashMap

m = HashMap()

for i in range(100000):
    m[str(i)] = i

for i in range(50000):
    del m[str(i)]

for k, v in m.items():
    pass

print(len(m))
