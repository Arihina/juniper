# Caching Policies Benchmark Results

## Environment

- Python 3.11.0rc1
- Platform: Linux x86_64
- Capacity: 4,096
- Operations: 1,000,000
- Key space: 100,000
- Warmup: 100,000 ops
- Map implementations tested: `SwissTable`, `RHMap`, `BTHashMap`, `HashMap`, `dict`

---

## `SwissTable`

| Policy | Workload | Ops/sec | ns/op | Hit Ratio | Time |
|---|---|---:|---:|---:|---:|
| ARCCache | uniform |        392,546 | 2547.5 | 0.041 | 2.55s |
| ARCCache | zipf |        817,147 | 1223.8 | 0.901 | 1.22s |
| ARCCache | loop |      1,321,550 | 756.7 | 1.000 | 0.76s |
| ARCCache | scan |        831,233 | 1203.0 | 0.000 | 1.20s |
| FIFOCache | uniform |        606,012 | 1650.1 | 0.041 | 1.65s |
| FIFOCache | zipf |      1,388,979 | 720.0 | 0.853 | 0.72s |
| FIFOCache | loop |      3,273,305 | 305.5 | 1.000 | 0.31s |
| FIFOCache | scan |        980,632 | 1019.8 | 0.000 | 1.02s |
| LFUCache | uniform |        422,214 | 2368.5 | 0.041 | 2.37s |
| LFUCache | zipf |        539,432 | 1853.8 | 0.901 | 1.85s |
| LFUCache | loop |        841,882 | 1187.8 | 1.000 | 1.19s |
| LFUCache | scan |        652,089 | 1533.5 | 0.000 | 1.53s |
| LRUCache | uniform |        594,274 | 1682.7 | 0.041 | 1.68s |
| LRUCache | zipf |      1,179,015 | 848.2 | 0.876 | 0.85s |
| LRUCache | loop |      2,178,939 | 458.9 | 1.000 | 0.46s |
| LRUCache | scan |        957,324 | 1044.6 | 0.000 | 1.04s |
| MRUCache | uniform |        618,583 | 1616.6 | 0.041 | 1.62s |
| MRUCache | zipf |        719,020 | 1390.8 | 0.124 | 1.39s |
| MRUCache | loop |      2,026,531 | 493.5 | 1.000 | 0.49s |
| MRUCache | scan |        962,203 | 1039.3 | 0.004 | 1.04s |
| TTLCache | uniform |        393,675 | 2540.2 | 0.041 | 2.54s |
| TTLCache | zipf |        992,433 | 1007.6 | 0.854 | 1.01s |
| TTLCache | loop |      2,043,805 | 489.3 | 1.000 | 0.49s |
| TTLCache | scan |        516,496 | 1936.1 | 0.000 | 1.94s |

---

## `RHMap`

| Policy | Workload | Ops/sec | ns/op | Hit Ratio | Time |
|---|---|---:|---:|---:|---:|
| ARCCache | uniform |        445,470 | 2244.8 | 0.041 | 2.24s |
| ARCCache | zipf |        695,887 | 1437.0 | 0.902 | 1.44s |
| ARCCache | loop |      1,536,027 | 651.0 | 1.000 | 0.65s |
| ARCCache | scan |      1,040,945 | 960.7 | 0.000 | 0.96s |
| FIFOCache | uniform |        766,290 | 1305.0 | 0.041 | 1.30s |
| FIFOCache | zipf |      1,172,528 | 852.9 | 0.854 | 0.85s |
| FIFOCache | loop |      2,916,718 | 342.9 | 1.000 | 0.34s |
| FIFOCache | scan |      1,081,882 | 924.3 | 0.000 | 0.92s |
| LFUCache | uniform |        549,433 | 1820.1 | 0.041 | 1.82s |
| LFUCache | zipf |        484,398 | 2064.4 | 0.901 | 2.06s |
| LFUCache | loop |        969,591 | 1031.4 | 1.000 | 1.03s |
| LFUCache | scan |        805,145 | 1242.0 | 0.000 | 1.24s |
| LRUCache | uniform |        706,863 | 1414.7 | 0.041 | 1.41s |
| LRUCache | zipf |        862,533 | 1159.4 | 0.876 | 1.16s |
| LRUCache | loop |      2,172,944 | 460.2 | 1.000 | 0.46s |
| LRUCache | scan |      1,116,738 | 895.5 | 0.000 | 0.90s |
| MRUCache | uniform |        759,336 | 1316.9 | 0.041 | 1.32s |
| MRUCache | zipf |        877,526 | 1139.6 | 0.124 | 1.14s |
| MRUCache | loop |      2,479,801 | 403.3 | 1.000 | 0.40s |
| MRUCache | scan |        130,725 | 7649.6 | 0.004 | 7.65s |
| TTLCache | uniform |        431,155 | 2319.4 | 0.041 | 2.32s |
| TTLCache | zipf |        923,642 | 1082.7 | 0.853 | 1.08s |
| TTLCache | loop |      2,253,173 | 443.8 | 1.000 | 0.44s |
| TTLCache | scan |        543,843 | 1838.8 | 0.000 | 1.84s |

---

## `BTHashMap`

| Policy | Workload | Ops/sec | ns/op | Hit Ratio | Time |
|---|---|---:|---:|---:|---:|
| ARCCache | uniform |        312,081 | 3204.3 | 0.041 | 3.20s |
| ARCCache | zipf |        604,945 | 1653.0 | 0.901 | 1.65s |
| ARCCache | loop |      1,012,807 | 987.4 | 1.000 | 0.99s |
| ARCCache | scan |        642,586 | 1556.2 | 0.000 | 1.56s |
| FIFOCache | uniform |        467,524 | 2138.9 | 0.041 | 2.14s |
| FIFOCache | zipf |        715,823 | 1397.0 | 0.854 | 1.40s |
| FIFOCache | loop |      1,727,039 | 579.0 | 1.000 | 0.58s |
| FIFOCache | scan |        702,201 | 1424.1 | 0.000 | 1.42s |
| LFUCache | uniform |        411,452 | 2430.4 | 0.041 | 2.43s |
| LFUCache | zipf |        514,800 | 1942.5 | 0.901 | 1.94s |
| LFUCache | loop |        734,971 | 1360.6 | 1.000 | 1.36s |
| LFUCache | scan |        580,643 | 1722.2 | 0.000 | 1.72s |
| LRUCache | uniform |        550,586 | 1816.2 | 0.041 | 1.82s |
| LRUCache | zipf |      1,067,927 | 936.4 | 0.876 | 0.94s |
| LRUCache | loop |      1,773,531 | 563.8 | 1.000 | 0.56s |
| LRUCache | scan |        861,466 | 1160.8 | 0.000 | 1.16s |
| MRUCache | uniform |        594,589 | 1681.8 | 0.041 | 1.68s |
| MRUCache | zipf |        723,726 | 1381.7 | 0.124 | 1.38s |
| MRUCache | loop |      1,638,652 | 610.3 | 1.000 | 0.61s |
| MRUCache | scan |        885,400 | 1129.4 | 0.004 | 1.13s |
| TTLCache | uniform |        385,892 | 2591.4 | 0.041 | 2.59s |
| TTLCache | zipf |        965,386 | 1035.9 | 0.853 | 1.04s |
| TTLCache | loop |      1,909,702 | 523.6 | 1.000 | 0.52s |
| TTLCache | scan |        509,563 | 1962.5 | 0.000 | 1.96s |

---

## `HashMap`

| Policy | Workload | Ops/sec | ns/op | Hit Ratio | Time |
|---|---|---:|---:|---:|---:|
| ARCCache | uniform |        314,700 | 3177.6 | 0.041 | 3.18s |
| ARCCache | zipf |        701,848 | 1424.8 | 0.901 | 1.42s |
| ARCCache | loop |        784,660 | 1274.4 | 1.000 | 1.27s |
| ARCCache | scan |        594,876 | 1681.0 | 0.000 | 1.68s |
| FIFOCache | uniform |        573,257 | 1744.4 | 0.041 | 1.74s |
| FIFOCache | zipf |      1,235,670 | 809.3 | 0.853 | 0.81s |
| FIFOCache | loop |      2,503,899 | 399.4 | 1.000 | 0.40s |
| FIFOCache | scan |        813,812 | 1228.8 | 0.000 | 1.23s |
| LFUCache | uniform |        424,028 | 2358.3 | 0.041 | 2.36s |
| LFUCache | zipf |        484,207 | 2065.2 | 0.901 | 2.07s |
| LFUCache | loop |        620,645 | 1611.2 | 1.000 | 1.61s |
| LFUCache | scan |        499,369 | 2002.5 | 0.000 | 2.00s |
| LRUCache | uniform |        526,865 | 1898.0 | 0.041 | 1.90s |
| LRUCache | zipf |        815,829 | 1225.7 | 0.877 | 1.23s |
| LRUCache | loop |      1,068,848 | 935.6 | 1.000 | 0.94s |
| LRUCache | scan |        692,625 | 1443.8 | 0.000 | 1.44s |
| MRUCache | uniform |        622,395 | 1606.7 | 0.041 | 1.61s |
| MRUCache | zipf |        744,881 | 1342.5 | 0.124 | 1.34s |
| MRUCache | loop |      1,625,352 | 615.3 | 1.000 | 0.62s |
| MRUCache | scan |        888,551 | 1125.4 | 0.004 | 1.13s |
| TTLCache | uniform |        402,210 | 2486.3 | 0.041 | 2.49s |
| TTLCache | zipf |        972,878 | 1027.9 | 0.853 | 1.03s |
| TTLCache | loop |      1,900,800 | 526.1 | 1.000 | 0.53s |
| TTLCache | scan |        500,962 | 1996.2 | 0.000 | 2.00s |

---

## `dict`

| Policy | Workload | Ops/sec | ns/op | Hit Ratio | Time |
|---|---|---:|---:|---:|---:|
| ARCCache | uniform |        435,669 | 2295.3 | 0.041 | 2.30s |
| ARCCache | zipf |        881,342 | 1134.6 | 0.901 | 1.13s |
| ARCCache | loop |      1,367,429 | 731.3 | 1.000 | 0.73s |
| ARCCache | scan |      1,014,291 | 985.9 | 0.000 | 0.99s |
| FIFOCache | uniform |        785,860 | 1272.5 | 0.041 | 1.27s |
| FIFOCache | zipf |      1,693,610 | 590.5 | 0.853 | 0.59s |
| FIFOCache | loop |      4,004,696 | 249.7 | 1.000 | 0.25s |
| FIFOCache | scan |      1,202,392 | 831.7 | 0.000 | 0.83s |
| LFUCache | uniform |        551,486 | 1813.3 | 0.041 | 1.81s |
| LFUCache | zipf |        621,644 | 1608.6 | 0.901 | 1.61s |
| LFUCache | loop |        787,700 | 1269.5 | 1.000 | 1.27s |
| LFUCache | scan |        816,576 | 1224.6 | 0.000 | 1.22s |
| LRUCache | uniform |        741,340 | 1348.9 | 0.041 | 1.35s |
| LRUCache | zipf |      1,417,972 | 705.2 | 0.877 | 0.71s |
| LRUCache | loop |      2,559,093 | 390.8 | 1.000 | 0.39s |
| LRUCache | scan |      1,172,846 | 852.6 | 0.000 | 0.85s |
| MRUCache | uniform |        809,079 | 1236.0 | 0.041 | 1.24s |
| MRUCache | zipf |        948,356 | 1054.5 | 0.124 | 1.05s |
| MRUCache | loop |      2,390,040 | 418.4 | 1.000 | 0.42s |
| MRUCache | scan |      1,218,263 | 820.8 | 0.004 | 0.82s |
| TTLCache | uniform |        413,378 | 2419.1 | 0.041 | 2.42s |
| TTLCache | zipf |      1,153,089 | 867.2 | 0.854 | 0.87s |
| TTLCache | loop |      2,266,394 | 441.2 | 1.000 | 0.44s |
| TTLCache | scan |        565,992 | 1766.8 | 0.000 | 1.77s |

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
| ARCCache | uniform | `RHMap` |        445,470 | 2244.8 | 0.041 |
| ARCCache | zipf | `dict` |        881,342 | 1134.6 | 0.901 |
| ARCCache | loop | `RHMap` |      1,536,027 | 651.0 | 1.000 |
| ARCCache | scan | `RHMap` |      1,040,945 | 960.7 | 0.000 |
| FIFOCache | uniform | `dict` |        785,860 | 1272.5 | 0.041 |
| FIFOCache | zipf | `dict` |      1,693,610 | 590.5 | 0.853 |
| FIFOCache | loop | `dict` |      4,004,696 | 249.7 | 1.000 |
| FIFOCache | scan | `dict` |      1,202,392 | 831.7 | 0.000 |
| LFUCache | uniform | `dict` |        551,486 | 1813.3 | 0.041 |
| LFUCache | zipf | `dict` |        621,644 | 1608.6 | 0.901 |
| LFUCache | loop | `RHMap` |        969,591 | 1031.4 | 1.000 |
| LFUCache | scan | `dict` |        816,576 | 1224.6 | 0.000 |
| LRUCache | uniform | `dict` |        741,340 | 1348.9 | 0.041 |
| LRUCache | zipf | `dict` |      1,417,972 | 705.2 | 0.877 |
| LRUCache | loop | `dict` |      2,559,093 | 390.8 | 1.000 |
| LRUCache | scan | `dict` |      1,172,846 | 852.6 | 0.000 |
| MRUCache | uniform | `dict` |        809,079 | 1236.0 | 0.041 |
| MRUCache | zipf | `dict` |        948,356 | 1054.5 | 0.124 |
| MRUCache | loop | `RHMap` |      2,479,801 | 403.3 | 1.000 |
| MRUCache | scan | `dict` |      1,218,263 | 820.8 | 0.004 |
| TTLCache | uniform | `RHMap` |        431,155 | 2319.4 | 0.041 |
| TTLCache | zipf | `dict` |      1,153,089 | 867.2 | 0.854 |
| TTLCache | loop | `dict` |      2,266,394 | 441.2 | 1.000 |
| TTLCache | scan | `dict` |        565,992 | 1766.8 | 0.000 |
