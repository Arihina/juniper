# Caching Policies Benchmark Results

## Environment

- Python 3.11.0rc1
- Platform: Linux x86_64
- Capacity: 4,096
- Operations: 1,000,000
- Key space: 100,000
- Warmup: 100,000 ops
- Map implementations tested: `dict`, `RHMap`, `SwissTable`

---

## `dict`

| Policy | Workload | Ops/sec | ns/op | Hit Ratio | Time |
|---|---|---:|---:|---:|---:|
| ARCCache | uniform |        487,403 | 2051.7 | 0.041 | 2.05s |
| ARCCache | zipf |        943,122 | 1060.3 | 0.901 | 1.06s |
| ARCCache | loop |      1,438,313 | 695.3 | 1.000 | 0.70s |
| ARCCache | scan |        970,692 | 1030.2 | 0.000 | 1.03s |
| FIFOCache | uniform |        736,781 | 1357.3 | 0.041 | 1.36s |
| FIFOCache | zipf |      1,729,293 | 578.3 | 0.853 | 0.58s |
| FIFOCache | loop |      3,924,374 | 254.8 | 1.000 | 0.25s |
| FIFOCache | scan |      1,237,530 | 808.1 | 0.000 | 0.81s |
| LFUCache | uniform |        580,323 | 1723.2 | 0.041 | 1.72s |
| LFUCache | zipf |        644,602 | 1551.3 | 0.902 | 1.55s |
| LFUCache | loop |        921,548 | 1085.1 | 1.000 | 1.09s |
| LFUCache | scan |        854,554 | 1170.2 | 0.000 | 1.17s |
| LRUCache | uniform |        709,517 | 1409.4 | 0.041 | 1.41s |
| LRUCache | zipf |      1,247,366 | 801.7 | 0.876 | 0.80s |
| LRUCache | loop |      2,218,977 | 450.7 | 1.000 | 0.45s |
| LRUCache | scan |      1,097,580 | 911.1 | 0.000 | 0.91s |
| MRUCache | uniform |        624,690 | 1600.8 | 0.041 | 1.60s |
| MRUCache | zipf |        807,443 | 1238.5 | 0.125 | 1.24s |
| MRUCache | loop |      2,223,956 | 449.6 | 1.000 | 0.45s |
| MRUCache | scan |      1,050,339 | 952.1 | 0.004 | 0.95s |
| TTLCache | uniform |        383,218 | 2609.5 | 0.041 | 2.61s |
| TTLCache | zipf |        944,073 | 1059.2 | 0.853 | 1.06s |
| TTLCache | loop |      2,078,672 | 481.1 | 1.000 | 0.48s |
| TTLCache | scan |        465,178 | 2149.7 | 0.000 | 2.15s |

---

## `RHMap`

| Policy | Workload | Ops/sec | ns/op | Hit Ratio | Time |
|---|---|---:|---:|---:|---:|
| ARCCache | uniform |        394,529 | 2534.7 | 0.041 | 2.53s |
| ARCCache | zipf |        686,131 | 1457.4 | 0.902 | 1.46s |
| ARCCache | loop |      1,562,249 | 640.1 | 1.000 | 0.64s |
| ARCCache | scan |      1,105,677 | 904.4 | 0.000 | 0.90s |
| FIFOCache | uniform |        800,712 | 1248.9 | 0.041 | 1.25s |
| FIFOCache | zipf |      1,199,772 | 833.5 | 0.854 | 0.83s |
| FIFOCache | loop |      3,705,163 | 269.9 | 1.000 | 0.27s |
| FIFOCache | scan |      1,150,074 | 869.5 | 0.000 | 0.87s |
| LFUCache | uniform |        517,323 | 1933.0 | 0.041 | 1.93s |
| LFUCache | zipf |        424,456 | 2356.0 | 0.902 | 2.36s |
| LFUCache | loop |        832,761 | 1200.8 | 1.000 | 1.20s |
| LFUCache | scan |        777,202 | 1286.7 | 0.000 | 1.29s |
| LRUCache | uniform |        646,442 | 1546.9 | 0.041 | 1.55s |
| LRUCache | zipf |      1,090,180 | 917.3 | 0.876 | 0.92s |
| LRUCache | loop |      2,611,048 | 383.0 | 1.000 | 0.38s |
| LRUCache | scan |      1,183,224 | 845.1 | 0.000 | 0.85s |
| MRUCache | uniform |        702,138 | 1424.2 | 0.041 | 1.42s |
| MRUCache | zipf |        918,057 | 1089.3 | 0.124 | 1.09s |
| MRUCache | loop |      2,550,092 | 392.1 | 1.000 | 0.39s |
| MRUCache | scan |        140,859 | 7099.3 | 0.004 | 7.10s |
| TTLCache | uniform |        375,928 | 2660.1 | 0.041 | 2.66s |
| TTLCache | zipf |        698,858 | 1430.9 | 0.854 | 1.43s |
| TTLCache | loop |      1,659,214 | 602.7 | 1.000 | 0.60s |
| TTLCache | scan |        436,775 | 2289.5 | 0.000 | 2.29s |

---

## `SwissTable`

| Policy | Workload | Ops/sec | ns/op | Hit Ratio | Time |
|---|---|---:|---:|---:|---:|
| ARCCache | uniform |        450,032 | 2222.1 | 0.041 | 2.22s |
| ARCCache | zipf |        983,267 | 1017.0 | 0.901 | 1.02s |
| ARCCache | loop |      1,457,418 | 686.1 | 1.000 | 0.69s |
| ARCCache | scan |        996,540 | 1003.5 | 0.000 | 1.00s |
| FIFOCache | uniform |        666,994 | 1499.3 | 0.041 | 1.50s |
| FIFOCache | zipf |      1,545,160 | 647.2 | 0.853 | 0.65s |
| FIFOCache | loop |      3,582,300 | 279.2 | 1.000 | 0.28s |
| FIFOCache | scan |      1,068,628 | 935.8 | 0.000 | 0.94s |
| LFUCache | uniform |        416,004 | 2403.8 | 0.041 | 2.40s |
| LFUCache | zipf |        450,189 | 2221.3 | 0.901 | 2.22s |
| LFUCache | loop |        677,131 | 1476.8 | 1.000 | 1.48s |
| LFUCache | scan |        616,977 | 1620.8 | 0.000 | 1.62s |
| LRUCache | uniform |        627,338 | 1594.0 | 0.041 | 1.59s |
| LRUCache | zipf |      1,318,095 | 758.7 | 0.877 | 0.76s |
| LRUCache | loop |      2,202,144 | 454.1 | 1.000 | 0.45s |
| LRUCache | scan |        863,240 | 1158.4 | 0.000 | 1.16s |
| MRUCache | uniform |        513,677 | 1946.7 | 0.041 | 1.95s |
| MRUCache | zipf |        660,122 | 1514.9 | 0.123 | 1.51s |
| MRUCache | loop |      2,012,665 | 496.9 | 1.000 | 0.50s |
| MRUCache | scan |        892,947 | 1119.9 | 0.004 | 1.12s |
| TTLCache | uniform |        355,425 | 2813.5 | 0.041 | 2.81s |
| TTLCache | zipf |        875,367 | 1142.4 | 0.853 | 1.14s |
| TTLCache | loop |      1,891,105 | 528.8 | 1.000 | 0.53s |
| TTLCache | scan |        448,175 | 2231.3 | 0.000 | 2.23s |

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
| ARCCache | uniform | `dict` |        487,403 | 2051.7 | 0.041 |
| ARCCache | zipf | `SwissTable` |        983,267 | 1017.0 | 0.901 |
| ARCCache | loop | `RHMap` |      1,562,249 | 640.1 | 1.000 |
| ARCCache | scan | `RHMap` |      1,105,677 | 904.4 | 0.000 |
| FIFOCache | uniform | `RHMap` |        800,712 | 1248.9 | 0.041 |
| FIFOCache | zipf | `dict` |      1,729,293 | 578.3 | 0.853 |
| FIFOCache | loop | `dict` |      3,924,374 | 254.8 | 1.000 |
| FIFOCache | scan | `dict` |      1,237,530 | 808.1 | 0.000 |
| LFUCache | uniform | `dict` |        580,323 | 1723.2 | 0.041 |
| LFUCache | zipf | `dict` |        644,602 | 1551.3 | 0.902 |
| LFUCache | loop | `dict` |        921,548 | 1085.1 | 1.000 |
| LFUCache | scan | `dict` |        854,554 | 1170.2 | 0.000 |
| LRUCache | uniform | `dict` |        709,517 | 1409.4 | 0.041 |
| LRUCache | zipf | `SwissTable` |      1,318,095 | 758.7 | 0.877 |
| LRUCache | loop | `RHMap` |      2,611,048 | 383.0 | 1.000 |
| LRUCache | scan | `RHMap` |      1,183,224 | 845.1 | 0.000 |
| MRUCache | uniform | `RHMap` |        702,138 | 1424.2 | 0.041 |
| MRUCache | zipf | `RHMap` |        918,057 | 1089.3 | 0.124 |
| MRUCache | loop | `RHMap` |      2,550,092 | 392.1 | 1.000 |
| MRUCache | scan | `dict` |      1,050,339 | 952.1 | 0.004 |
| TTLCache | uniform | `dict` |        383,218 | 2609.5 | 0.041 |
| TTLCache | zipf | `dict` |        944,073 | 1059.2 | 0.853 |
| TTLCache | loop | `dict` |      2,078,672 | 481.1 | 1.000 |
| TTLCache | scan | `dict` |        465,178 | 2149.7 | 0.000 |
