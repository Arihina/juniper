from juniper import BTHashMap, RHMap, HashMap
import time
import random

print("=== Создание ===")
m = BTHashMap()
print(f"Пустая: {m}")
print(f"len = {len(m)}")

print("\n=== put / [] ===")
m["name"] = "Alice"
m["age"] = 30
m["city"] = "Moscow"
m.put("score", 99.5)
print(f"После вставок: {m}")
print(f"len = {len(m)}")

print("\n=== get / [] ===")
print(f"m['name']  = {m['name']}")
print(f"m.get('age') = {m.get('age')}")
print(f"m.get('missing', 'default') = {m.get('missing', 'default')}")

print("\n=== in (contains) ===")
print(f"'name' in m  -> {'name' in m}")
print(f"'foo'  in m  -> {'foo' in m}")

print("\n=== Обновление ===")
m["age"] = 31
print(f"m['age'] = {m['age']}")

print("\n=== Итерация ===")
for k in m:
    print(f"  {k}")

print("\n=== keys / values / items ===")
print("keys:  ", list(m.keys()))
print("values:", list(m.values()))
print("items: ", list(m.items()))

print("\n=== del m[key] ===")
del m["city"]
print(f"После del: {m}")

print("\n=== remove ===")
m.remove("score")
print(f"После remove: {m}")
try:
    m.remove("nope")
except KeyError as e:
    print(f"remove('nope') -> KeyError: {e}")

print("\n=== pop ===")
val = m.pop("age")
print(f"pop('age') = {val}")
val = m.pop("missing", "fallback")
print(f"pop('missing', 'fallback') = {val}")
print(f"Осталось: {m}")

print("\n=== Создание из dict ===")
m2 = BTHashMap({"a": 1, "b": 2, "c": 3})
print(f"BTHashMap(dict) = {m2}")

print("\n=== Создание из пар ===")
m3 = BTHashMap([("x", 10), ("y", 20)])
print(f"BTHashMap(pairs) = {m3}")

print("\n=== == / != ===")
a = BTHashMap({"k": 1, "v": 2})
b = BTHashMap({"v": 2, "k": 1})
c = BTHashMap({"k": 1})
print(f"a == b -> {a == b}")
print(f"a != c -> {a != c}")

print("\n=== Нагрузочный тест (100000 вставок) ===")
big = BTHashMap()
for i in range(100_000):
    big[i] = i * 2
print(f"len = {len(big)}")
print(f"big[42]    = {big[42]}")
print(f"big[99999] = {big[99999]}")
assert 50000 in big
assert 100001 not in big
print("Вставка OK")

print("\n=== Shrink test ===")
for i in range(99_900):
    del big[i]
print(f"После удаления 99900: len = {len(big)}")
for i in range(99_900, 100_000):
    assert i in big
    assert big[i] == i * 2
print("Shrink корректен, оставшиеся 100 элементов на месте")

print("\n=== clear ===")
big.clear()
print(f"После clear: len = {len(big)}")

print("\n=== Бенчмарк: вставка 100к ===")
data = list(range(100_000))
random.shuffle(data)

for name, cls in [("BTHashMap", BTHashMap), ("RHMap", RHMap), ("HashMap", HashMap)]:
    t0 = time.perf_counter()
    m = cls() if name != "HashMap" else cls(131072)
    for i in data:
        if hasattr(m, '__setitem__'):
            m[i] = i
        else:
            m.put(i, i)
    t1 = time.perf_counter()
    print(f"  {name:12s}: {(t1-t0)*1000:.1f} ms, len={len(m) if hasattr(m, '__len__') else '?'}")

print("\n=== Бенчмарк: 100к поисков ===")
lookup = [random.randint(0, 99_999) for _ in range(100_000)]

for name, cls in [("BTHashMap", BTHashMap), ("RHMap", RHMap)]:
    m = cls()
    for i in range(100_000):
        m[i] = i
    t0 = time.perf_counter()
    for k in lookup:
        m[k]
    t1 = time.perf_counter()
    print(f"  {name:12s}: {(t1-t0)*1000:.1f} ms")

print("\n=== del ===")
tmp = BTHashMap({"x": 1})
del tmp
print("tmp удалён")
