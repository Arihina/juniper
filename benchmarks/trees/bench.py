"""
Juniper Tree / Ordered Set Benchmark Suite
==========================================

Сравнительное тестирование ordered-структур:
  BSTSet, AVLSet, RBSet, BTreeSet, SkipListSet, set (baseline)

Метрики:
1. Throughput операций
2. Latency перцентили
3. Масштабируемость
4. Потребление памяти (tracemalloc + RSS)
5. Влияние паттерна ключей
6. Высота деревьев
"""

import argparse
import gc
import random
import resource
import statistics
import sys
import time
import tracemalloc

from juniper import BSTSet, AVLSet, RBSet, BTreeSet, SkipListSet

STRUCTS = [
    ("set", set),
    ("BSTSet", BSTSet),
    ("AVLSet", AVLSet),
    ("RBSet", RBSet),
    ("BTreeSet", BTreeSet),
    ("SkipListSet", SkipListSet),
]

# ------------------------------------------------

def make_struct(cls):
    return set() if cls is set else cls()

def add(s, x):
    s.add(x)

def contains(s, x):
    return (x in s) if isinstance(s, set) else s.contains(x)

def remove(s, x):
    s.remove(x)

def get_height(s):
    if isinstance(s, set):
        return "—"
    if hasattr(s, "height"):
        return s.height()
    return "—"

def get_rss_mb():
    return resource.getrusage(resource.RUSAGE_SELF).ru_maxrss / 1024

def timed(func):
    gc.disable()
    t0 = time.perf_counter()
    func()
    t1 = time.perf_counter()
    gc.enable()
    return t1 - t0

def measure_memory(func):
    gc.collect()
    gc.disable()
    tracemalloc.start()
    rss_before = get_rss_mb()
    func()
    rss_after = get_rss_mb()
    _, peak = tracemalloc.get_traced_memory()
    tracemalloc.stop()
    gc.enable()
    return rss_after - rss_before, peak / (1024*1024)

# ------------------------------------------------
# Bench kernels
# ------------------------------------------------

def bench_insert(cls, keys):
    def run():
        s = make_struct(cls)
        for k in keys:
            add(s, k)
    return timed(run)

def bench_lookup(cls, keys):
    s = make_struct(cls)
    for k in keys:
        add(s, k)
    random.shuffle(keys)
    def run():
        for k in keys:
            contains(s, k)
    return timed(run)

def bench_delete(cls, keys):
    def run():
        s = make_struct(cls)
        for k in keys:
            add(s, k)
        for k in keys:
            remove(s, k)
    return timed(run)

def bench_mixed(cls, keys):
    s = make_struct(cls)
    for k in keys:
        add(s, k)
    ops = []
    for _ in range(len(keys)):
        if random.random() < 0.8:
            ops.append(("r", random.choice(keys)))
        else:
            ops.append(("w", random.randint(0, len(keys)*3)))
    def run():
        for op,k in ops:
            if op=="r":
                contains(s,k)
            else:
                add(s,k)
    return timed(run)

# ------------------------------------------------
# Part 1 — operations
# ------------------------------------------------

def part1(N):
    print("\n=== PART 1: Operations ===")
    keys=list(range(N)); random.shuffle(keys)

    tests=[
        ("Insert", bench_insert),
        ("Lookup", bench_lookup),
        ("Delete", bench_delete),
        ("Mixed 80R/20W", bench_mixed)
    ]

    for title,fn in tests:
        print(f"\n--- {title} ---")
        for name,cls in STRUCTS:
            t=fn(cls, keys.copy())
            print(f"{name:12s} {t:.3f}s")

# ------------------------------------------------
# Part 2 — scaling
# ------------------------------------------------

def part2(sizes):
    print("\n=== PART 2: Scaling insert ===")
    for n in sizes:
        keys=list(range(n)); random.shuffle(keys)
        print(f"\nN={n}")
        for name,cls in STRUCTS:
            print(name, f"{bench_insert(cls,keys.copy()):.3f}s")

# ------------------------------------------------
# Part 3 — latency percentiles
# ------------------------------------------------

def part3(N):
    print("\n=== PART 3: Lookup latency ===")
    keys=list(range(N)); random.shuffle(keys)
    SAMPLES=5000

    for name,cls in STRUCTS:
        s=make_struct(cls)
        for k in keys: add(s,k)

        sample=[random.choice(keys) for _ in range(SAMPLES)]
        lats=[]
        for k in sample:
            t0=time.perf_counter()
            contains(s,k)
            lats.append((time.perf_counter()-t0)*1e6)

        lats.sort()
        print(name,
              "avg",round(statistics.mean(lats),2),
              "p99",round(lats[int(len(lats)*0.99)],2))

# ------------------------------------------------
# Part 4 — memory
# ------------------------------------------------

def part4(sizes):
    print("\n=== PART 4: Memory ===")
    for n in sizes:
        keys=list(range(n)); random.shuffle(keys)
        print(f"\nN={n}")
        for name,cls in STRUCTS:
            def fill():
                s=make_struct(cls)
                for k in keys: add(s,k)
            rss,trace=measure_memory(fill)
            print(name,"RSS",round(rss,2),"MB","trace",round(trace,2),"MB")

# ------------------------------------------------
# Part 5 — height
# ------------------------------------------------

def part5(N):
    print("\n=== PART 5: Tree height ===")
    keys=list(range(N)); random.shuffle(keys)
    for name,cls in STRUCTS:
        s=make_struct(cls)
        for k in keys: add(s,k)
        print(name,"height:",get_height(s))

# ------------------------------------------------

def main():
    parser=argparse.ArgumentParser()
    parser.add_argument("--quick",action="store_true")
    args=parser.parse_args()

    if args.quick:
        N=10000; sizes=[1000,5000,10000]
    else:
        N=50000; sizes=[1000,10000,50000]

    print("Juniper Tree Benchmark Suite")
    part1(N)
    part2(sizes)
    part3(N)
    part4(sizes)
    part5(N)

if __name__=="__main__":
    main()
    