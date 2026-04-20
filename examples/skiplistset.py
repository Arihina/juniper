from juniper import SkipListSet, RBSet, AVLSet, BTreeSet
import random
import time

print("=== Создание ===")
s = SkipListSet()
print(f"Пустой set: {s}")
print(f"len = {len(s)}")

print("\n=== add ===")
for x in [5, 3, 8, 1, 4, 7, 9, 2, 6, 10]:
    s.add(x)
print(f"После добавления 1..10: {s}")
print(f"len = {len(s)}, height (уровни) = {s.height()}")

print("\n=== Дубликаты ===")
s.add(5)
s.add(3)
print(f"После повторного add(5), add(3): len = {len(s)}")

print("\n=== in (contains) ===")
print(f"5 in s  -> {5 in s}")
print(f"42 in s -> {42 in s}")

print("\n=== Итерация (in-order) ===")
print("Элементы:", list(s))

print("\n=== for-loop ===")
for val in s:
    print(val, end=" ")
print()

print("\n=== discard ===")
s.discard(100)
print("discard(100) — ок, без ошибки")

print("\n=== remove ===")
s.remove(5)
print(f"После remove(5): {s}")
print(f"len = {len(s)}")

try:
    s.remove(5)
except KeyError as e:
    print(f"remove(5) повторно -> KeyError: {e}")

print("\n=== pop (минимальный элемент) ===")
while len(s) > 5:
    val = s.pop()
    print(f"pop -> {val}", end="  ")
print(f"\nОсталось: {s}")

print("\n=== Создание из iterable ===")
s2 = SkipListSet([30, 10, 20, 10, 20])
print(f"SkipListSet([30, 10, 20, 10, 20]) = {s2}")
print(f"len = {len(s2)}")

print("\n=== Сравнение == / != ===")
a = SkipListSet([1, 2, 3])
b = SkipListSet([3, 2, 1])
c = SkipListSet([1, 2])
print(f"SkipListSet([1,2,3]) == SkipListSet([3,2,1]) -> {a == b}")
print(f"SkipListSet([1,2,3]) != SkipListSet([1,2])   -> {a != c}")

print("\n=== clear ===")
a.clear()
print(f"После clear: {a}, len = {len(a)}")

print("\n=== Строки ===")
words = SkipListSet(["banana", "apple", "cherry", "apple", "date"])
print(f"Строковый set: {words}")
print(f"Итерация: {list(words)}")

print("\n=== Нагрузочный тест (100000 элементов) ===")
big = SkipListSet(range(100_000))
print(f"len = {len(big)}, height = {big.height()}")
assert 50000 in big
assert 100001 not in big

for i in range(0, 100_000, 2):
    big.discard(i)
print(f"После удаления чётных: len = {len(big)}, height = {big.height()}")
print("Нагрузочный тест пройден!")

print("\n=== Бенчмарк: все sorted-set реализации ===")
N = 100_000
data = random.sample(range(N * 10), N)
lookup = [random.choice(data) for _ in range(N)]

print(f"\n  {'Структура':16s}  {'Вставка':>10s}  {'Поиск':>10s}  {'Высота':>7s}")
print(f"  {'-'*16}  {'-'*10}  {'-'*10}  {'-'*7}")

for name, cls in [("SkipListSet", SkipListSet),
                  ("RBSet", RBSet),
                  ("AVLSet", AVLSet),
                  ("BTreeSet", BTreeSet)]:
    t0 = time.perf_counter()
    s = cls(data)
    t1 = time.perf_counter()
    ins_ms = (t1 - t0) * 1000

    t0 = time.perf_counter()
    for k in lookup:
        k in s
    t1 = time.perf_counter()
    look_ms = (t1 - t0) * 1000

    h = s.height() if hasattr(s, 'height') else "—"
    print(f"  {name:16s}  {ins_ms:8.1f} ms  {look_ms:8.1f} ms  {str(h):>7s}")

print("\n=== del ===")
tmp = SkipListSet([1, 2, 3])
del tmp
print("tmp удалён")
