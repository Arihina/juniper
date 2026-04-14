from juniper import HashMap
import time
import random
import string

def rand_key(n=10):
    return ''.join(random.choices(string.ascii_letters, k=n))

N = 100_000

m = HashMap()

print("=== INSERT PHASE ===")
start = time.time()

keys = [rand_key() for _ in range(N)]

for i, k in enumerate(keys):
    m[k] = i

end = time.time()
print(f"Inserted {N} items in {end - start:.3f}s")
print("len:", len(m))


print("\n=== READ PHASE ===")
start = time.time()

for k in keys:
    if m[k] is None:
        raise RuntimeError("Key missing!")

end = time.time()
print(f"Read {N} items in {end - start:.3f}s")


print("\n=== ITERATION TEST ===")
start = time.time()

count = 0
for k, v in m.items():
    count += 1

end = time.time()
print(f"Iterated {count} items in {end - start:.3f}s")


print("\n=== DELETE HALF (trigger shrink later) ===")
start = time.time()

for k in keys[:N // 2]:
    del m[k]

end = time.time()
print(f"Deleted {N//2} items in {end - start:.3f}s")
print("len:", len(m))


print("\n=== VALIDATION AFTER DELETE ===")
for k in keys[N // 2:]:
    if m[k] is None:
        raise RuntimeError("Corruption after delete!")


print("\n=== DELETE ALL (force shrink hard) ===")
start = time.time()

for k in keys[N // 2:]:
    del m[k]

end = time.time()
print(f"Deleted remaining items in {end - start:.3f}s")
print("len:", len(m))


print("\n=== FINAL CHECK ===")
if len(m) != 0:
    raise RuntimeError("Map not empty after full delete")
