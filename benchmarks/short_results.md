# Caching Policies Benchmark Results

## Environment

- Python 3.11.0rc1
- Platform: Linux x86_64
- Capacity: 4,096
- Operations: 1,000,000
- Key space: 100,000
- Warmup: 100,000 ops
- Map implementations tested: `RHMap`, `SwissTable`

---

## `RHMap`

| Policy | Workload | Ops/sec | ns/op | Hit Ratio | Time |
|---|---|---:|---:|---:|---:|
| ARCCache | uniform |        436,995 | 2288.4 | 0.041 | 2.29s |
| ARCCache | zipf |        538,324 | 1857.6 | 0.901 | 1.86s |
| ARCCache | loop |      1,162,906 | 859.9 | 1.000 | 0.86s |
| ARCCache | scan |        776,974 | 1287.0 | 0.000 | 1.29s |
| FIFOCache | uniform |        657,208 | 1521.6 | 0.041 | 1.52s |
| FIFOCache | zipf |      1,098,176 | 910.6 | 0.853 | 0.91s |
| FIFOCache | loop |      3,458,455 | 289.1 | 1.000 | 0.29s |
| FIFOCache | scan |      1,098,772 | 910.1 | 0.000 | 0.91s |
| LFUCache | uniform |        555,871 | 1799.0 | 0.041 | 1.80s |
| LFUCache | zipf |        505,108 | 1979.8 | 0.901 | 1.98s |
| LFUCache | loop |        991,889 | 1008.2 | 1.000 | 1.01s |
| LFUCache | scan |        830,396 | 1204.2 | 0.000 | 1.20s |
| LRUCache | uniform |        762,735 | 1311.1 | 0.041 | 1.31s |
| LRUCache | zipf |      1,051,622 | 950.9 | 0.877 | 0.95s |
| LRUCache | loop |      2,532,015 | 394.9 | 1.000 | 0.39s |
| LRUCache | scan |      1,205,456 | 829.6 | 0.000 | 0.83s |
| MRUCache | uniform |        788,399 | 1268.4 | 0.041 | 1.27s |
| MRUCache | zipf |        943,622 | 1059.7 | 0.123 | 1.06s |
| MRUCache | loop |      2,490,532 | 401.5 | 1.000 | 0.40s |
| MRUCache | scan |        138,082 | 7242.1 | 0.004 | 7.24s |
| TTLCache | uniform |        449,114 | 2226.6 | 0.041 | 2.23s |
| TTLCache | zipf |        924,059 | 1082.2 | 0.853 | 1.08s |
| TTLCache | loop |      2,282,191 | 438.2 | 1.000 | 0.44s |
| TTLCache | scan |        567,598 | 1761.8 | 0.000 | 1.76s |

---

## `SwissTable`

| Policy | Workload | Ops/sec | ns/op | Hit Ratio | Time |
|---|---|---:|---:|---:|---:|
| ARCCache | uniform |        314,097 | 3183.7 | 0.041 | 3.18s |
| ARCCache | zipf |        769,916 | 1298.8 | 0.902 | 1.30s |
| ARCCache | loop |      1,237,875 | 807.8 | 1.000 | 0.81s |
| ARCCache | scan |        797,707 | 1253.6 | 0.000 | 1.25s |
| FIFOCache | uniform |        590,695 | 1692.9 | 0.041 | 1.69s |
| FIFOCache | zipf |      1,218,558 | 820.6 | 0.853 | 0.82s |
| FIFOCache | loop |      3,107,796 | 321.8 | 1.000 | 0.32s |
| FIFOCache | scan |      1,059,809 | 943.6 | 0.000 | 0.94s |
| LFUCache | uniform |        471,334 | 2121.6 | 0.041 | 2.12s |
| LFUCache | zipf |        606,468 | 1648.9 | 0.901 | 1.65s |
| LFUCache | loop |        912,363 | 1096.1 | 1.000 | 1.10s |
| LFUCache | scan |        701,335 | 1425.9 | 0.000 | 1.43s |
| LRUCache | uniform |        640,818 | 1560.5 | 0.041 | 1.56s |
| LRUCache | zipf |      1,299,956 | 769.3 | 0.877 | 0.77s |
| LRUCache | loop |      2,322,227 | 430.6 | 1.000 | 0.43s |
| LRUCache | scan |      1,065,408 | 938.6 | 0.000 | 0.94s |
| MRUCache | uniform |        654,995 | 1526.7 | 0.041 | 1.53s |
| MRUCache | zipf |        791,840 | 1262.9 | 0.126 | 1.26s |
| MRUCache | loop |      2,261,588 | 442.2 | 1.000 | 0.44s |
| MRUCache | scan |      1,018,474 | 981.9 | 0.004 | 0.98s |
| TTLCache | uniform |        420,433 | 2378.5 | 0.041 | 2.38s |
| TTLCache | zipf |      1,038,102 | 963.3 | 0.854 | 0.96s |
| TTLCache | loop |      2,121,909 | 471.3 | 1.000 | 0.47s |
| TTLCache | scan |        538,150 | 1858.2 | 0.000 | 1.86s |

---

## Observations

### Policy Notes

- `FIFOCache` achieves the highest throughput due to minimal mutation on reads.
- `LFUCache` provides the strongest skew-awareness but has the highest CPU overhead among O(1) policies.
- `ARCCache` adapts well to mixed workloads and approaches LFU-level hit ratios.
- `LRUCache` remains the best general-purpose tradeoff.
- `MRUCache` performs poorly on locality-heavy workloads but can be effective for scans.
- `TTLCache` is slower due to ordered expiry maintenance (`O(log n)` operations).

### Workload Notes

| Workload | Description |
|---|---|
| `uniform` | Random uniform access over a large key space |
| `zipf` | Skewed hot-set distribution (Zipfian) |
| `loop` | Small cyclic working set fitting entirely in cache |
| `scan` | Sequential scan with no temporal locality |

---

## Summary — Fastest Map per (Policy, Workload)

| Policy | Workload | Best Map | Ops/sec | ns/op | Hit Ratio |
|---|---|---|---:|---:|---:|
| ARCCache | uniform | `RHMap` |        436,995 | 2288.4 | 0.041 |
| ARCCache | zipf | `SwissTable` |        769,916 | 1298.8 | 0.902 |
| ARCCache | loop | `SwissTable` |      1,237,875 | 807.8 | 1.000 |
| ARCCache | scan | `SwissTable` |        797,707 | 1253.6 | 0.000 |
| FIFOCache | uniform | `RHMap` |        657,208 | 1521.6 | 0.041 |
| FIFOCache | zipf | `SwissTable` |      1,218,558 | 820.6 | 0.853 |
| FIFOCache | loop | `RHMap` |      3,458,455 | 289.1 | 1.000 |
| FIFOCache | scan | `RHMap` |      1,098,772 | 910.1 | 0.000 |
| LFUCache | uniform | `RHMap` |        555,871 | 1799.0 | 0.041 |
| LFUCache | zipf | `SwissTable` |        606,468 | 1648.9 | 0.901 |
| LFUCache | loop | `RHMap` |        991,889 | 1008.2 | 1.000 |
| LFUCache | scan | `RHMap` |        830,396 | 1204.2 | 0.000 |
| LRUCache | uniform | `RHMap` |        762,735 | 1311.1 | 0.041 |
| LRUCache | zipf | `SwissTable` |      1,299,956 | 769.3 | 0.877 |
| LRUCache | loop | `RHMap` |      2,532,015 | 394.9 | 1.000 |
| LRUCache | scan | `RHMap` |      1,205,456 | 829.6 | 0.000 |
| MRUCache | uniform | `RHMap` |        788,399 | 1268.4 | 0.041 |
| MRUCache | zipf | `RHMap` |        943,622 | 1059.7 | 0.123 |
| MRUCache | loop | `RHMap` |      2,490,532 | 401.5 | 1.000 |
| MRUCache | scan | `SwissTable` |      1,018,474 | 981.9 | 0.004 |
| TTLCache | uniform | `RHMap` |        449,114 | 2226.6 | 0.041 |
| TTLCache | zipf | `SwissTable` |      1,038,102 | 963.3 | 0.854 |
| TTLCache | loop | `RHMap` |      2,282,191 | 438.2 | 1.000 |
| TTLCache | scan | `RHMap` |        567,598 | 1761.8 | 0.000 |
