from juniper import AVLSet, RBSet

print("=== Создание ===")
s = AVLSet()
print(f"Пустой set: {s}")
print(f"len = {len(s)}")

print("\n=== add ===")
for x in [5, 3, 8, 1, 4, 7, 9, 2, 6, 10]:
    s.add(x)
print(f"После добавления 1..10: {s}")
print(f"len = {len(s)}")
print(f"height = {s.height()}")

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
s2 = AVLSet([30, 10, 20, 10, 20])
print(f"AVLSet([30, 10, 20, 10, 20]) = {s2}")
print(f"len = {len(s2)}")

print("\n=== Сравнение == / != ===")
a = AVLSet([1, 2, 3])
b = AVLSet([3, 2, 1])
c = AVLSet([1, 2])
print(f"AVLSet([1,2,3]) == AVLSet([3,2,1]) -> {a == b}")
print(f"AVLSet([1,2,3]) != AVLSet([1,2])   -> {a != c}")

print("\n=== clear ===")
a.clear()
print(f"После clear: {a}, len = {len(a)}")

print("\n=== Строки ===")
words = AVLSet(["banana", "apple", "cherry", "apple", "date"])
print(f"Строковый set: {words}")
print(f"Итерация: {list(words)}")

print("\n=== Нагрузочный тест (100000 элементов) ===")
big = AVLSet(range(100_000))
print(f"len = {len(big)}")
print(f"height = {big.height()}")
assert 50000 in big
assert 100001 not in big
# Удалим половину
for i in range(0, 100_000, 2):
    big.discard(i)
print(f"После удаления чётных: len = {len(big)}, height = {big.height()}")
print("Нагрузочный тест пройден!")

print("\n=== Сравнение высот AVL vs RB ===")
import random
data = random.sample(range(1_000_000), 50_000)
avl = AVLSet(data)
rb  = RBSet(data)
print(f"50000 случайных элементов:")
print(f"  AVL height = {avl.height()}")
print(f"  (RB-tree не имеет метода height для сравнения, но AVL гарантирует <= 1.44 * log2(n))")
import math
theoretical_max = 1.44 * math.log2(50000)
print(f"  Теоретический максимум AVL: {theoretical_max:.1f}")
print(f"  Реальная высота:            {avl.height()}")

print("\n=== del ===")
s3 = AVLSet([1, 2, 3])
del s3
print("s3 удалён")
