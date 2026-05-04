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

print("\n=== CONTAINS ===")
print("'a' in m:", "a" in m)
print("'x' in m:", "x" in m)
print("'x' not in m:", "x" not in m)

print("\n=== GET ===")
print("get('a'):", m.get("a"))
print("get('missing'):", m.get("missing"))
print("get('missing', -1):", m.get("missing", -1))

m["none_val"] = None
print("get('none_val'):", m.get("none_val"))
print("get('none_val', -1):", m.get("none_val", -1))
del m["none_val"]

print("\n=== PUT ===")
print("put('a', 999):", m.put("a", 999))
print("get('a'):", m.get("a"))
print("put('new', 42):", m.put("new", 42))
print("get('new'):", m.get("new"))
del m["new"]
m["a"] = 100

print("\n=== REMOVE ===")
m["tmp"] = 777
print("remove('tmp'):", m.remove("tmp"))
print("remove('missing'):", m.remove("missing"))
print("'tmp' in m:", "tmp" in m)

print("\n=== POP ===")
m["tmp"] = 555
print("pop('tmp'):", m.pop("tmp"))
print("pop('missing', -1):", m.pop("missing", -1))
try:
    m.pop("missing")
except KeyError as e:
    print("pop('missing'):", "KeyError", e)

print("\n=== KEYERROR ON [] ===")
try:
    _ = m["nope"]
except KeyError as e:
    print("m['nope']:", "KeyError", e)

print("\n=== HASH / EQ EXCEPTIONS ===")
class BadHash:
    def __hash__(self):
        raise RuntimeError("bad hash")
try:
    m[BadHash()] = 1
except RuntimeError as e:
    print("set with bad hash:", "RuntimeError", e)
try:
    BadHash() in m
except RuntimeError as e:
    print("contains with bad hash:", "RuntimeError", e)

print("\nlen before mutation tests:", len(m))

print("\n=== MUTATION DURING ITER (INSERT) ===")
try:
    for k in m:
        print("iter:", k)
        m["new_key"] = 999
except Exception as e:
    print("caught:", type(e).__name__, e)

print("\n=== MUTATION DURING ITER (DELETE) ===")
try:
    for k in m:
        print("iter:", k)
        del m[k]
except Exception as e:
    print("caught:", type(e).__name__, e)

print("\n=== MUTATION DURING ITEMS ITER ===")
try:
    for k, v in m.items():
        print(k, v)
        m["x"] = 1
except Exception as e:
    print("caught:", type(e).__name__, e)

print("\n=== MUTATION VIA put() DURING ITER ===")
try:
    for k in m:
        print("iter:", k)
        m.put("via_put", 1)
except Exception as e:
    print("caught:", type(e).__name__, e)

print("\n=== MUTATION VIA remove() DURING ITER ===")
try:
    for k in list(m.keys()):
        m[k] = 0
    for k in m:
        print("iter:", k)
        m.remove(k)
except Exception as e:
    print("caught:", type(e).__name__, e)

print("\n=== TREEIFY (collision stress) ===")
class Collide:
    __slots__ = ("v",)
    def __init__(self, v): self.v = v
    def __hash__(self): return 42
    def __eq__(self, o): return isinstance(o, Collide) and self.v == o.v
    def __repr__(self): return f"C({self.v})"

big = HashMap()
keys = [Collide(i) for i in range(200)]
for i, k in enumerate(keys):
    big[k] = i
print("len after 200 collisions:", len(big))

print("get C(0):", big.get(keys[0]))
print("get C(199):", big.get(keys[199]))
print("C(50) in big:", keys[50] in big)

for k in keys[:195]:
    del big[k]
print("len after deletions:", len(big))
print("C(199) in big:", keys[199] in big)
print("C(0) in big:", keys[0] in big)

print("\n=== FINAL STATE ===")
for k, v in m.items():
    print(k, v)
print("len:", len(m))