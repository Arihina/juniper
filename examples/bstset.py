from juniper import BSTSet, RBSet, AVLSet
import random
import time

print("=== Создание ===")
s = BSTSet()
print(f"Пустой set: {s}")
print(f"len = {len(s)}")

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
print(f"len = {len(s)}, height = {s.height()}")

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
s2 = BSTSet([30, 10, 20, 10, 20])
print(f"BSTSet([30, 10, 20, 10, 20]) = {s2}")
print(f"len = {len(s2)}")

print("\n=== Сравнение == / != ===")
a = BSTSet([1, 2, 3])
b = BSTSet([3, 2, 1])
c = BSTSet([1, 2])
print(f"BSTSet([1,2,3]) == BSTSet([3,2,1]) -> {a == b}")
print(f"BSTSet([1,2,3]) != BSTSet([1,2])   -> {a != c}")

print("\n=== clear ===")
a.clear()
print(f"После clear: {a}, len = {len(a)}")

print("\n=== Строки ===")
words = BSTSet(["banana", "apple", "cherry", "apple", "date"])
print(f"Строковый set: {words}")
print(f"Итерация: {list(words)}")

print("\n=== Деградация на отсортированных данных ===")
sorted_set = BSTSet(range(1000))
print(f"BSTSet(range(1000)): height = {sorted_set.height()}")
print("  (без балансировки — вырождается в список!)")

random_set = BSTSet(random.sample(range(1000), 1000))
print(f"BSTSet(random 1000): height = {random_set.height()}")
print("  (на случайных данных — гораздо лучше)")

print("\n=== Сравнение высот: BST vs AVL vs RB (случайные 100к) ===")
N = 100_000
data = random.sample(range(N * 10), N)

bst = BSTSet(data)
avl = AVLSet(data)

print(f"  BST:  height = {bst.height()}")
print(f"  AVL:  height = {avl.height()}")

print("\n=== Бенчмарк ===")
for name, cls in [("BSTSet", BSTSet), ("AVLSet", AVLSet), ("RBSet", RBSet)]:
    t0 = time.perf_counter()
    s = cls(data)
    t1 = time.perf_counter()
    print(f"  {name:8s}: insert {(t1-t0)*1000:.1f} ms")

print("\n=== del ===")
tmp = BSTSet([1, 2, 3])
del tmp
print("tmp удалён")
