from juniper import SList, DList, SkipListSet

passed = 0
failed = 0


def check(name, got, expected):
    global passed, failed
    if got == expected:
        passed += 1
    else:
        failed += 1
        print(f"  FAIL: {name}: got {got!r}, expected {expected!r}")


print("=== SList: is_empty / _len ===")

s = SList()
check("empty is_empty", s.is_empty(), True)
check("empty _len", s._len(), 0)
check("empty len()", len(s), 0)

s.push_back(1)
check("1 elem is_empty", s.is_empty(), False)
check("1 elem _len", s._len(), 1)

for i in range(2, 101):
    s.push_back(i)
check("100 elem _len", s._len(), 100)
check("100 elem len()", len(s), 100)
check("100 elem is_empty", s.is_empty(), False)

s.clear()
check("after clear is_empty", s.is_empty(), True)
check("after clear _len", s._len(), 0)

print(f"  SList: {passed} passed")
p_slist = passed
passed = 0

print("\n=== DList: is_empty / _len ===")

d = DList()
check("empty is_empty", d.is_empty(), True)
check("empty _len", d._len(), 0)

d.push_back("x")
check("1 elem is_empty", d.is_empty(), False)
check("1 elem _len", d._len(), 1)

for i in range(99):
    d.push_back(i)
check("100 elem _len", d._len(), 100)
check("100 elem is_empty", d.is_empty(), False)

d.clear()
check("after clear is_empty", d.is_empty(), True)
check("after clear _len", d._len(), 0)

print(f"  DList is_empty/_len: {passed} passed")
p_dlist_basic = passed
passed = 0

print("\n=== DList: move_to_front ===")

d = DList(["a", "b", "c", "d", "e"])

d.move_to_front(2)
check("move middle", list(d), ["c", "a", "b", "d", "e"])

d.move_to_front(4)
check("move last", list(d), ["e", "c", "a", "b", "d"])

d.move_to_front(0)
check("move first (noop)", list(d), ["e", "c", "a", "b", "d"])

d.move_to_front(-1)
check("move -1", list(d), ["d", "e", "c", "a", "b"])

d.move_to_front(-2)
check("move -2", list(d), ["a", "d", "e", "c", "b"])

d2 = DList([1, 2])
d2.move_to_front(1)
check("2-elem move", list(d2), [2, 1])
d2.move_to_front(0)
check("2-elem noop", list(d2), [2, 1])

d1 = DList([42])
d1.move_to_front(0)
check("1-elem noop", list(d1), [42])

try:
    d.move_to_front(100)
    check("out of range", "no error", "IndexError")
except IndexError:
    check("out of range", "IndexError", "IndexError")

try:
    d.move_to_front(-100)
    check("neg out of range", "no error", "IndexError")
except IndexError:
    check("neg out of range", "IndexError", "IndexError")

check("len unchanged", len(d), 5)

d3 = DList(["x", "y", "z"])
d3.move_to_front(2)
result = []
for v in d3:
    result.append(v)
check("iter after move", result, ["z", "x", "y"])

check("reversed after move", list(reversed(d3)), ["y", "x", "z"])

print(f"  DList move_to_front: {passed} passed")
p_dlist_mtf = passed
passed = 0

print("\n=== DList: LRU cache simulation ===")

cache = DList(["page_A", "page_B", "page_C", "page_D"])

idx = cache.index("page_C")
cache.move_to_front(idx)
check("LRU access C", list(cache), ["page_C", "page_A", "page_B", "page_D"])

idx = cache.index("page_D")
cache.move_to_front(idx)
check("LRU access D", list(cache), ["page_D", "page_C", "page_A", "page_B"])

evicted = cache.pop_back()
check("LRU evict", evicted, "page_B")
check("LRU remaining", list(cache), ["page_D", "page_C", "page_A"])

cache.push_front("page_E")
check("LRU add new", list(cache), ["page_E", "page_D", "page_C", "page_A"])

idx = cache.index("page_A")
cache.move_to_front(idx)
check("LRU access A", list(cache), ["page_A", "page_E", "page_D", "page_C"])

while len(cache) > 2:
    cache.pop_back()
check("LRU shrink to 2", list(cache), ["page_A", "page_E"])

print(f"  DList LRU sim: {passed} passed")
p_lru = passed
passed = 0

print("\n=== SkipListSet: is_empty / _len ===")

sl = SkipListSet()
check("empty is_empty", sl.is_empty(), True)
check("empty _len", sl._len(), 0)

sl.add(42)
check("1 elem is_empty", sl.is_empty(), False)
check("1 elem _len", sl._len(), 1)

for i in range(99):
    sl.add(i)

check("99 elem _len", sl._len(), 99)
check("99 elem is_empty", sl.is_empty(), False)

sl.clear()
check("after clear is_empty", sl.is_empty(), True)
check("after clear _len", sl._len(), 0)

print(f"  SkipListSet is_empty/_len: {passed} passed")
p_skip_basic = passed
passed = 0

print("\n=== SkipListSet: peek_min ===")

sl = SkipListSet()

try:
    sl.peek_min()
    check("empty peek_min", "no error", "KeyError")
except KeyError:
    check("empty peek_min", "KeyError", "KeyError")

sl.add(100)
check("single peek_min", sl.peek_min(), 100)
check("len after peek", len(sl), 1)

for x in [50, 200, 10, 75]:
    sl.add(x)
check("multi peek_min", sl.peek_min(), 10)

check("peek_min again", sl.peek_min(), 10)
check("peek_min 3rd time", sl.peek_min(), 10)
check("len still same", len(sl), 5)

val = sl.pop()
check("pop returns min", val, 10)
check("peek_min after pop", sl.peek_min(), 50)

sl.pop()
check("peek_min after 2nd pop", sl.peek_min(), 75)

sl.add(1)
check("peek_min with new min", sl.peek_min(), 1)

sl2 = SkipListSet(["banana", "apple", "cherry"])
check("string peek_min", sl2.peek_min(), "apple")

print("\n--- purge_expired pattern demo ---")
import time as _time

ttls = SkipListSet()
now = _time.time()
for offset in [0.1, 0.5, 1.0, 2.0, 5.0]:
    ttls.add(now + offset)

threshold = now + 0.6
purged = 0
while not ttls.is_empty() and ttls.peek_min() < threshold:
    ttls.pop()
    purged += 1
check("purged expired", purged, 2)
check("remaining", ttls._len(), 3)

print(f"  SkipListSet peek_min: {passed} passed")
p_skip_peek = passed
passed = 0

total = p_slist + p_dlist_basic + p_dlist_mtf + p_lru + p_skip_basic + p_skip_peek
print(f"\n{'='*50}")
print(f"  TOTAL: {total} passed, {failed} failed")
if failed == 0:
    print("  ALL TESTS PASSED!")
else:
    print(f"  {failed} TESTS FAILED")
print(f"{'='*50}")
