"""
Juniper Hash Map Benchmark Suite
=================================

Нагрузочное и сравнительное тестирование хэш-таблиц:
  HashMap, RHMap, BTHashMap, SwissTable, dict (baseline)

Запуск:
  python3 hashmaps.py              # полный прогон
  python3 hashmaps.py --quick      # быстрый (N=10k)
  python3 hashmaps.py --no-memory  # без замеров памяти

Критерии оценки
---------------
1. Throughput    — операций/сек при заданном N
2. Latency       — мкс на операцию (среднее) и перцентили p50/p99/p99.9
3. Масштабируемость — как меняется throughput при росте N
4. Потребление памяти — RSS и tracemalloc на разных N
5. Паттерн ключей — sequential, random, Zipf (hot/cold)
6. Операционный микс — insert, lookup, update, delete, mixed
"""

import argparse
import gc
import os
from pathlib import Path
import random
import resource
import statistics
import sys
import time
import tracemalloc

from juniper import HashMap, RHMap, BTHashMap, SwissTable

# ================================================================
#  Configuration
# ================================================================

MAPS = [
    ("dict",       dict),
    ("HashMap",    HashMap),
    ("RHMap",      RHMap),
    ("BTHashMap",  BTHashMap),
    ("SwissTable", SwissTable),
]

ZIPF_ALPHA = 1.2


# ================================================================
#  Helpers
# ================================================================

def make_map(cls, n=0):
    if cls is HashMap:
        cap = max(16, n * 2) if n else 1024
        return HashMap(cap)
    if cls is dict:
        return {}
    return cls()


def zipf_keys(n, space):
    return [int(random.paretovariate(ZIPF_ALPHA)) % space for _ in range(n)]


def get_rss_mb():
    return resource.getrusage(resource.RUSAGE_SELF).ru_maxrss / 1024


def measure_memory(func):
    gc.collect()
    gc.disable()
    tracemalloc.start()
    rss_before = get_rss_mb()

    result = func()

    rss_after = get_rss_mb()
    _, peak = tracemalloc.get_traced_memory()
    tracemalloc.stop()
    gc.enable()

    return result, rss_after - rss_before, peak / (1024 * 1024)


def timed(func):
    gc.disable()
    t0 = time.perf_counter()
    func()
    t1 = time.perf_counter()
    gc.enable()
    return t1 - t0


def fmt_ops(count, sec):
    if sec <= 0:
        return "—"
    v = count / sec
    if v >= 1_000_000:
        return f"{v / 1_000_000:.2f}M"
    if v >= 1_000:
        return f"{v / 1_000:.1f}K"
    return f"{v:.0f}"


def fmt_ms(sec):
    return f"{sec * 1000:.1f}"


def fmt_mb(mb):
    return f"{mb:.2f}"


# ================================================================
#  Table printer
# ================================================================

def print_header(title):
    print(f"\n{'=' * 78}")
    print(f"  {title}")
    print(f"{'=' * 78}")


def print_table(headers, rows):
    widths = []
    for i in range(len(headers)):
        w = len(str(headers[i]))
        for r in rows:
            w = max(w, len(str(r[i])))
        widths.append(w + 2)

    def fmt_row(vals):
        parts = []
        for i, v in enumerate(vals):
            s = str(v)
            parts.append(s.ljust(widths[i]) if i == 0 else s.rjust(widths[i]))
        return "  " + "  ".join(parts)

    print(fmt_row(headers))
    print("  " + "  ".join("-" * w for w in widths))
    for r in rows:
        print(fmt_row(r))


# ================================================================
#  Benchmark kernels
# ================================================================

def bench_insert_seq(cls, n):
    def run():
        m = make_map(cls, n)
        for i in range(n):
            m[i] = i
    return timed(run)


def bench_insert_rnd(cls, n, keys):
    def run():
        m = make_map(cls, n)
        for k in keys:
            m[k] = k
    return timed(run)


def bench_lookup_hit(cls, n, keys, lookups):
    m = make_map(cls, n)
    for k in keys:
        m[k] = k
    def run():
        for k in lookups:
            m[k]
    return timed(run)


def bench_lookup_miss(cls, n, keys, miss_keys):
    m = make_map(cls, n)
    for k in keys:
        m[k] = k
    def run():
        for k in miss_keys:
            k in m
    return timed(run)


def bench_lookup_zipf(cls, n, keys, zipf_lk):
    m = make_map(cls, n)
    for k in keys:
        m[k] = k
    def run():
        for k in zipf_lk:
            m[k]
    return timed(run)


def bench_update(cls, n, keys):
    m = make_map(cls, n)
    for k in keys:
        m[k] = k
    def run():
        for k in keys:
            m[k] = 999
    return timed(run)


def bench_delete(cls, n, keys):
    def run():
        m = make_map(cls, n)
        for k in keys:
            m[k] = k
        for k in keys:
            del m[k]
    return timed(run)


def bench_mixed(cls, n, keys):
    m = make_map(cls, n)
    for k in keys:
        m[k] = k
    ops = []
    for _ in range(n):
        if random.random() < 0.8:
            ops.append(('r', random.choice(keys)))
        else:
            ops.append(('w', random.randint(0, n * 3)))
    def run():
        for op, k in ops:
            if op == 'r':
                k in m
            else:
                m[k] = k
    return timed(run)


# ================================================================
#  Part 1: Per-operation benchmarks
# ================================================================

def part1_operations(N):
    print_header(f"Часть 1: Производительность отдельных операций (N={N:,})")

    keys = list(range(N))
    random.shuffle(keys)
    lookups = [random.choice(keys) for _ in range(N)]
    miss_keys = list(range(N, N * 2))
    random.shuffle(miss_keys)
    miss_keys = miss_keys[:N]
    zipf_lk = [k % N for k in zipf_keys(N, N)]

    tests = [
        ("Seq insert",     lambda c: bench_insert_seq(c, N), N),
        ("Rnd insert",     lambda c: bench_insert_rnd(c, N, keys), N),
        ("Lookup hit",     lambda c: bench_lookup_hit(c, N, keys, lookups), N),
        ("Lookup miss",    lambda c: bench_lookup_miss(c, N, keys, miss_keys), N),
        ("Lookup Zipf",    lambda c: bench_lookup_zipf(c, N, keys, zipf_lk), N),
        ("Update all",     lambda c: bench_update(c, N, keys), N),
        ("Insert+Delete",  lambda c: bench_delete(c, N, keys), N * 2),
        ("Mixed 80R/20W",  lambda c: bench_mixed(c, N, keys), N),
    ]

    for test_name, bench_fn, ops_count in tests:
        print(f"\n  --- {test_name} ---")
        rows = []
        for name, cls in MAPS:
            t = bench_fn(cls)
            rows.append((name, f"{fmt_ms(t)} мс", f"{fmt_ops(ops_count, t)} ops/s",
                         f"{t / ops_count * 1_000_000:.2f} мкс"))
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

def part2_scaling(sizes):
    print_header("Часть 2: Масштабируемость")
    print(f"\n  Размеры: {', '.join(f'{s:,}' for s in sizes)}")

    headers = ["N"] + [n for n, _ in MAPS]

    print(f"\n  --- Random insert (мс) ---")
    rows = []
    for n in sizes:
        k = list(range(n)); random.shuffle(k)
        row = [f"{n:,}"]
        for _, cls in MAPS:
            row.append(fmt_ms(bench_insert_rnd(cls, n, k)))
        rows.append(row)
    print_table(headers, rows)
    write_markdown_report(
        "part2_scaling.md",
        "Random insert (мс)",
        headers,
        rows
    )

    print(f"\n  --- Random lookup hit (мс) ---")
    rows = []
    for n in sizes:
        k = list(range(n)); random.shuffle(k)
        lk = [random.choice(k) for _ in range(n)]
        row = [f"{n:,}"]
        for _, cls in MAPS:
            row.append(fmt_ms(bench_lookup_hit(cls, n, k, lk)))
        rows.append(row)
    print_table(headers, rows)
    write_markdown_report(
        "part2_scaling.md",
        "Random lookup hit (мс)",
        headers,
        rows
    )

    print(f"\n  --- Insert + Delete all (мс) ---")
    rows = []
    for n in sizes:
        k = list(range(n)); random.shuffle(k)
        row = [f"{n:,}"]
        for _, cls in MAPS:
            row.append(fmt_ms(bench_delete(cls, n, k)))
        rows.append(row)
    print_table(headers, rows)
    write_markdown_report(
        "part2_scaling.md",
        "Insert + Delete all (мс)",
        headers,
        rows
    )


# ================================================================
#  Part 3: Latency percentiles
# ================================================================

def part3_latency(N):
    print_header(f"Часть 3: Перцентили латентности lookup (N={N:,})")

    keys = list(range(N)); random.shuffle(keys)
    SAMPLES = 5000
    print(f"\n  {SAMPLES} поэлементных замеров\n")

    headers = ["Структура", "avg", "p50", "p90", "p99", "p99.9"]
    rows = []

    for name, cls in MAPS:
        m = make_map(cls, N)
        for k in keys:
            m[k] = k

        sample = [random.choice(keys) for _ in range(SAMPLES)]
        lats = []
        for k in sample:
            t0 = time.perf_counter()
            _ = m[k]
            t1 = time.perf_counter()
            lats.append((t1 - t0) * 1_000_000)

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
        f"Latency percentiles (N={N:,})",
        headers,
        rows
    )


# ================================================================
#  Part 4: Memory consumption
# ================================================================

def part4_memory(sizes):
    print_header("Часть 4: Потребление памяти")
    print("""
  tracemalloc — Python-аллокации (нижняя граница для C-расширений)
  RSS delta   — реальное потребление RAM (верхняя граница)
""")

    headers = ["N", "Структура", "tracemalloc MB", "RSS Δ MB"]
    rows = []

    for n in sizes:
        keys = list(range(n)); random.shuffle(keys)
        for name, cls in MAPS:
            def fill():
                m = make_map(cls, n)
                for k in keys:
                    m[k] = k
                return m
            _, rss_d, trace_p = measure_memory(fill)
            rows.append((f"{n:,}", name, fmt_mb(trace_p), fmt_mb(rss_d)))
        rows.append(("", "", "", ""))
    print_table(headers, rows)


# ================================================================
#  Part 5: Key pattern impact
# ================================================================

def part5_patterns(N):
    print_header(f"Часть 4: Влияние паттерна ключей (N={N:,})")
    print("""
  Sequential — 0,1,2,...N      Random — перемешанные
  Zipf       — 20% ключей = 80% обращений
""")

    keys = list(range(N)); random.shuffle(keys)
    lookups = [random.choice(keys) for _ in range(N)]
    zipf_lk = [k % N for k in zipf_keys(N, N)]

    headers = ["Структура", "Seq ins", "Rnd ins", "Rnd look", "Zipf look"]
    rows = []
    for name, cls in MAPS:
        rows.append((name,
            f"{fmt_ms(bench_insert_seq(cls, N))} мс",
            f"{fmt_ms(bench_insert_rnd(cls, N, keys))} мс",
            f"{fmt_ms(bench_lookup_hit(cls, N, keys, lookups))} мс",
            f"{fmt_ms(bench_lookup_zipf(cls, N, keys, zipf_lk))} мс"))
    print_table(headers, rows)
    write_markdown_report(
        "part5_patterns.md",
        f"Key patterns impact (N={N:,})",
        headers,
        rows
    )


# ================================================================
#  Part 6: Caching scenarios (descriptive)
# ================================================================

def part6_scenarios():
    print_header("Часть 6: Сценарии кэширования")
    print("""
  ┌───────────────────────────────────────────────────────────────────┐
  │ 1. LRU-кэш (веб-сервер, page cache)                               │
  ├───────────────────────────────────────────────────────────────────┤
  │ Профиль:  ~90% lookup, ~5% insert, ~5% evict                      │
  │ Ключи:    строки (URL, session_id), Zipf                          │
  │ Критично: p99 lookup, throughput на горячих ключах                │
  │ Выбор:    SwissTable / RHMap + DList (move_to_front O(1))         │
  │ Метрики:  p99 < 5μs, throughput > 5M ops/s                        │
  └───────────────────────────────────────────────────────────────────┘
  ┌───────────────────────────────────────────────────────────────────┐
  │ 2. Write-heavy (счётчики, rate limiter)                           │
  ├───────────────────────────────────────────────────────────────────┤
  │ Профиль:  ~30% lookup, ~50% update, ~20% insert                   │
  │ Критично: throughput update, стабильность при resize              │
  │ Выбор:    RHMap (предсказуемый resize, backward-shift delete)     │
  │ Метрики:  update > 3M ops/s, resize stall < 10ms                  │
  └───────────────────────────────────────────────────────────────────┘
  ┌───────────────────────────────────────────────────────────────────┐
  │ 3. TTL-кэш (DNS, session store)                                   │
  ├───────────────────────────────────────────────────────────────────┤
  │ Профиль:  ~70% lookup, ~15% insert, ~15% delete (expiry)          │
  │ Критично: delete без деградации, эффективная purge                │
  │ Выбор:    RHMap (нет tombstone) + SkipListSet (peek_min purge)    │
  │ Метрики:  delete > 2M ops/s, lookup стабилен после 50% delete     │
  └───────────────────────────────────────────────────────────────────┘
  ┌───────────────────────────────────────────────────────────────────┐
  │ 4. Read-only lookup (конфиг, feature flags)                       │
  ├───────────────────────────────────────────────────────────────────┤
  │ Профиль:  ~100% lookup после загрузки                             │
  │ Критично: абсолютная скорость lookup, предсказуемый p99           │
  │ Выбор:    SwissTable (control byte → быстрый early-exit)          │
  │ Метрики:  p50 < 0.5μs, p99 < 2μs, > 8M ops/s                      │
  └───────────────────────────────────────────────────────────────────┘
  ┌───────────────────────────────────────────────────────────────────┐
  │ 5. Hash-flood / DoS (adversarial коллизии)                        │
  ├───────────────────────────────────────────────────────────────────┤
  │ Критично: worst-case per-op не O(n)                               │
  │ Выбор:    BTHashMap — O(log k) на коллизию (B-tree bucket)        │
  │ Метрики:  single insert < 100μs при 10к коллизий                  │
  └───────────────────────────────────────────────────────────────────┘
""")


# ================================================================
#  Part 7: Metrics collection guide
# ================================================================

def part7_guide():
    print_header("Часть 7: Сбор метрик производительности и памяти")
    print("""
  A. ВРЕМЯ ВЫПОЛНЕНИЯ
  ───────────────────
  time.perf_counter() — монотонные часы, наносекундная точность.
  Рекомендации: gc.disable() перед замером, 3+ прогона → медиана,
  warmup-прогон для прогрева CPU-кэшей.

  B. ПАМЯТЬ (Python-уровень)
  ──────────────────────────
  tracemalloc.start() / get_traced_memory() — пиковое потребление
  Python-аллокаций. Для C-расширений это нижняя граница (не видит
  прямые malloc внутри C-кода).

  C. ПАМЯТЬ (OS-уровень)
  ──────────────────────
  resource.getrusage(RUSAGE_SELF).ru_maxrss — RSS в КБ (Linux).
  Включает все аллокации. Замерять дельту до/после.

  /proc/self/status → VmRSS, VmPeak — более детально.

  psutil.Process().memory_info() — кроссплатформенно (pip install psutil).

  D. ВНЕШНИЕ ПРОФАЙЛЕРЫ
  ─────────────────────
  valgrind --tool=massif python3 script.py
    → heap profiler, самый точный для C-кода (20-50x overhead)

  valgrind --tool=memcheck --leak-check=full python3 script.py
    → поиск утечек памяти

  heaptrack python3 script.py
    → быстрая альтернатива massif (~2x overhead)

  perf stat -e cache-misses,cache-references python3 script.py
    → промахи кэша CPU (ключевая метрика: SwissTable vs HashMap)

  perf record -g python3 script.py && perf report
    → CPU profiling на уровне функций
""")


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
#  Entry point
# ================================================================

def main():
    parser = argparse.ArgumentParser(description="Juniper Hash Map Benchmark")
    parser.add_argument("--quick", action="store_true",
                        help="Быстрый прогон (N=10k)")
    parser.add_argument("--no-memory", action="store_true",
                        help="Пропустить замеры памяти")
    args = parser.parse_args()

    if args.quick:
        N = 10_000
        sizes = [1_000, 5_000, 10_000]
    else:
        N = 50_000
        sizes = [1_000, 10_000, 50_000]

    print("Juniper Hash Map Benchmark Suite")
    print(f"Python {sys.version.split()[0]}, N={N:,}, sizes={sizes}")

    for n in sizes:
        part1_operations(n)
    part2_scaling(sizes)
    for n in sizes:
        part3_latency(n)
    # if not args.no_memory:
    #     part4_memory(sizes)
    for n in sizes:
        part5_patterns(n)
    # part6_scenarios()
    # part7_guide()

    print("\n" + "=" * 78)
    print("  Benchmark complete.")
    print("=" * 78)


if __name__ == "__main__":
    main()
