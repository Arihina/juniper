from juniper import SList, DList

def test_list(cls, name):
    print(f"\n{'='*50}")
    print(f"  {name}")
    print(f"{'='*50}")

    print("\n--- Создание ---")
    lst = cls()
    print(f"Пустой: {lst}, len={len(lst)}")

    lst2 = cls([10, 20, 30])
    print(f"Из iterable: {lst2}, len={len(lst2)}")

    print("\n--- push_front / push_back ---")
    lst.push_back("a")
    lst.push_back("b")
    lst.push_back("c")
    lst.push_front("z")
    print(f"После push: {lst}")

    print("\n--- insert ---")
    lst.insert(2, "INSERTED")
    print(f"insert(2, 'INSERTED'): {lst}")

    print("\n--- [] (getitem) ---")
    print(f"lst[0]  = {lst[0]}")
    print(f"lst[-1] = {lst[-1]}")
    print(f"lst[2]  = {lst[2]}")

    print("\n--- in (contains) ---")
    print(f"'a' in lst -> {'a' in lst}")
    print(f"'x' in lst -> {'x' in lst}")

    print("\n--- index ---")
    print(f"lst.index('b') = {lst.index('b')}")

    print("\n--- Итерация ---")
    print("for:", list(lst))

    print("\n--- pop_front / pop_back ---")
    print(f"pop_front -> {lst.pop_front()}")
    print(f"pop_back  -> {lst.pop_back()}")
    print(f"После pop: {lst}")

    print("\n--- remove ---")
    lst.remove("INSERTED")
    print(f"remove('INSERTED'): {lst}")

    try:
        lst.remove("nonexistent")
    except ValueError as e:
        print(f"remove('nonexistent') -> ValueError: {e}")

    print("\n--- reverse ---")
    lst.reverse()
    print(f"После reverse: {lst}")

    print("\n--- == / != ---")
    a = cls([1, 2, 3])
    b = cls([1, 2, 3])
    c = cls([3, 2, 1])
    print(f"{name}([1,2,3]) == {name}([1,2,3]) -> {a == b}")
    print(f"{name}([1,2,3]) != {name}([3,2,1]) -> {a != c}")

    print("\n--- clear ---")
    a.clear()
    print(f"После clear: {a}, len={len(a)}")

    print("\n--- del ---")
    tmp = cls([1])
    del tmp
    print("del OK")


test_list(SList, "SList")
test_list(DList, "DList")

print("\n\n=== DList: reversed() ===")
d = DList([1, 2, 3, 4, 5])
print(f"Прямой:   {list(d)}")
print(f"Обратный: {list(reversed(d))}")
