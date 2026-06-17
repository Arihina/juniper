import argparse
import gc
from pathlib import Path
import random
import statistics
import sys
import time
import string
import hashlib

from juniper import HashMap, RHMap, BTHashMap, SwissTable


MAPS = [
    ("dict",       dict),
    # ("HashMap",    HashMap),
    ("RHMap",      RHMap),
    # ("BTHashMap",  BTHashMap),
    ("SwissTable", SwissTable),
]

ZIPF_ALPHA = 1.2

REPORT_DIR = Path("modified_reports")
REPORT_DIR.mkdir(exist_ok=True)


# ================================================================
#  Custom key/value classes
# ================================================================
import functools

@functools.total_ordering
class Key:
    """Кастомный ключ: оборачивает строку, реализует __hash__ и __eq__."""
    __slots__ = ("_raw", "_h")

    def __init__(self, raw: str):
        self._raw = raw
        self._h = int(hashlib.md5(raw.encode()).hexdigest(), 16) & 0xFFFF_FFFF_FFFF_FFFF

    def __hash__(self):
        return self._h

    def __eq__(self, other):
        return isinstance(other, Key) and self._raw == other._raw

    def __lt__(self, other):
        return self._raw < other._raw

    def __repr__(self):
        return f"Key({self._raw!r})"

class Value:
    """Кастомное значение: несёт полезную нагрузку (список, счётчик)."""

    __slots__ = ("label", "count", "tags")

    def __init__(self, label: str):
        self.label = label
        self.count = 0
        self.tags: list[str] = []

    def increment(self):
        self.count += 1

    def __repr__(self):
        return f"Value({self.label!r}, count={self.count})"


# ================================================================
#  Helpers
# ================================================================

_RNG_CHARS = string.ascii_letters + string.digits


def rand_str(length=12) -> str:
    return "".join(random.choices(_RNG_CHARS, k=length))


def make_str_keys(n: int, key_len: int = 12) -> list[str]:
    """Уникальные случайные строки."""
    seen: set[str] = set()
    keys: list[str] = []
    while len(keys) < n:
        s = rand_str(key_len)
        if s not in seen:
            seen.add(s)
            keys.append(s)
    return keys


def make_custom_keys(str_keys: list[str]) -> list[Key]:
    return [Key(s) for s in str_keys]


def make_map(cls, n=0):
    if cls is HashMap:
        cap = max(16, n * 2) if n else 1024
        return HashMap(cap)
    if cls is dict:
        return {}
    return cls()


def zipf_keys(n, space):
    return [int(random.paretovariate(ZIPF_ALPHA)) % space for _ in range(n)]


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
#  Table / markdown helpers
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


def markdown_table(headers, rows):
    lines = [
        "| " + " | ".join(map(str, headers)) + " |",
        "| " + " | ".join("---" for _ in headers) + " |",
    ]
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


# ================================================================
#  Generic benchmark kernels (принимают любые ключи/значения)
# ================================================================

def bench_insert(cls, n, keys, values):
    def run():
        m = make_map(cls, n)
        for k, v in zip(keys, values):
            m[k] = v
    return timed(run)


def bench_lookup_hit(cls, n, keys, values, lookups):
    m = make_map(cls, n)
    for k, v in zip(keys, values):
        m[k] = v
    def run():
        for k in lookups:
            _ = m[k]
    return timed(run)


def bench_lookup_miss(cls, n, keys, values, miss_keys):
    m = make_map(cls, n)
    for k, v in zip(keys, values):
        m[k] = v
    def run():
        for k in miss_keys:
            _ = k in m
    return timed(run)


def bench_lookup_zipf(cls, n, keys, values, zipf_lk):
    m = make_map(cls, n)
    for k, v in zip(keys, values):
        m[k] = v
    def run():
        for k in zipf_lk:
            _ = m[k]
    return timed(run)


def bench_update(cls, n, keys, values, sentinel):
    m = make_map(cls, n)
    for k, v in zip(keys, values):
        m[k] = v
    def run():
        for k in keys:
            m[k] = sentinel
    return timed(run)


def bench_delete(cls, n, keys, values):
    def run():
        m = make_map(cls, n)
        for k, v in zip(keys, values):
            m[k] = v
        for k in keys:
            del m[k]
    return timed(run)


def bench_mixed(cls, n, keys, values):
    m = make_map(cls, n)
    for k, v in zip(keys, values):
        m[k] = v
    ops = []
    for i in range(n):
        if random.random() < 0.8:
            ops.append(('r', keys[i % len(keys)]))
        else:
            ops.append(('w', keys[random.randint(0, len(keys) - 1)], values[i % len(values)]))
    def run():
        for op in ops:
            if op[0] == 'r':
                _ = op[1] in m
            else:
                m[op[1]] = op[2]
    return timed(run)


def bench_latency(cls, n, keys, values, samples=3000):
    m = make_map(cls, n)
    for k, v in zip(keys, values):
        m[k] = v
    sample_keys = [random.choice(keys) for _ in range(samples)]
    lats = []
    for k in sample_keys:
        t0 = time.perf_counter()
        _ = m[k]
        t1 = time.perf_counter()
        lats.append((t1 - t0) * 1_000_000)
    lats.sort()
    return lats


# ================================================================
#  Fixture builders
# ================================================================

class Fixtures:
    """Ленивая генерация фикстур для конкретного N и режима."""

    def __init__(self, n: int, mode: str):
        self.n = n
        self.mode = mode
        self._build(n, mode)

    def _build(self, n, mode):
        if mode == "int":
            raw_keys = list(range(n))
            random.shuffle(raw_keys)
            self.keys = raw_keys
            self.values = list(range(n))
            self.miss_keys = list(range(n, n * 2))
            random.shuffle(self.miss_keys)
            self.miss_keys = self.miss_keys[:n]
            self.lookups = [random.choice(self.keys) for _ in range(n)]
            self.zipf_lk = [self.keys[k % n] for k in zipf_keys(n, n)]
            self.sentinel = 999
        elif mode == "str":
            str_keys = make_str_keys(n, key_len=16)
            self.keys = str_keys
            self.values = [rand_str(8) for _ in range(n)]
            self.miss_keys = make_str_keys(n, key_len=20)  # другая длина — почти наверняка промах
            self.lookups = [random.choice(self.keys) for _ in range(n)]
            self.zipf_lk = [self.keys[k % n] for k in zipf_keys(n, n)]
            self.sentinel = "UPDATED"
        elif mode == "custom":
            str_keys = make_str_keys(n, key_len=16)
            custom_keys = make_custom_keys(str_keys)
            self.keys = custom_keys
            self.values = [Value(f"val_{i}") for i in range(n)]
            miss_strs = make_str_keys(n, key_len=20)
            self.miss_keys = make_custom_keys(miss_strs)
            self.lookups = [random.choice(self.keys) for _ in range(n)]
            self.zipf_lk = [self.keys[k % n] for k in zipf_keys(n, n)]
            self.sentinel = Value("sentinel")
        else:
            raise ValueError(f"Unknown mode: {mode}")


# ================================================================
#  Single-mode benchmark suite
# ================================================================

def run_suite(n: int, mode: str, md_suffix: str):
    label = {"int": "int", "str": "str", "custom": "Key/Value"}[mode]
    print_header(f"Режим: {label}  (N={n:,})")

    fx = Fixtures(n, mode)

    tests = [
        ("Insert rnd",     lambda c: bench_insert(c, n, fx.keys, fx.values),                           n),
        ("Lookup hit",     lambda c: bench_lookup_hit(c, n, fx.keys, fx.values, fx.lookups),           n),
        ("Lookup miss",    lambda c: bench_lookup_miss(c, n, fx.keys, fx.values, fx.miss_keys),        n),
        ("Lookup Zipf",    lambda c: bench_lookup_zipf(c, n, fx.keys, fx.values, fx.zipf_lk),         n),
        ("Update all",     lambda c: bench_update(c, n, fx.keys, fx.values, fx.sentinel),              n),
        ("Insert+Delete",  lambda c: bench_delete(c, n, fx.keys, fx.values),                           n * 2),
        ("Mixed 80R/20W",  lambda c: bench_mixed(c, n, fx.keys, fx.values),                           n),
    ]

    for test_name, bench_fn, ops_count in tests:
        print(f"\n  --- {test_name} ---")
        rows = []
        for name, cls in MAPS:
            t = bench_fn(cls)
            rows.append((name, f"{fmt_ms(t)} мс", f"{fmt_ops(ops_count, t)} ops/s",
                         f"{t / ops_count * 1_000_000:.2f} мкс"))
        print_table(["Структура", "Время", "Throughput", "мкс/op"], rows)
        write_markdown_report(
            f"bench_{md_suffix}_N{n}.md",
            f"{test_name} ({label}, N={n:,})",
            ["Структура", "Время", "Throughput", "мкс/op"],
            rows,
        )

    # Латентность
    print(f"\n  --- Latency percentiles (lookup hit) ---")
    lat_rows = []
    headers = ["Структура", "avg", "p50", "p90", "p99", "p99.9"]
    for name, cls in MAPS:
        lats = bench_latency(cls, n, fx.keys, fx.values)
        lat_rows.append((
            name,
            f"{statistics.mean(lats):.2f}μs",
            f"{lats[len(lats)//2]:.2f}μs",
            f"{lats[int(len(lats)*0.90)]:.2f}μs",
            f"{lats[int(len(lats)*0.99)]:.2f}μs",
            f"{lats[min(int(len(lats)*0.999), len(lats)-1)]:.2f}μs",
        ))
    print_table(headers, lat_rows)
    write_markdown_report(
        f"bench_{md_suffix}_N{n}.md",
        f"Latency percentiles ({label}, N={n:,})",
        headers,
        lat_rows,
    )


# ================================================================
#  Scaling across sizes (для одного режима)
# ================================================================

def run_scaling(sizes: list[int], mode: str, md_suffix: str):
    label = {"int": "int", "str": "str", "custom": "Key/Value"}[mode]
    print_header(f"Масштабируемость — {label}  (sizes={sizes})")

    headers = ["N"] + [n for n, _ in MAPS]

    print(f"\n  --- Insert (мс) ---")
    rows = []
    for n in sizes:
        fx = Fixtures(n, mode)
        row = [f"{n:,}"]
        for _, cls in MAPS:
            row.append(fmt_ms(bench_insert(cls, n, fx.keys, fx.values)))
        rows.append(row)
    print_table(headers, rows)
    write_markdown_report(f"bench_{md_suffix}_scaling.md", f"Insert ({label}, мс)", headers, rows)

    print(f"\n  --- Lookup hit (мс) ---")
    rows = []
    for n in sizes:
        fx = Fixtures(n, mode)
        row = [f"{n:,}"]
        for _, cls in MAPS:
            row.append(fmt_ms(bench_lookup_hit(cls, n, fx.keys, fx.values, fx.lookups)))
        rows.append(row)
    print_table(headers, rows)
    write_markdown_report(f"bench_{md_suffix}_scaling.md", f"Lookup hit ({label}, мс)", headers, rows)

    print(f"\n  --- Insert+Delete (мс) ---")
    rows = []
    for n in sizes:
        fx = Fixtures(n, mode)
        row = [f"{n:,}"]
        for _, cls in MAPS:
            row.append(fmt_ms(bench_delete(cls, n, fx.keys, fx.values)))
        rows.append(row)
    print_table(headers, rows)
    write_markdown_report(f"bench_{md_suffix}_scaling.md", f"Insert+Delete ({label}, мс)", headers, rows)


# ================================================================
#  Сравнительная таблица: int vs str vs custom (одна операция)
# ================================================================

def run_comparison(n: int):
    print_header(f"Сравнение режимов: int / str / custom  (N={n:,}, Lookup hit)")

    modes = [("int", "int"), ("str", "str"), ("custom", "Key/Value")]
    headers = ["Структура"] + [label for _, label in modes]
    rows = []

    fixtures = {mode: Fixtures(n, mode) for mode, _ in modes}

    for name, cls in MAPS:
        row = [name]
        for mode, _ in modes:
            fx = fixtures[mode]
            t = bench_lookup_hit(cls, n, fx.keys, fx.values, fx.lookups)
            row.append(f"{fmt_ms(t)} мс")
        rows.append(row)

    print_table(headers, rows)
    write_markdown_report("bench_comparison.md", f"Lookup hit: int vs str vs Key/Value (N={n:,})", headers, rows)


# ================================================================
#  Entry point
# ================================================================

def main():
    parser = argparse.ArgumentParser(description="Juniper Extended Benchmark")
    parser.add_argument("--quick", action="store_true", help="Быстрый прогон (N=5k)")
    parser.add_argument(
        "--modes", nargs="+", choices=["int", "str", "custom"], default=["int", "str", "custom"],
        help="Какие режимы ключей прогнать",
    )
    args = parser.parse_args()

    if args.quick:
        N = 5_000
        sizes = [1_000, 2_500, 5_000]
    else:
        N = 500_000
        sizes = [500_000, 1_000_000, 2_000_000]

    print("Juniper Extended Benchmark")
    print(f"Python {sys.version.split()[0]}, N={N:,}, sizes={sizes}")
    print(f"Режимы: {args.modes}")

    # Подробные бенчмарки по каждому режиму
    mode_suffix = {"int": "int", "str": "str", "custom": "custom"}
    for mode in args.modes:
        for n in sizes:
            run_suite(n, mode, mode_suffix[mode])
        run_scaling(sizes, mode, mode_suffix[mode])

    # Сравнительная таблица
    if len(args.modes) > 1:
        run_comparison(N)

    print("\n" + "=" * 78)
    print("  Benchmark complete. Reports saved to ./modified_reports/")
    print("=" * 78)


if __name__ == "__main__":
    main()
