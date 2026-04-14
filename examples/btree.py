from juniper import BTreeSet, RBSet, AVLSet
import time
import random
import math

print("=== Создание ===")
s = BTreeSet()
print(f"Пустой set: {s}")
print(f"len = {len(s)}, order = {s.order()}, height = {s.height()}")

print("\n=== add ===")
for x in [5, 3, 8, 1, 4, 7, 9, 2, 6, 10]:
    s.add(x)
print(f"После добавления 1..10: {s}")
print(f"len = {len(s)}, height = {s.height()}")

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
s2 = BTreeSet([30, 10, 20, 10, 20])
print(f"BTreeSet([30, 10, 20, 10, 20]) = {s2}")
print(f"len = {len(s2)}")

print("\n=== Задание order ===")
s3 = BTreeSet([1, 2, 3, 4, 5], order=4)
print(f"BTreeSet(order=4): {s3}, order={s3.order()}, height={s3.height()}")

print("\n=== Сравнение == / != ===")
a = BTreeSet([1, 2, 3])
b = BTreeSet([3, 2, 1])
c = BTreeSet([1, 2])
print(f"BTreeSet([1,2,3]) == BTreeSet([3,2,1]) -> {a == b}")
print(f"BTreeSet([1,2,3]) != BTreeSet([1,2])   -> {a != c}")

print("\n=== clear ===")
a.clear()
print(f"После clear: {a}, len = {len(a)}")

print("\n=== Строки ===")
words = BTreeSet(["banana", "apple", "cherry", "apple", "date"])
print(f"Строковый set: {words}")
print(f"Итерация: {list(words)}")

print("\n=== Нагрузочный тест (100000 элементов) ===")
big = BTreeSet(range(100_000))
print(f"len = {len(big)}, height = {big.height()}, order = {big.order()}")
assert 50000 in big
assert 100001 not in big

for i in range(0, 100_000, 2):
    big.discard(i)
print(f"После удаления чётных: len = {len(big)}, height = {big.height()}")
print("Нагрузочный тест пройден!")

print("\n=== Сравнение высот: BTree vs AVL vs RB ===")
N = 100_000
data = random.sample(range(N * 10), N)

bt  = BTreeSet(data)
avl = AVLSet(data)
rb  = RBSet(data)

print(f"{N} случайных элементов:")
print(f"  B-tree (order={bt.order()}): height = {bt.height()}")
print(f"  AVL:                          height = {avl.height()}")
print(f"  Теор. макс B-tree: log_{bt.order()//2}({N}) = "
      f"{math.log(N) / math.log(bt.order()//2):.1f}")
print(f"  Теор. макс AVL:    1.44 * log2({N}) = "
      f"{1.44 * math.log2(N):.1f}")

print("\n=== Бенчмарк: вставка 100к элементов ===")
data_seq = list(range(100_000))
random.shuffle(data_seq)

for name, cls in [("BTreeSet", BTreeSet), ("AVLSet", AVLSet), ("RBSet", RBSet)]:
    t0 = time.perf_counter()
    s = cls(data_seq)
    t1 = time.perf_counter()
    print(f"  {name:12s}: {(t1-t0)*1000:.1f} ms")

print("\n=== Бенчмарк: 100к поисков ===")
lookup_keys = [random.randint(0, 99_999) for _ in range(100_000)]

for name, cls in [("BTreeSet", BTreeSet), ("AVLSet", AVLSet), ("RBSet", RBSet)]:
    s = cls(range(100_000))
    t0 = time.perf_counter()
    for k in lookup_keys:
        k in s
    t1 = time.perf_counter()
    print(f"  {name:12s}: {(t1-t0)*1000:.1f} ms")

print("\n=== del ===")
tmp = BTreeSet([1, 2, 3])
del tmp
print("tmp удалён")
