#!/usr/bin/env python3
"""
Juniper List Benchmark Suite
==============================

Сравнительное тестирование списков:
  - SList      (singly linked list)
  - DList      (doubly linked list)
  - list       (Python built-in, baseline — dynamic array)

Запуск:
  python3 bench.py              # полный (N=50k)
  python3 bench.py --quick      # быстрый (N=10k)
  python3 bench.py --no-memory  # без замеров памяти
"""

import argparse
import gc
from pathlib import Path
import random
import resource
import statistics
import sys
import time
import tracemalloc

from juniper import SList, DList

LISTS = [
    ("list (builtin)", list),
    ("SList",          SList),
    ("DList",          DList),
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

def bench_push_back(cls, n):
    """Append N elements to back."""
    if cls is list:
        def run():
            l = []
            for i in range(n): l.append(i)
    else:
        def run():
            l = cls()
            for i in range(n): l.push_back(i)
    return timed(run)


def bench_push_front(cls, n):
    """Prepend N elements to front."""
    if cls is list:
        def run():
            l = []
            for i in range(n): l.insert(0, i)
    else:
        def run():
            l = cls()
            for i in range(n): l.push_front(i)
    return timed(run)


def bench_pop_front(cls, n):
    """Push N then pop all from front."""
    if cls is list:
        def run():
            l = list(range(n))
            for _ in range(n): l.pop(0)
    else:
        def run():
            l = cls(range(n))
            for _ in range(n): l.pop_front()
    return timed(run)


def bench_pop_back(cls, n):
    """Push N then pop all from back."""
    if cls is list:
        def run():
            l = list(range(n))
            for _ in range(n): l.pop()
    else:
        def run():
            l = cls(range(n))
            for _ in range(n): l.pop_back()
    return timed(run)


def bench_iteration(cls, n):
    """Iterate over N elements."""
    l = cls(range(n)) if cls is not list else list(range(n))
    def run():
        for _ in l: pass
    return timed(run)


def bench_index_access_seq(cls, n, step):
    """Sequential index access: l[0], l[step], l[2*step], ..."""
    l = cls(range(n)) if cls is not list else list(range(n))
    indices = list(range(0, n, step))
    def run():
        for i in indices:
            l[i]
    return timed(run), len(indices)


def bench_contains(cls, n, lookups):
    """N `in` checks on list of size N."""
    l = cls(range(n)) if cls is not list else list(range(n))
    def run():
        for k in lookups:
            k in l
    return timed(run)


def bench_reverse(cls, n):
    """Reverse a list of N elements."""
    if cls is list:
        def run():
            l = list(range(n))
            l.reverse()
    else:
        def run():
            l = cls(range(n))
            l.reverse()
    return timed(run)


def bench_from_iterable(cls, n):
    """Create from range(N)."""
    r = range(n)
    def run():
        cls(r)
    return timed(run)


# ================================================================
#  DList-specific benchmarks
# ================================================================

def bench_dlist_move_to_front_idx(n):
    """Move random indices to front N times."""
    d = DList(range(n))
    indices = [random.randint(0, n-1) for _ in range(n)]
    def run():
        for i in indices:
            d.move_to_front(i % len(d))
    return timed(run)


def bench_dlist_node_handles(n):
    """push_back_node + move_to_front_node + pop_back_node (LRU pattern)."""
    def run():
        d = DList()
        handles = []
        # fill
        for i in range(n):
            handles.append(d.push_back_node(i))
        # access random → move to front
        for _ in range(n):
            h = random.choice(handles)
            if h.is_attached:
                d.move_to_front_node(h)
        # evict from back
        while len(d) > 0:
            d.pop_back_node()
    return timed(run)


def bench_dlist_reversed_iter(n):
    """Reversed iteration (DList only)."""
    d = DList(range(n))
    def run():
        for _ in reversed(d): pass
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

    lookups_few = [random.randint(0, N-1) for _ in range(min(N, 1000))]

    tests = [
        ("push_back ×N",       lambda c: bench_push_back(c, N), N),
        ("push_front ×N",      lambda c: bench_push_front(c, N), N),
        ("pop_front ×N",       lambda c: bench_pop_front(c, N), N),
        ("pop_back ×N",        lambda c: bench_pop_back(c, N), N),
        ("Iteration",          lambda c: bench_iteration(c, N), N),
        ("Reverse",            lambda c: bench_reverse(c, N), N),
        ("From iterable",      lambda c: bench_from_iterable(c, N), N),
    ]

    for test_name, bench_fn, ops in tests:
        print(f"\n  --- {test_name} ---")
        rows = []
        for name, cls in LISTS:
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

    # contains (O(n) for all — small sample)
    print(f"\n  --- Contains ×{len(lookups_few)} (linear scan) ---")
    rows = []
    for name, cls in LISTS:
        t = bench_contains(cls, N, lookups_few)
        rows.append((name, f"{fmt_ms(t)} мс", f"{fmt_ops(len(lookups_few), t)} ops/s",
                     f"{t/len(lookups_few)*1e6:.2f} мкс"))
    print_table(["Структура", "Время", "Throughput", "мкс/op"], rows)
    write_markdown_report(
        "part1_operations.md",
        f"Contains linear scan ×{len(lookups_few)} (N={N:,})",
        ["Структура", "Время", "Throughput", "мкс/op"],
        rows
    )

    # Index access
    print(f"\n  --- Index access [0], [100], [200], ... ---")
    rows = []
    for name, cls in LISTS:
        t, cnt = bench_index_access_seq(cls, N, 100)
        rows.append((name, f"{fmt_ms(t)} мс", f"{fmt_ops(cnt, t)} ops/s",
                     f"{t/cnt*1e6:.2f} мкс"))
    print_table(["Структура", "Время", "Throughput", "мкс/op"], rows)
    write_markdown_report(
        "part1_operations.md",
        f"Index access sequential (N={N:,})",
        ["Структура", "Время", "Throughput", "мкс/op"],
        rows
    )


# ================================================================
#  Part 2: Scaling
# ================================================================

def part2(sizes):
    print_header("Часть 2: Масштабируемость")
    print(f"\n  Размеры: {', '.join(f'{s:,}' for s in sizes)}")

    headers = ["N"] + [n for n, _ in LISTS]

    print(f"\n  --- push_back ×N (мс) ---")
    rows = []
    for n in sizes:
        row = [f"{n:,}"]
        for _, cls in LISTS:
            row.append(fmt_ms(bench_push_back(cls, n)))
        rows.append(row)
    print_table(headers, rows)
    write_markdown_report(
        "part2_scaling.md",
        "push_back ×N (мс)",
        headers,
        rows
    )

    print(f"\n  --- push_front ×N (мс) ---")
    rows = []
    for n in sizes:
        row = [f"{n:,}"]
        for _, cls in LISTS:
            row.append(fmt_ms(bench_push_front(cls, n)))
        rows.append(row)
    print_table(headers, rows)
    write_markdown_report(
        "part2_scaling.md",
        "push_front ×N (мс)",
        headers,
        rows
    )

    print(f"\n  --- Iteration (мс) ---")
    rows = []
    for n in sizes:
        row = [f"{n:,}"]
        for _, cls in LISTS:
            row.append(fmt_ms(bench_iteration(cls, n)))
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
    print_header(f"Часть 3: Перцентили латентности push_back (N={N:,})")

    SAMPLES = 5000
    print(f"\n  {SAMPLES} поэлементных замеров на уже заполненном списке\n")

    headers = ["Структура", "avg", "p50", "p90", "p99", "p99.9"]
    rows = []
    for name, cls in LISTS:
        if cls is list:
            l = list(range(N))
        else:
            l = cls(range(N))

        lats = []
        for i in range(SAMPLES):
            v = N + i
            if cls is list:
                t0 = time.perf_counter()
                l.append(v)
                t1 = time.perf_counter()
            else:
                t0 = time.perf_counter()
                l.push_back(v)
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
        f"push_back latency percentiles (N={N:,})",
        headers,
        rows,
    )

#     print("""
#   Примечание: list (Python) иногда показывает высокий p99.9 из-за
#   realloc при росте внутреннего массива. Linked lists не делают realloc,
#   но каждый push — это malloc, что стабильнее по latency.""")


# ================================================================
#  Part 4: Memory
# ================================================================

def part4(sizes):
    print_header("Часть 4: Потребление памяти")

    headers = ["N", "Структура", "tracemalloc MB", "RSS Δ MB"]
    rows = []
    for n in sizes:
        for name, cls in LISTS:
            def fill():
                return cls(range(n))
            _, rss_d, trace_p = measure_memory(fill)
            rows.append((f"{n:,}", name, fmt_mb(trace_p), fmt_mb(rss_d)))
        rows.append(("", "", "", ""))
    print_table(headers, rows)

    print("""
  list (Python) — компактный: один непрерывный массив указателей.
  SList — 2 указателя на узел (value + next) + malloc overhead.
  DList — 3 указателя на узел (value + prev + next) + malloc overhead.
  Linked lists потребляют в 3-5x больше памяти чем array list.""")


# ================================================================
#  Part 5: DList-specific features
# ================================================================

def part5(N):
    print_header(f"Часть 5: DList — специфичные операции (N={N:,})")

    print(f"\n  --- move_to_front (по индексу) ×{N:,} ---")
    t = bench_dlist_move_to_front_idx(N)
    print(f"  Время: {fmt_ms(t)} мс  ({fmt_ops(N, t)} ops/s)")
    print(f"  Примечание: O(n) поиск по индексу + O(1) перемещение")

    print(f"\n  --- LRU pattern через node handles ×{N:,} ---")
    print(f"  (push_back_node + move_to_front_node + pop_back_node)")
    t = bench_dlist_node_handles(N)
    print(f"  Время: {fmt_ms(t)} мс")
    print(f"  Примечание: O(1) на каждую операцию, без поиска по индексу")

    print(f"\n  --- reversed() iteration ---")
    t = bench_dlist_reversed_iter(N)
    t_fwd = bench_iteration(DList, N)
    print(f"  Forward:  {fmt_ms(t_fwd)} мс")
    print(f"  Reversed: {fmt_ms(t)} мс")
    rows = [
        (
            "move_to_front(index)",
            f"{fmt_ms(bench_dlist_move_to_front_idx(N))} мс",
            "O(n) поиск + O(1) move"
        ),
        (
            "node handles LRU",
            f"{fmt_ms(bench_dlist_node_handles(N))} мс",
            "O(1) operations"
        ),
        (
            "reversed iteration",
            f"{fmt_ms(bench_dlist_reversed_iter(N))} мс",
            "native reverse iterator"
        ),
    ]

    write_markdown_report(
        "part5_dlist.md",
        f"DList specific operations (N={N:,})",
        ["Операция", "Время", "Примечание"],
        rows,
    )
#     print("""
#   Рекомендация:
#     Для LRU-кэша используйте node handles (push_back_node / move_to_front_node).
#     Это даёт O(1) на все операции — в отличие от move_to_front(index),
#     который тратит O(n) на поиск узла по индексу.""")


# ================================================================
#  Part 6: Complexity comparison
# ================================================================

def part6():
    print_header("Часть 6: Теоретическая сложность операций")

    print("""
  ┌──────────────────┬────────────────┬────────────────┬────────────────┐
  │ Операция         │ list (array)   │ SList          │ DList          │
  ├──────────────────┼────────────────┼────────────────┼────────────────┤
  │ push_back        │ O(1) аморт.    │ O(1)           │ O(1)           │
  │ push_front       │ O(n) сдвиг     │ O(1)           │ O(1)           │
  │ pop_back         │ O(1)           │ O(n) обход     │ O(1)           │
  │ pop_front        │ O(n) сдвиг     │ O(1)           │ O(1)           │
  │ insert(i, v)     │ O(n)           │ O(n)           │ O(n)           │
  │ l[i] (доступ)    │ O(1)           │ O(n)           │ O(n) от края   │
  │ contains (in)    │ O(n)           │ O(n)           │ O(n)           │
  │ remove(v)        │ O(n)           │ O(n)           │ O(n)           │
  │ reverse          │ O(n)           │ O(n)           │ O(n)           │
  │ iteration        │ O(n) cache✓    │ O(n)           │ O(n)           │
  │ move_to_front(h) │ —              │ —              │ O(1) ← handle  │
  │ move_to_back(h)  │ —              │ —              │ O(1) ← handle  │
  │ unlink(h)        │ —              │ —              │ O(1) ← handle  │
  │ reversed()       │ O(n)           │ —              │ O(n) нативно   │
  ├──────────────────┼────────────────┼────────────────┼────────────────┤
  │ Память на элем.  │ 8 байт (ptr)   │ ~24 байт       │ ~32 байт       │
  │ Cache locality   │ отличная       │ плохая         │ плохая         │
  └──────────────────┴────────────────┴────────────────┴────────────────┘

  list (array) выигрывает на iteration и random access (cache locality).
  Linked lists выигрывают на push_front / pop_front и O(1) node operations.
  DList + node handles = идеальная основа для LRU-кэша.""")


# ================================================================
#  Entry point
# ================================================================

def main():
    parser = argparse.ArgumentParser(description="Juniper List Benchmark")
    parser.add_argument("--quick", action="store_true")
    parser.add_argument("--no-memory", action="store_true")
    args = parser.parse_args()

    N = 10_000 if args.quick else 50_000
    sizes = [1_000, 5_000, 10_000] if args.quick else [1_000, 10_000, 50_000]

    print("Juniper List Benchmark Suite")
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
    part6()

    print(f"\n{'='*78}")
    print("  Benchmark complete.")
    print(f"{'='*78}")


if __name__ == "__main__":
    main()