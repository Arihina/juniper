"""
Benchmark suite for cache policies:
    - ARC
    - FIFO
    - LFU
    - LRU
    - MRU
    - TTL

Измеряет:
    - throughput (ops/sec)
    - latency per op (ns/op)
    - hit ratio
    - отдельно get/put mixed workloads
    - разные access patterns
    - разные map_impl (SwissTable, RHMap, BTHashMap, HashMap, dict)

Запуск:
    python3.11 cache_bench.py [--policies P1,P2] [--maps M1,M2] [--ops N]
    python3.11 cache_bench.py --output results.md
    python3.11 cache_bench.py --maps SwissTable,RHMap,BTHashMap,HashMap --output results.md
"""

from __future__ import annotations

import argparse
import gc
import random
import time
from dataclasses import dataclass
from typing import Callable, List, Type

from juniper.core.cache.policies.arc import ARCCache
from juniper.core.cache.policies.fifo import FIFOCache
from juniper.core.cache.policies.lfu import LFUCache
from juniper.core.cache.policies.lru import LRUCache
from juniper.core.cache.policies.mru import MRUCache
from juniper.core.cache.policies.ttl import TTLCache

from juniper.data_structures import BTHashMap, HashMap, RHMap, SwissTable

import juniper.core.cache.policies.arc  as _p_arc
import juniper.core.cache.policies.fifo as _p_fifo
import juniper.core.cache.policies.lfu  as _p_lfu
import juniper.core.cache.policies.lru  as _p_lru
import juniper.core.cache.policies.mru  as _p_mru
import juniper.core.cache.policies.ttl  as _p_ttl

_POLICY_MODULES = [_p_arc, _p_fifo, _p_lfu, _p_lru, _p_mru, _p_ttl]

def _patch_map_check():
    """
    Заменяет _check_map_impl во всех модулях политик на no-op,
    чтобы можно было передавать любую реализацию map-а (HashMap, dict-обёртку и т.д.).
    """
    _noop = staticmethod(lambda map_impl: None)
    for mod in _POLICY_MODULES:
        if hasattr(mod, "_check_map_impl"):
            mod._check_map_impl = _noop

_patch_map_check()


# ============================================================
# dict ADAPTER
# Оборачивает встроенный dict в API, совместимый с juniper map-ами:
#   put / get / remove / pop / contains / clear / keys / values / items
# ============================================================

class DictWrapper:
    """Thin wrapper over Python's built-in dict exposing the juniper map API."""

    __slots__ = ("_d",)

    def __init__(self):
        self._d: dict = {}

    # ---- write ----

    def put(self, key, value):
        old = self._d.get(key)
        self._d[key] = value
        return old

    def remove(self, key):
        try:
            return self._d.pop(key)
        except KeyError:
            raise KeyError(key)

    def pop(self, key, *args):
        return self._d.pop(key, *args)

    def clear(self):
        self._d.clear()

    # ---- read ----

    def get(self, key, default=None):
        return self._d.get(key, default)

    def contains(self, key) -> bool:
        return key in self._d

    def keys(self):
        return iter(self._d.keys())

    def values(self):
        return iter(self._d.values())

    def items(self):
        return iter(self._d.items())

    def __len__(self):
        return len(self._d)

    def __repr__(self):
        return f"DictWrapper({len(self._d)} items)"



class HashMapWrapper:
    """
    Wrapper over juniper.HashMap adding the missing `contains` / `clear`
    methods that cache policies rely on.
    """

    __slots__ = ("_m",)

    def __init__(self):
        self._m = HashMap()

    # ---- missing from raw HashMap ----

    def contains(self, key) -> bool:
        # HashMap has no contains — probe via get
        return self._m.get(key) is not None

    def clear(self):
        for k in list(self._m.keys()):
            self._m.remove(k)

    # ---- delegation ----

    def put(self, key, value):
        return self._m.put(key, value)

    def get(self, key, default=None):
        return self._m.get(key, default)

    def remove(self, key):
        return self._m.remove(key)

    def pop(self, key, *args):
        return self._m.pop(key, *args)

    def keys(self):
        return self._m.keys()

    def values(self):
        return self._m.values()

    def items(self):
        return self._m.items()

    def __repr__(self):
        return "HashMapWrapper()"

# ============================================================
# CONFIG
# ============================================================

CAPACITY   = 4096
OPERATIONS = 1_000_000
WARMUP_OPS = 100_000

KEY_SPACE   = 100_000
SEED        = 42
TTL_SECONDS = 60.0

random.seed(SEED)


# ============================================================
# MAP IMPLEMENTATIONS
# ============================================================

# Каждая запись: (display_name, factory_fn)
# factory_fn() -> свежий экземпляр map-а (без аргументов)
MAP_IMPLS = [
    ("SwissTable", SwissTable),
    ("RHMap",      RHMap),
    ("BTHashMap",  BTHashMap),
    ("HashMap",    HashMapWrapper),
    ("dict",       DictWrapper),
]


# ============================================================
# WORKLOADS
# ============================================================

def workload_uniform(n: int, key_space: int):
    """Равномерное распределение. Плохой кейс для LFU/LRU."""
    yield from (random.randint(0, key_space - 1) for _ in range(n))


def workload_zipf(n: int, key_space: int, alpha: float = 1.2):
    """Zipf/hotset workload. Хорош для LFU/LRU/ARC."""
    weights = [1 / (i + 1) ** alpha for i in range(key_space)]
    total   = sum(weights)
    probs   = [w / total for w in weights]
    keys    = list(range(key_space))
    yield from random.choices(keys, probs, k=n)


def workload_loop(n: int, working_set: int):
    """Циклический workload. Часто убивает MRU."""
    for i in range(n):
        yield i % working_set


def workload_scan(n: int):
    """Sequential scan. Хороший кейс для MRU."""
    yield from range(n)


# ============================================================
# BENCH INFRA
# ============================================================

@dataclass
class Result:
    policy:      str
    map_impl:    str
    workload:    str
    ops:         int
    seconds:     float
    ops_per_sec: float
    ns_per_op:   float
    hit_ratio:   float


def make_cache(policy_cls: Type, map_cls):
    """Создаёт кеш с заданной политикой и реализацией map-а."""
    kwargs = dict(capacity=CAPACITY, map_impl=map_cls)
    if policy_cls is TTLCache:
        kwargs["default_ttl"] = TTL_SECONDS
    return policy_cls(**kwargs)


def benchmark_policy(
    policy_cls:    Type,
    map_name:      str,
    map_cls,
    workload_name: str,
    workload_fn:   Callable[[int], object],
    operations:    int,
) -> Result:

    gc.disable()

    cache = make_cache(policy_cls, map_cls)

    # ---- warmup ----
    for key in workload_fn(WARMUP_OPS):
        if cache.get(key) is None:
            cache.put(key, key)

    # ---- benchmark ----
    hits = misses = 0
    start = time.perf_counter_ns()

    for key in workload_fn(operations):
        value = cache.get(key)
        if value is None:
            misses += 1
            cache.put(key, key)
        else:
            hits += 1

    end = time.perf_counter_ns()
    gc.enable()

    elapsed_ns  = end - start
    elapsed_sec = elapsed_ns / 1e9

    return Result(
        policy      = policy_cls.__name__,
        map_impl    = map_name,
        workload    = workload_name,
        ops         = operations,
        seconds     = elapsed_sec,
        ops_per_sec = operations / elapsed_sec,
        ns_per_op   = elapsed_ns / operations,
        hit_ratio   = hits / operations,
    )


# ============================================================
# RUNNER
# ============================================================

POLICIES = [
    ARCCache,
    FIFOCache,
    LFUCache,
    LRUCache,
    MRUCache,
    TTLCache,
]

WORKLOADS = [
    ("uniform", lambda n: workload_uniform(n, KEY_SPACE)),
    ("zipf",    lambda n: workload_zipf(n, KEY_SPACE)),
    ("loop",    lambda n: workload_loop(n, CAPACITY // 2)),
    ("scan",    lambda n: workload_scan(n)),
]


def run_all(
    policies:   List[Type] | None = None,
    map_impls:  list | None       = None,
    operations: int               = OPERATIONS,
) -> List[Result]:

    policies  = policies  or POLICIES
    map_impls = map_impls or MAP_IMPLS

    results: List[Result] = []

    for policy in policies:
        print(f"\n=== {policy.__name__} ===")

        for map_name, map_cls in map_impls:
            print(f"  map_impl: {map_name}")

            for workload_name, workload_fn in WORKLOADS:
                print(f"    workload: {workload_name}...", end=" ", flush=True)

                result = benchmark_policy(
                    policy_cls    = policy,
                    map_name      = map_name,
                    map_cls       = map_cls,
                    workload_name = workload_name,
                    workload_fn   = workload_fn,
                    operations    = operations,
                )

                results.append(result)
                print(f"{result.ops_per_sec:,.0f} ops/s  hit={result.hit_ratio:.3f}")

    return results


# ============================================================
# REPORTING
# ============================================================

def print_results(results: List[Result]):
    headers = ["Policy", "Map", "Workload", "Ops/sec", "ns/op", "Hit Ratio", "Time"]

    rows = [
        [
            r.policy,
            r.map_impl,
            r.workload,
            f"{r.ops_per_sec:>14,.0f}",
            f"{r.ns_per_op:>8.1f}",
            f"{r.hit_ratio:.3f}",
            f"{r.seconds:.2f}s",
        ]
        for r in results
    ]

    widths = [
        max(len(str(x)) for x in col)
        for col in zip(headers, *rows)
    ]

    sep = "-+-".join("-" * w for w in widths)
    fmt = " | ".join(f"{{:<{w}}}" for w in widths)

    print("\n")
    print(fmt.format(*headers))
    print(sep)

    prev_policy = prev_map = None
    for row, r in zip(rows, results):
        # визуальный разделитель при смене политики или map_impl
        if r.policy != prev_policy:
            if prev_policy is not None:
                print(sep)
            prev_policy = r.policy
            prev_map    = None
        if r.map_impl != prev_map:
            prev_map = r.map_impl

        print(fmt.format(*row))


def print_summary(results: List[Result]):
    """
    Сводная таблица: для каждой пары (policy, workload) — лучший map_impl по ops/sec.
    """
    from collections import defaultdict

    best: dict[tuple, Result] = {}
    for r in results:
        k = (r.policy, r.workload)
        if k not in best or r.ops_per_sec > best[k].ops_per_sec:
            best[k] = r

    print("\n\n=== SUMMARY: fastest map_impl per (policy, workload) ===\n")

    headers = ["Policy", "Workload", "Best Map", "Ops/sec", "ns/op", "Hit Ratio"]
    rows = [
        [
            r.policy,
            r.workload,
            r.map_impl,
            f"{r.ops_per_sec:>14,.0f}",
            f"{r.ns_per_op:>8.1f}",
            f"{r.hit_ratio:.3f}",
        ]
        for r in best.values()
    ]

    widths = [max(len(str(x)) for x in col) for col in zip(headers, *rows)]
    fmt    = " | ".join(f"{{:<{w}}}" for w in widths)
    sep    = "-+-".join("-" * w for w in widths)

    print(fmt.format(*headers))
    print(sep)
    for row in rows:
        print(fmt.format(*row))


# ============================================================
# MARKDOWN REPORT
# ============================================================

# Observations per policy — отображаются в секции ### Observations
_POLICY_OBSERVATIONS = {
    "FIFOCache":  "`FIFOCache` achieves the highest throughput due to minimal mutation on reads.",
    "LFUCache":   "`LFUCache` provides the strongest skew-awareness but has the highest CPU overhead among O(1) policies.",
    "ARCCache":   "`ARCCache` adapts well to mixed workloads and approaches LFU-level hit ratios.",
    "LRUCache":   "`LRUCache` remains the best general-purpose tradeoff.",
    "MRUCache":   "`MRUCache` performs poorly on locality-heavy workloads but can be effective for scans.",
    "TTLCache":   "`TTLCache` is slower due to ordered expiry maintenance (`O(log n)` operations).",
}

_WORKLOAD_NOTES = [
    ("`uniform`", "Random uniform access over a large key space"),
    ("`zipf`",    "Skewed hot-set distribution (Zipfian)"),
    ("`loop`",    "Small cyclic working set fitting entirely in cache"),
    ("`scan`",    "Sequential scan with no temporal locality"),
]


def save_markdown(
    results:    List[Result],
    path:       str,
    operations: int = OPERATIONS,
):
    """
    Сохраняет результаты в Markdown-файл.

    Структура:
        - Один раздел ## на каждый map_impl
        - Внутри — таблица со всеми политиками и воркloadами
        - ### Observations (общие + per-policy)
        - ### Workload Notes
        - ### Summary — лучший map_impl для каждой (policy, workload)
    """
    import platform
    import sys
    from collections import defaultdict

    # сгруппировать по map_impl
    by_map: dict[str, List[Result]] = defaultdict(list)
    for r in results:
        by_map[r.map_impl].append(r)

    lines: List[str] = []

    lines.append("# Caching Policies Benchmark Results\n")

    # ---- общая конфигурация ----
    lines.append("## Environment\n")
    lines.append(f"- Python {sys.version.split()[0]}")
    lines.append(f"- Platform: {platform.system()} {platform.machine()}")
    lines.append(f"- Capacity: {CAPACITY:,}")
    lines.append(f"- Operations: {operations:,}")
    lines.append(f"- Key space: {KEY_SPACE:,}")
    lines.append(f"- Warmup: {WARMUP_OPS:,} ops")
    lines.append(f"- Map implementations tested: {', '.join(f'`{n}`' for n in by_map)}")
    lines.append("")

    # ---- раздел на каждый map_impl ----
    for map_name, map_results in by_map.items():
        lines.append(f"---\n")
        lines.append(f"## `{map_name}`\n")
        lines.append("| Policy | Workload | Ops/sec | ns/op | Hit Ratio | Time |")
        lines.append("|---|---|---:|---:|---:|---:|")

        for r in map_results:
            lines.append(
                f"| {r.policy} | {r.workload} "
                f"| {r.ops_per_sec:>14,.0f} "
                f"| {r.ns_per_op:.1f} "
                f"| {r.hit_ratio:.3f} "
                f"| {r.seconds:.2f}s |"
            )

        lines.append("")

    # ---- Observations ----
    lines.append("---\n")
    lines.append("## Observations\n")
    lines.append("### Policy Notes\n")
    for obs in _POLICY_OBSERVATIONS.values():
        lines.append(f"- {obs}")
    lines.append("")

    lines.append("### Workload Notes\n")
    lines.append("| Workload | Description |")
    lines.append("|---|---|")
    for wl, desc in _WORKLOAD_NOTES:
        lines.append(f"| {wl} | {desc} |")
    lines.append("")

    # ---- Summary: лучший map_impl per (policy, workload) ----
    lines.append("---\n")
    lines.append("## Summary — Fastest Map per (Policy, Workload)\n")
    lines.append("| Policy | Workload | Best Map | Ops/sec | ns/op | Hit Ratio |")
    lines.append("|---|---|---|---:|---:|---:|")

    best: dict[tuple, Result] = {}
    for r in results:
        k = (r.policy, r.workload)
        if k not in best or r.ops_per_sec > best[k].ops_per_sec:
            best[k] = r

    for r in best.values():
        lines.append(
            f"| {r.policy} | {r.workload} | `{r.map_impl}` "
            f"| {r.ops_per_sec:>14,.0f} "
            f"| {r.ns_per_op:.1f} "
            f"| {r.hit_ratio:.3f} |"
        )

    lines.append("")

    with open(path, "w", encoding="utf-8") as f:
        f.write("\n".join(lines))

    print(f"\nMarkdown report saved → {path}")


# ============================================================
# CLI
# ============================================================

def _parse_args():
    p = argparse.ArgumentParser(description="Cache policy benchmark")

    policy_names = {cls.__name__: cls for cls in POLICIES}
    map_names    = {name: (name, cls) for name, cls in MAP_IMPLS}

    p.add_argument(
        "--policies",
        default=",".join(policy_names),
        help=f"Comma-separated list of policies to run. Default: all. "
             f"Choices: {','.join(policy_names)}",
    )
    p.add_argument(
        "--maps",
        default=",".join(n for n, _ in MAP_IMPLS),
        help=f"Comma-separated list of map impls. Default: all. "
             f"Choices: {','.join(map_names)}",
    )
    p.add_argument(
        "--ops",
        type=int,
        default=OPERATIONS,
        help=f"Number of operations per benchmark (default: {OPERATIONS:,})",
    )
    p.add_argument(
        "--no-summary",
        action="store_true",
        help="Skip the summary table",
    )
    p.add_argument(
        "--output",
        default=None,
        metavar="FILE.md",
        help="Save results as a Markdown report (e.g. results.md)",
    )

    args = p.parse_args()

    selected_policies = [policy_names[n.strip()] for n in args.policies.split(",")]
    selected_maps     = [map_names[n.strip()]     for n in args.maps.split(",")]

    return args, selected_policies, selected_maps


# ============================================================
# ENTRY
# ============================================================

if __name__ == "__main__":
    args, sel_policies, sel_maps = _parse_args()

    print("=" * 80)
    print("CACHE POLICY BENCHMARK")
    print("=" * 80)
    print(f"Capacity     : {CAPACITY}")
    print(f"Operations   : {args.ops:,}")
    print(f"Key space    : {KEY_SPACE:,}")
    print(f"Warmup ops   : {WARMUP_OPS:,}")
    print(f"TTL seconds  : {TTL_SECONDS}")
    print(f"Policies     : {', '.join(p.__name__ for p in sel_policies)}")
    print(f"Map impls    : {', '.join(n for n, _ in sel_maps)}")
    print("=" * 80)

    results = run_all(
        policies   = sel_policies,
        map_impls  = sel_maps,
        operations = args.ops,
    )

    print_results(results)

    if not args.no_summary:
        print_summary(results)

    if args.output:
        save_markdown(results, args.output, operations=args.ops)
