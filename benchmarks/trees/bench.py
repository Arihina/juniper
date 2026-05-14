#!/usr/bin/env python3
"""
Juniper Tree / Sorted-Set Benchmark Suite
==========================================

Сравнительное тестирование sorted-set структур:
  - RBSet       (Red-Black tree)
  - AVLSet      (AVL tree)
  - BTreeSet    (B-tree, order=64)
  - BSTSet      (unbalanced BST)
  - SkipListSet (skip list, p=0.25)
  - set         (Python built-in, baseline — неупорядоченный)

Запуск:
  python3 bench.py              # полный (N=50k)
  python3 bench.py --quick      # быстрый (N=10k)
  python3 bench.py --no-memory  # без замеров памяти
"""

import argparse
import gc
import random
import resource
import statistics
import sys
import time
import tracemalloc

from pathlib import Path


from juniper import RBSet, AVLSet, BTreeSet, BSTSet, SkipListSet

SETS = [
    ("RBSet",         RBSet),
    ("AVLSet",        AVLSet),
    ("BTreeSet",      BTreeSet),
    ("BSTSet",        BSTSet),
    ("SkipListSet",   SkipListSet),
]


# ================================================================
#  Helpers
# ================================================================

def timed(func):
    gc.disable()
    t0 = time.perf_counter()
    func()
    t1 = time.perf_counter()
    gc.enable()
    return t1 - t0


def fmt_ops(count, sec):
    if sec <= 0: return "—"
    v = count / sec
    if v >= 1e6: return f"{v/1e6:.2f}M"
    if v >= 1e3: return f"{v/1e3:.1f}K"
    return f"{v:.0f}"


def fmt_ms(sec):
    return f"{sec*1000:.1f}"


def fmt_mb(mb):
    return f"{mb:.2f}"


def get_rss_mb():
    return resource.getrusage(resource.RUSAGE_SELF).ru_maxrss / 1024


def measure_memory(func):
    gc.collect(); gc.disable()
    tracemalloc.start()
    rss0 = get_rss_mb()
    result = func()
    rss1 = get_rss_mb()
    _, peak = tracemalloc.get_traced_memory()
    tracemalloc.stop()
    gc.enable()
    return result, rss1 - rss0, peak / (1024*1024)


def print_header(title):
    print(f"\n{'='*78}")
    print(f"  {title}")
    print(f"{'='*78}")


def print_table(headers, rows):
    widths = []
    for i in range(len(headers)):
        w = len(str(headers[i]))
        for r in rows:
            if i < len(r):
                w = max(w, len(str(r[i])))
        widths.append(w + 2)
    def fmt_row(vals):
        parts = []
        for i, v in enumerate(vals):
            s = str(v)
            parts.append(s.ljust(widths[i]) if i == 0 else s.rjust(widths[i]))
        return "  " + "  ".join(parts)
    print(fmt_row(headers))
    print("  " + "  ".join("-"*w for w in widths))
    for r in rows:
        print(fmt_row(r))


# ================================================================
#  Benchmark kernels
# ================================================================

def bench_insert_seq(cls, n):
    def run():
        s = cls(range(n))
    return timed(run)


def bench_insert_rnd(cls, n, keys):
    def run():
        s = cls(keys)
    return timed(run)


def bench_insert_one_by_one(cls, n, keys):
    def run():
        s = cls()
        for k in keys:
            s.add(k)
    return timed(run)


def bench_contains_hit(cls, n, keys, lookups):
    s = cls(keys)
    def run():
        for k in lookups:
            k in s
    return timed(run)


def bench_contains_miss(cls, n, keys, miss_keys):
    s = cls(keys)
    def run():
        for k in miss_keys:
            k in s
    return timed(run)


def bench_iteration(cls, n, keys):
    s = cls(keys)
    def run():
        for _ in s:
            pass
    return timed(run)


def bench_delete_all(cls, n, keys):
    def run():
        s = cls(keys)
        for k in keys:
            s.discard(k) if hasattr(s, 'discard') else s.discard(k)
    return timed(run)


def bench_pop_all(cls, n, keys):
    """Pop min repeatedly until empty (sorted-set specific)."""
    if cls is set:
        def run():
            s = set(keys)
            while s:
                s.pop()
        return timed(run)
    def run():
        s = cls(keys)
        while len(s) > 0:
            s.pop()
    return timed(run)


def bench_mixed(cls, n, keys):
    """50% contains, 25% add, 25% discard."""
    s = cls(keys[:n//2])
    ops = []
    for _ in range(n):
        r = random.random()
        if r < 0.5:
            ops.append(('c', random.randint(0, n*2)))
        elif r < 0.75:
            ops.append(('a', random.randint(0, n*2)))
        else:
            ops.append(('d', random.randint(0, n*2)))
    def run():
        for op, k in ops:
            if op == 'c':
                k in s
            elif op == 'a':
                s.add(k)
            else:
                s.discard(k)
    return timed(run)


# ================================================================
#  Markdown reporting
# ================================================================

REPORT_DIR = Path("reports")
REPORT_DIR.mkdir(exist_ok=True)


def markdown_table(headers, rows):
    lines = []

    lines.append("| " + " | ".join(map(str, headers)) + " |")
    lines.append("| " + " | ".join("---" for _ in headers) + " |")

    for row in rows:
        lines.append("| " + " | ".join(map(str, row)) + " |")

    return "\n".join(lines)


def write_markdown_report(filename, title, headers, rows, extra_text=None):
    path = REPORT_DIR / filename

    with open(path, "a", encoding="utf-8") as f:
        f.write(f"# {title}\n\n")

        if extra_text:
            f.write(extra_text.strip() + "\n\n")

        f.write(markdown_table(headers, rows))
        f.write("\n\n")

    # print(f"  [markdown] saved -> {path}")

# ================================================================
#  Part 1: Per-operation benchmarks
# ================================================================

def part1(N):
    print_header(f"Часть 1: Производительность отдельных операций (N={N:,})")

    keys = list(range(N))
    random.shuffle(keys)
    lookups = [random.choice(keys) for _ in range(N)]
    miss_keys = list(range(N, N*2))
    random.shuffle(miss_keys)
    miss_keys = miss_keys[:N]

    tests = [
        ("Seq insert (from range)", lambda c: bench_insert_seq(c, N), N),
        ("Rnd insert (from list)",  lambda c: bench_insert_rnd(c, N, keys), N),
        ("Rnd insert (one-by-one)", lambda c: bench_insert_one_by_one(c, N, keys), N),
        ("Contains (100% hit)",     lambda c: bench_contains_hit(c, N, keys, lookups), N),
        ("Contains (100% miss)",    lambda c: bench_contains_miss(c, N, keys, miss_keys), N),
        ("Iteration (full scan)",   lambda c: bench_iteration(c, N, keys), N),
        ("Discard all",             lambda c: bench_delete_all(c, N, keys), N),
        ("Pop min until empty",     lambda c: bench_pop_all(c, N, keys), N),
        ("Mixed 50C/25A/25D",       lambda c: bench_mixed(c, N, keys), N),
    ]

    for test_name, bench_fn, ops in tests:
        print(f"\n  --- {test_name} ---")
        rows = []
        for name, cls in SETS:
            t = bench_fn(cls)
            rows.append((name, f"{fmt_ms(t)} мс", f"{fmt_ops(ops, t)} ops/s",
                         f"{t/ops*1e6:.2f} мкс"))
        print_table(["Структура", "Время", "Throughput", "мкс/op"], rows)
        md_headers = ["Структура", "Время", "Throughput", "мкс/op"]

        write_markdown_report(
            "part1_operations.md",
            f"{test_name} (N={N:,})",
            md_headers,
            rows
        )


# ================================================================
#  Part 2: Scaling
# ================================================================

def part2(sizes):
    print_header("Часть 2: Масштабируемость")
    print(f"\n  Размеры: {', '.join(f'{s:,}' for s in sizes)}")

    headers = ["N"] + [n for n, _ in SETS]

    print(f"\n  --- Random insert (мс) ---")
    rows = []
    for n in sizes:
        k = list(range(n)); random.shuffle(k)
        row = [f"{n:,}"]
        for _, cls in SETS:
            row.append(fmt_ms(bench_insert_rnd(cls, n, k)))
        rows.append(row)
    print_table(headers, rows)
    write_markdown_report(
        "part2_scaling.md",
        "Random insert (мс)",
        headers,
        rows
    )

    print(f"\n  --- Contains hit (мс) ---")
    rows = []
    for n in sizes:
        k = list(range(n)); random.shuffle(k)
        lk = [random.choice(k) for _ in range(n)]
        row = [f"{n:,}"]
        for _, cls in SETS:
            row.append(fmt_ms(bench_contains_hit(cls, n, k, lk)))
        rows.append(row)
    print_table(headers, rows)
    write_markdown_report(
        "part2_scaling.md",
        "Contains hit (мс)",
        headers,
        rows
    )

    print(f"\n  --- Iteration (мс) ---")
    rows = []
    for n in sizes:
        k = list(range(n)); random.shuffle(k)
        row = [f"{n:,}"]
        for _, cls in SETS:
            row.append(fmt_ms(bench_iteration(cls, n, k)))
        rows.append(row)
    print_table(headers, rows)
    write_markdown_report(
        "part2_scaling.md",
        "Iteration (мс)",
        headers,
        rows
    )


# ================================================================
#  Part 3: Latency percentiles
# ================================================================

def part3(N):
    print_header(f"Часть 3: Перцентили латентности contains (N={N:,})")

    keys = list(range(N)); random.shuffle(keys)
    SAMPLES = 5000
    print(f"\n  {SAMPLES} поэлементных замеров\n")

    headers = ["Структура", "avg", "p50", "p90", "p99", "p99.9"]
    rows = []
    for name, cls in SETS:
        s = cls(keys)
        sample = [random.choice(keys) for _ in range(SAMPLES)]
        lats = []
        for k in sample:
            t0 = time.perf_counter()
            k in s
            t1 = time.perf_counter()
            lats.append((t1-t0)*1e6)
        lats.sort()
        rows.append((name,
            f"{statistics.mean(lats):.2f}μs",
            f"{lats[len(lats)//2]:.2f}μs",
            f"{lats[int(len(lats)*0.90)]:.2f}μs",
            f"{lats[int(len(lats)*0.99)]:.2f}μs",
            f"{lats[min(int(len(lats)*0.999), len(lats)-1)]:.2f}μs",
        ))
    print_table(headers, rows)
    write_markdown_report(
        "part3_latency.md",
        f"Latency percentiles contains (N={N:,})",
        headers,
        rows
    )


# ================================================================
#  Part 4: Memory
# ================================================================

def part4(sizes):
    print_header("Часть 4: Потребление памяти")

    headers = ["N", "Структура", "tracemalloc MB", "RSS Δ MB"]
    rows = []
    for n in sizes:
        keys = list(range(n)); random.shuffle(keys)
        for name, cls in SETS:
            def fill():
                return cls(keys)
            _, rss_d, trace_p = measure_memory(fill)
            rows.append((f"{n:,}", name, fmt_mb(trace_p), fmt_mb(rss_d)))
        rows.append(("", "", "", ""))
    print_table(headers, rows)


# ================================================================
#  Part 5: Tree height / structure
# ================================================================

def part5(N):
    print_header(f"Часть 4: Высота деревьев и структурные свойства (N={N:,})")

    keys_rnd = list(range(N)); random.shuffle(keys_rnd)
    keys_seq = list(range(N))

    print(f"\n  --- Высота при случайной вставке ---")
    headers = ["Структура", "Высота", "Теор. min", "Примечание"]
    rows = []
    import math
    for name, cls in SETS:
        if cls is set:
            rows.append((name, "—", "—", "хэш-таблица, нет высоты"))
            continue
        s = cls(keys_rnd)
        h = s.height() if hasattr(s, 'height') else "—"
        if cls is AVLSet:
            tmin = f"{1.44*math.log2(N):.1f}"
            note = "≤ 1.44·log₂(N)"
        elif cls is BTreeSet:
            tmin = f"{math.log(N)/math.log(32):.1f}"
            note = f"log_{{t}}(N), order={s.order()}"
        elif cls is BSTSet:
            tmin = f"{math.log2(N):.1f}"
            note = "без балансировки, avg ~2·ln(N)"
        elif cls is SkipListSet:
            tmin = f"{math.log(N)/math.log(4):.1f}"
            note = "вероятностная, ~log_{1/p}(N)"
        else:
            tmin = f"{2*math.log2(N):.1f}"
            note = "≤ 2·log₂(N+1)"
        rows.append((name, str(h), tmin, note))
    print_table(headers, rows)
    write_markdown_report(
        "part5_structure.md",
        f"Tree height random insert (N={N:,})",
        headers,
        rows
    )

    print(f"\n  --- Высота при последовательной вставке (worst case) ---")
    headers = ["Структура", "Высота", "Примечание"]
    rows = []
    for name, cls in SETS:
        if cls is set:
            rows.append((name, "—", ""))
            continue
        s = cls(keys_seq)
        h = s.height() if hasattr(s, 'height') else "—"
        note = ""
        if cls is BSTSet:
            note = "ДЕГРАДАЦИЯ: вырождается в список!"
        elif cls is SkipListSet:
            note = "вероятностная — не деградирует"
        else:
            note = "балансировка сохраняется"
        rows.append((name, str(h), note))
    print_table(headers, rows)
    write_markdown_report(
        "part5_structure.md",
        f"Tree height sequential insert worst-case (N={N:,})",
        headers,
        rows
    )


# ================================================================
#  Part 6: Sorted iteration order
# ================================================================

def part6(N):
    print_header(f"Часть 5: Отсортированный обход")

    keys = [42, 7, 99, 3, 55, 18, 71]
    print(f"\n  Вставка: {keys}\n")

    headers = ["Структура", "Итерация"]
    rows = []
    for name, cls in SETS:
        s = cls(keys)
        rows.append((name, str(list(s))))
    print_table(headers, rows)
    write_markdown_report(
        "part6_iteration_order.md",
        "Sorted iteration order",
        headers,
        rows)

#     print("""
#   Python set не гарантирует порядок.
#   Все juniper sorted-set структуры итерируют in-order (по возрастанию).
#   Это ключевое отличие: sorted set = set + порядок.""")


# ================================================================
#  Entry point
# ================================================================

def main():
    parser = argparse.ArgumentParser(description="Juniper Tree Benchmark")
    parser.add_argument("--quick", action="store_true")
    parser.add_argument("--no-memory", action="store_true")
    args = parser.parse_args()

    N = 10_000 if args.quick else 50_000
    sizes = [1_000, 5_000, 10_000] if args.quick else [1_000, 10_000, 50_000]

    print("Juniper Tree / Sorted-Set Benchmark Suite")
    print(f"Python {sys.version.split()[0]}, N={N:,}, sizes={sizes}")

    for n in sizes:
        part1(n)
    part2(sizes)
    for n in sizes:
        part3(n)
    # if not args.no_memory:
    #     part4(sizes)
    for n in sizes:
        part5(n)
    for n in sizes:
        part6(n)

    print(f"\n{'='*78}")
    print("  Benchmark complete.")
    print(f"{'='*78}")


if __name__ == "__main__":
    main()
    