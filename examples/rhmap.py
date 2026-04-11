from juniper import RHMap

print("=== Создание ===")
m = RHMap()
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

print("\n=== Обновление значения ===")
m["age"] = 31
print(f"m['age'] после обновления = {m['age']}")

print("\n=== Итерация по ключам (for) ===")
for k in m:
    print(f"  {k}")

print("\n=== keys / values / items ===")
print("keys:  ", list(m.keys()))
print("values:", list(m.values()))
print("items: ", list(m.items()))

print("\n=== del m[key] ===")
del m["city"]
print(f"После del m['city']: {m}")
print(f"len = {len(m)}")

print("\n=== remove ===")
m.remove("score")
print(f"После remove('score'): {m}")

try:
    m.remove("nonexistent")
except KeyError as e:
    print(f"remove('nonexistent') -> KeyError: {e}")

print("\n=== pop ===")
val = m.pop("age")
print(f"pop('age') = {val}")
val = m.pop("missing", "fallback")
print(f"pop('missing', 'fallback') = {val}")

print(f"\nОсталось: {m}")

print("\n=== Создание из dict ===")
m2 = RHMap({"a": 1, "b": 2, "c": 3})
print(f"RHMap(dict) = {m2}")

print("\n=== Создание из пар ===")
m3 = RHMap([("x", 10), ("y", 20)])
print(f"RHMap(pairs) = {m3}")

print("\n=== Сравнение == / != ===")
a = RHMap({"k": 1, "v": 2})
b = RHMap({"v": 2, "k": 1})
c = RHMap({"k": 1})
print(f"a == b -> {a == b}")
print(f"a != c -> {a != c}")

print("\n=== Нагрузочный тест (100000 вставок) ===")
big = RHMap()
for i in range(100_000):
    big[i] = i * 2
print(f"len = {len(big)}")
print(f"big[42]    = {big[42]}")
print(f"big[99999] = {big[99999]}")
assert 50000 in big
assert 100001 not in big
print("Нагрузочный тест пройден!")

print("\n=== clear ===")
big.clear()
print(f"После clear: len = {len(big)}")

print("\n=== del ===")
m4 = RHMap({"temp": 1})
del m4
print("m4 удалён")

