# Caching Policies Benchmark Results

## Environment

- Python 3.11.0rc1
- Platform: Linux x86_64
- Capacity: 4,096
- Operations: 1,000,000
- Key space: 100,000
- Warmup: 100,000 ops
- Map implementations tested: `SwissTable`, `RHMap`, `BTHashMap`, `HashMap`

---

## `SwissTable`

| Policy | Workload | Ops/sec | ns/op | Hit Ratio | Time |
|---|---|---:|---:|---:|---:|
| ARCCache | uniform |        371,605 | 2691.0 | 0.041 | 2.69s |
| ARCCache | zipf |        771,214 | 1296.7 | 0.901 | 1.30s |
| ARCCache | loop |      1,279,780 | 781.4 | 1.000 | 0.78s |
| ARCCache | scan |        810,234 | 1234.2 | 0.000 | 1.23s |
| FIFOCache | uniform |        601,630 | 1662.2 | 0.041 | 1.66s |
| FIFOCache | zipf |      1,354,177 | 738.5 | 0.854 | 0.74s |
| FIFOCache | loop |      3,244,985 | 308.2 | 1.000 | 0.31s |
| FIFOCache | scan |        946,541 | 1056.5 | 0.000 | 1.06s |
| LFUCache | uniform |        429,403 | 2328.8 | 0.041 | 2.33s |
| LFUCache | zipf |        530,394 | 1885.4 | 0.901 | 1.89s |
| LFUCache | loop |        810,747 | 1233.4 | 1.000 | 1.23s |
| LFUCache | scan |        642,295 | 1556.9 | 0.000 | 1.56s |
| LRUCache | uniform |        585,663 | 1707.5 | 0.041 | 1.71s |
| LRUCache | zipf |      1,217,370 | 821.4 | 0.876 | 0.82s |
| LRUCache | loop |      2,030,918 | 492.4 | 1.000 | 0.49s |
| LRUCache | scan |        983,926 | 1016.3 | 0.000 | 1.02s |
| MRUCache | uniform |        609,365 | 1641.1 | 0.041 | 1.64s |
| MRUCache | zipf |        733,991 | 1362.4 | 0.126 | 1.36s |
| MRUCache | loop |      2,099,448 | 476.3 | 1.000 | 0.48s |
| MRUCache | scan |        980,224 | 1020.2 | 0.004 | 1.02s |
| TTLCache | uniform |        378,275 | 2643.6 | 0.041 | 2.64s |
| TTLCache | zipf |        960,859 | 1040.7 | 0.853 | 1.04s |
| TTLCache | loop |      1,943,925 | 514.4 | 1.000 | 0.51s |
| TTLCache | scan |        483,848 | 2066.8 | 0.000 | 2.07s |

---

## `RHMap`

| Policy | Workload | Ops/sec | ns/op | Hit Ratio | Time |
|---|---|---:|---:|---:|---:|
| ARCCache | uniform |        444,451 | 2250.0 | 0.041 | 2.25s |
| ARCCache | zipf |        673,320 | 1485.2 | 0.902 | 1.49s |
| ARCCache | loop |      1,502,593 | 665.5 | 1.000 | 0.67s |
| ARCCache | scan |        982,180 | 1018.1 | 0.000 | 1.02s |
| FIFOCache | uniform |        766,393 | 1304.8 | 0.041 | 1.30s |
| FIFOCache | zipf |      1,149,205 | 870.2 | 0.853 | 0.87s |
| FIFOCache | loop |      3,861,811 | 258.9 | 1.000 | 0.26s |
| FIFOCache | scan |      1,178,717 | 848.4 | 0.000 | 0.85s |
| LFUCache | uniform |        543,651 | 1839.4 | 0.041 | 1.84s |
| LFUCache | zipf |        476,888 | 2096.9 | 0.901 | 2.10s |
| LFUCache | loop |        955,761 | 1046.3 | 1.000 | 1.05s |
| LFUCache | scan |        804,795 | 1242.6 | 0.000 | 1.24s |
| LRUCache | uniform |        734,551 | 1361.4 | 0.041 | 1.36s |
| LRUCache | zipf |      1,031,763 | 969.2 | 0.876 | 0.97s |
| LRUCache | loop |      2,533,934 | 394.6 | 1.000 | 0.39s |
| LRUCache | scan |      1,162,109 | 860.5 | 0.000 | 0.86s |
| MRUCache | uniform |        755,277 | 1324.0 | 0.041 | 1.32s |
| MRUCache | zipf |        871,448 | 1147.5 | 0.126 | 1.15s |
| MRUCache | loop |      2,432,759 | 411.1 | 1.000 | 0.41s |
| MRUCache | scan |        135,496 | 7380.3 | 0.004 | 7.38s |
| TTLCache | uniform |        424,173 | 2357.5 | 0.041 | 2.36s |
| TTLCache | zipf |        909,346 | 1099.7 | 0.853 | 1.10s |
| TTLCache | loop |      2,257,424 | 443.0 | 1.000 | 0.44s |
| TTLCache | scan |        519,166 | 1926.2 | 0.000 | 1.93s |

---

## `BTHashMap`

| Policy | Workload | Ops/sec | ns/op | Hit Ratio | Time |
|---|---|---:|---:|---:|---:|
| ARCCache | uniform |        309,655 | 3229.4 | 0.041 | 3.23s |
| ARCCache | zipf |        716,307 | 1396.0 | 0.901 | 1.40s |
| ARCCache | loop |      1,119,172 | 893.5 | 1.000 | 0.89s |
| ARCCache | scan |        680,271 | 1470.0 | 0.000 | 1.47s |
| FIFOCache | uniform |        569,293 | 1756.6 | 0.041 | 1.76s |
| FIFOCache | zipf |      1,206,161 | 829.1 | 0.854 | 0.83s |
| FIFOCache | loop |      2,422,231 | 412.8 | 1.000 | 0.41s |
| FIFOCache | scan |        837,298 | 1194.3 | 0.000 | 1.19s |
| LFUCache | uniform |        405,260 | 2467.6 | 0.041 | 2.47s |
| LFUCache | zipf |        486,204 | 2056.8 | 0.901 | 2.06s |
| LFUCache | loop |        704,873 | 1418.7 | 1.000 | 1.42s |
| LFUCache | scan |        549,859 | 1818.6 | 0.000 | 1.82s |
| LRUCache | uniform |        551,293 | 1813.9 | 0.041 | 1.81s |
| LRUCache | zipf |      1,036,244 | 965.0 | 0.876 | 0.97s |
| LRUCache | loop |      1,769,548 | 565.1 | 1.000 | 0.57s |
| LRUCache | scan |        825,674 | 1211.1 | 0.000 | 1.21s |
| MRUCache | uniform |        547,948 | 1825.0 | 0.041 | 1.82s |
| MRUCache | zipf |        696,164 | 1436.4 | 0.124 | 1.44s |
| MRUCache | loop |      1,576,902 | 634.2 | 1.000 | 0.63s |
| MRUCache | scan |        861,567 | 1160.7 | 0.004 | 1.16s |
| TTLCache | uniform |        328,849 | 3040.9 | 0.041 | 3.04s |
| TTLCache | zipf |        764,294 | 1308.4 | 0.853 | 1.31s |
| TTLCache | loop |      1,688,278 | 592.3 | 1.000 | 0.59s |
| TTLCache | scan |        383,380 | 2608.4 | 0.000 | 2.61s |

---

## `HashMap`

| Policy | Workload | Ops/sec | ns/op | Hit Ratio | Time |
|---|---|---:|---:|---:|---:|
| ARCCache | uniform |        317,809 | 3146.5 | 0.041 | 3.15s |
| ARCCache | zipf |        705,573 | 1417.3 | 0.901 | 1.42s |
| ARCCache | loop |        931,544 | 1073.5 | 1.000 | 1.07s |
| ARCCache | scan |        572,875 | 1745.6 | 0.000 | 1.75s |
| FIFOCache | uniform |        566,860 | 1764.1 | 0.041 | 1.76s |
| FIFOCache | zipf |      1,208,407 | 827.5 | 0.854 | 0.83s |
| FIFOCache | loop |      2,477,701 | 403.6 | 1.000 | 0.40s |
| FIFOCache | scan |        761,980 | 1312.4 | 0.000 | 1.31s |
| LFUCache | uniform |        410,247 | 2437.6 | 0.041 | 2.44s |
| LFUCache | zipf |        468,161 | 2136.0 | 0.901 | 2.14s |
| LFUCache | loop |        563,954 | 1773.2 | 1.000 | 1.77s |
| LFUCache | scan |        496,158 | 2015.5 | 0.000 | 2.02s |
| LRUCache | uniform |        549,297 | 1820.5 | 0.041 | 1.82s |
| LRUCache | zipf |      1,007,007 | 993.0 | 0.876 | 0.99s |
| LRUCache | loop |      1,149,091 | 870.3 | 1.000 | 0.87s |
| LRUCache | scan |        724,593 | 1380.1 | 0.000 | 1.38s |
| MRUCache | uniform |        631,566 | 1583.4 | 0.041 | 1.58s |
| MRUCache | zipf |        746,433 | 1339.7 | 0.125 | 1.34s |
| MRUCache | loop |      1,488,685 | 671.7 | 1.000 | 0.67s |
| MRUCache | scan |        889,954 | 1123.7 | 0.004 | 1.12s |
| TTLCache | uniform |        353,110 | 2832.0 | 0.041 | 2.83s |
| TTLCache | zipf |        972,016 | 1028.8 | 0.853 | 1.03s |
| TTLCache | loop |      1,727,894 | 578.7 | 1.000 | 0.58s |
| TTLCache | scan |        487,411 | 2051.7 | 0.000 | 2.05s |

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
| ARCCache | uniform | `RHMap` |        444,451 | 2250.0 | 0.041 |
| ARCCache | zipf | `SwissTable` |        771,214 | 1296.7 | 0.901 |
| ARCCache | loop | `RHMap` |      1,502,593 | 665.5 | 1.000 |
| ARCCache | scan | `RHMap` |        982,180 | 1018.1 | 0.000 |
| FIFOCache | uniform | `RHMap` |        766,393 | 1304.8 | 0.041 |
| FIFOCache | zipf | `SwissTable` |      1,354,177 | 738.5 | 0.854 |
| FIFOCache | loop | `RHMap` |      3,861,811 | 258.9 | 1.000 |
| FIFOCache | scan | `RHMap` |      1,178,717 | 848.4 | 0.000 |
| LFUCache | uniform | `RHMap` |        543,651 | 1839.4 | 0.041 |
| LFUCache | zipf | `SwissTable` |        530,394 | 1885.4 | 0.901 |
| LFUCache | loop | `RHMap` |        955,761 | 1046.3 | 1.000 |
| LFUCache | scan | `RHMap` |        804,795 | 1242.6 | 0.000 |
| LRUCache | uniform | `RHMap` |        734,551 | 1361.4 | 0.041 |
| LRUCache | zipf | `SwissTable` |      1,217,370 | 821.4 | 0.876 |
| LRUCache | loop | `RHMap` |      2,533,934 | 394.6 | 1.000 |
| LRUCache | scan | `RHMap` |      1,162,109 | 860.5 | 0.000 |
| MRUCache | uniform | `RHMap` |        755,277 | 1324.0 | 0.041 |
| MRUCache | zipf | `RHMap` |        871,448 | 1147.5 | 0.126 |
| MRUCache | loop | `RHMap` |      2,432,759 | 411.1 | 1.000 |
| MRUCache | scan | `SwissTable` |        980,224 | 1020.2 | 0.004 |
| TTLCache | uniform | `RHMap` |        424,173 | 2357.5 | 0.041 |
| TTLCache | zipf | `HashMap` |        972,016 | 1028.8 | 0.853 |
| TTLCache | loop | `RHMap` |      2,257,424 | 443.0 | 1.000 |
| TTLCache | scan | `RHMap` |        519,166 | 1926.2 | 0.000 |
