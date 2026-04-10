from juniper import RBSet

s = RBSet()
print(f"len = {len(s)}")

for x in [5, 3, 8, 1, 4, 7, 9, 2, 6, 10]:
    s.add(x)
print(f"len = {len(s)}")

s.add(5)
s.add(3)
print(f"len = {len(s)}")

print(f"5 in s  -> {5 in s}")
print(f"42 in s -> {42 in s}")

print("Элементы:", list(s))

for val in s:
    print(val, end=" ")
print()

s.discard(100)

s.remove(5)
print(f"После remove(5): {s}")
print(f"len = {len(s)}")

try:
    s.remove(5)
except KeyError as e:
    print(f"remove(5) повторно -> KeyError: {e}")

while len(s) > 5:
    val = s.pop()
    print(f"pop -> {val}", end="  ")
print(f"\nОсталось: {s}")

s2 = RBSet([30, 10, 20, 10, 20])
print(f"RBSet([30, 10, 20, 10, 20]) = {s2}")
print(f"len = {len(s2)}")

a = RBSet([1, 2, 3])
b = RBSet([3, 2, 1])
c = RBSet([1, 2])
print(f"RBSet([1,2,3]) == RBSet([3,2,1]) -> {a == b}")
print(f"RBSet([1,2,3]) != RBSet([1,2])   -> {a != c}")

a.clear()
print(f"После clear: {a}, len = {len(a)}")

words = RBSet(["banana", "apple", "cherry", "apple", "date"])
print(f"Строковый set: {words}")
print(f"Итерация: {list(words)}")

s3 = RBSet([1, 2, 3])
print(f"До del: {s3}")
del s3
