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
| ARCCache | uniform |        379,544 | 2634.7 | 0.041 | 2.63s |
| ARCCache | zipf |        859,194 | 1163.9 | 0.901 | 1.16s |
| ARCCache | loop |      1,328,617 | 752.7 | 1.000 | 0.75s |
| ARCCache | scan |        877,549 | 1139.5 | 0.000 | 1.14s |
| FIFOCache | uniform |        613,004 | 1631.3 | 0.041 | 1.63s |
| FIFOCache | zipf |      1,501,949 | 665.8 | 0.854 | 0.67s |
| FIFOCache | loop |      3,501,179 | 285.6 | 1.000 | 0.29s |
| FIFOCache | scan |      1,031,636 | 969.3 | 0.000 | 0.97s |
| LFUCache | uniform |        450,384 | 2220.3 | 0.041 | 2.22s |
| LFUCache | zipf |        567,576 | 1761.9 | 0.901 | 1.76s |
| LFUCache | loop |        870,022 | 1149.4 | 1.000 | 1.15s |
| LFUCache | scan |        671,134 | 1490.0 | 0.000 | 1.49s |
| LRUCache | uniform |        624,460 | 1601.4 | 0.041 | 1.60s |
| LRUCache | zipf |      1,288,580 | 776.0 | 0.876 | 0.78s |
| LRUCache | loop |      2,171,382 | 460.5 | 1.000 | 0.46s |
| LRUCache | scan |      1,014,039 | 986.2 | 0.000 | 0.99s |
| MRUCache | uniform |        646,939 | 1545.7 | 0.041 | 1.55s |
| MRUCache | zipf |        773,824 | 1292.3 | 0.126 | 1.29s |
| MRUCache | loop |      2,202,684 | 454.0 | 1.000 | 0.45s |
| MRUCache | scan |        989,578 | 1010.5 | 0.004 | 1.01s |
| TTLCache | uniform |        398,492 | 2509.5 | 0.041 | 2.51s |
| TTLCache | zipf |        975,602 | 1025.0 | 0.853 | 1.03s |
| TTLCache | loop |      2,059,771 | 485.5 | 1.000 | 0.49s |
| TTLCache | scan |        524,800 | 1905.5 | 0.000 | 1.91s |

---

## `RHMap`

| Policy | Workload | Ops/sec | ns/op | Hit Ratio | Time |
|---|---|---:|---:|---:|---:|
| ARCCache | uniform |        489,726 | 2042.0 | 0.041 | 2.04s |
| ARCCache | zipf |        685,928 | 1457.9 | 0.902 | 1.46s |
| ARCCache | loop |      1,500,463 | 666.5 | 1.000 | 0.67s |
| ARCCache | scan |      1,026,709 | 974.0 | 0.000 | 0.97s |
| FIFOCache | uniform |        813,383 | 1229.4 | 0.041 | 1.23s |
| FIFOCache | zipf |      1,242,338 | 804.9 | 0.853 | 0.80s |
| FIFOCache | loop |      3,921,624 | 255.0 | 1.000 | 0.25s |
| FIFOCache | scan |      1,235,820 | 809.2 | 0.000 | 0.81s |
| LFUCache | uniform |        575,340 | 1738.1 | 0.041 | 1.74s |
| LFUCache | zipf |        513,770 | 1946.4 | 0.901 | 1.95s |
| LFUCache | loop |        998,508 | 1001.5 | 1.000 | 1.00s |
| LFUCache | scan |        827,875 | 1207.9 | 0.000 | 1.21s |
| LRUCache | uniform |        771,567 | 1296.1 | 0.041 | 1.30s |
| LRUCache | zipf |      1,120,831 | 892.2 | 0.876 | 0.89s |
| LRUCache | loop |      2,646,096 | 377.9 | 1.000 | 0.38s |
| LRUCache | scan |      1,229,323 | 813.5 | 0.000 | 0.81s |
| MRUCache | uniform |        803,096 | 1245.2 | 0.041 | 1.25s |
| MRUCache | zipf |        949,905 | 1052.7 | 0.126 | 1.05s |
| MRUCache | loop |      2,454,076 | 407.5 | 1.000 | 0.41s |
| MRUCache | scan |        142,377 | 7023.6 | 0.004 | 7.02s |
| TTLCache | uniform |        440,899 | 2268.1 | 0.041 | 2.27s |
| TTLCache | zipf |        939,785 | 1064.1 | 0.853 | 1.06s |
| TTLCache | loop |      2,333,452 | 428.5 | 1.000 | 0.43s |
| TTLCache | scan |        551,907 | 1811.9 | 0.000 | 1.81s |

---

## `BTHashMap`

| Policy | Workload | Ops/sec | ns/op | Hit Ratio | Time |
|---|---|---:|---:|---:|---:|
| ARCCache | uniform |        345,683 | 2892.8 | 0.041 | 2.89s |
| ARCCache | zipf |        721,668 | 1385.7 | 0.901 | 1.39s |
| ARCCache | loop |      1,158,545 | 863.2 | 1.000 | 0.86s |
| ARCCache | scan |        726,175 | 1377.1 | 0.000 | 1.38s |
| FIFOCache | uniform |        604,005 | 1655.6 | 0.041 | 1.66s |
| FIFOCache | zipf |      1,352,464 | 739.4 | 0.854 | 0.74s |
| FIFOCache | loop |      2,545,429 | 392.9 | 1.000 | 0.39s |
| FIFOCache | scan |        882,675 | 1132.9 | 0.000 | 1.13s |
| LFUCache | uniform |        441,574 | 2264.6 | 0.041 | 2.26s |
| LFUCache | zipf |        515,937 | 1938.2 | 0.901 | 1.94s |
| LFUCache | loop |        742,237 | 1347.3 | 1.000 | 1.35s |
| LFUCache | scan |        604,603 | 1654.0 | 0.000 | 1.65s |
| LRUCache | uniform |        606,880 | 1647.8 | 0.041 | 1.65s |
| LRUCache | zipf |      1,134,204 | 881.7 | 0.876 | 0.88s |
| LRUCache | loop |      1,854,654 | 539.2 | 1.000 | 0.54s |
| LRUCache | scan |        898,507 | 1113.0 | 0.000 | 1.11s |
| MRUCache | uniform |        626,722 | 1595.6 | 0.041 | 1.60s |
| MRUCache | zipf |        769,929 | 1298.8 | 0.124 | 1.30s |
| MRUCache | loop |      1,710,452 | 584.6 | 1.000 | 0.58s |
| MRUCache | scan |        914,302 | 1093.7 | 0.004 | 1.09s |
| TTLCache | uniform |        382,748 | 2612.7 | 0.041 | 2.61s |
| TTLCache | zipf |        933,759 | 1070.9 | 0.853 | 1.07s |
| TTLCache | loop |      1,800,714 | 555.3 | 1.000 | 0.56s |
| TTLCache | scan |        498,086 | 2007.7 | 0.000 | 2.01s |

---

## `HashMap`

| Policy | Workload | Ops/sec | ns/op | Hit Ratio | Time |
|---|---|---:|---:|---:|---:|
| ARCCache | uniform |        381,950 | 2618.1 | 0.041 | 2.62s |
| ARCCache | zipf |        749,479 | 1334.3 | 0.901 | 1.33s |
| ARCCache | loop |      1,039,391 | 962.1 | 1.000 | 0.96s |
| ARCCache | scan |        665,674 | 1502.2 | 0.000 | 1.50s |
| FIFOCache | uniform |        621,524 | 1608.9 | 0.041 | 1.61s |
| FIFOCache | zipf |      1,339,823 | 746.4 | 0.854 | 0.75s |
| FIFOCache | loop |      2,585,739 | 386.7 | 1.000 | 0.39s |
| FIFOCache | scan |        822,501 | 1215.8 | 0.000 | 1.22s |
| LFUCache | uniform |        444,580 | 2249.3 | 0.041 | 2.25s |
| LFUCache | zipf |        507,673 | 1969.8 | 0.901 | 1.97s |
| LFUCache | loop |        670,431 | 1491.6 | 1.000 | 1.49s |
| LFUCache | scan |        566,622 | 1764.8 | 0.000 | 1.76s |
| LRUCache | uniform |        607,383 | 1646.4 | 0.041 | 1.65s |
| LRUCache | zipf |      1,111,703 | 899.5 | 0.876 | 0.90s |
| LRUCache | loop |      1,757,137 | 569.1 | 1.000 | 0.57s |
| LRUCache | scan |        868,990 | 1150.8 | 0.000 | 1.15s |
| MRUCache | uniform |        659,952 | 1515.3 | 0.041 | 1.52s |
| MRUCache | zipf |        772,218 | 1295.0 | 0.125 | 1.29s |
| MRUCache | loop |      1,648,207 | 606.7 | 1.000 | 0.61s |
| MRUCache | scan |        894,266 | 1118.2 | 0.004 | 1.12s |
| TTLCache | uniform |        406,910 | 2457.5 | 0.041 | 2.46s |
| TTLCache | zipf |        958,613 | 1043.2 | 0.853 | 1.04s |
| TTLCache | loop |      1,708,474 | 585.3 | 1.000 | 0.59s |
| TTLCache | scan |        492,699 | 2029.6 | 0.000 | 2.03s |

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
| ARCCache | uniform | `RHMap` |        489,726 | 2042.0 | 0.041 |
| ARCCache | zipf | `SwissTable` |        859,194 | 1163.9 | 0.901 |
| ARCCache | loop | `RHMap` |      1,500,463 | 666.5 | 1.000 |
| ARCCache | scan | `RHMap` |      1,026,709 | 974.0 | 0.000 |
| FIFOCache | uniform | `RHMap` |        813,383 | 1229.4 | 0.041 |
| FIFOCache | zipf | `SwissTable` |      1,501,949 | 665.8 | 0.854 |
| FIFOCache | loop | `RHMap` |      3,921,624 | 255.0 | 1.000 |
| FIFOCache | scan | `RHMap` |      1,235,820 | 809.2 | 0.000 |
| LFUCache | uniform | `RHMap` |        575,340 | 1738.1 | 0.041 |
| LFUCache | zipf | `SwissTable` |        567,576 | 1761.9 | 0.901 |
| LFUCache | loop | `RHMap` |        998,508 | 1001.5 | 1.000 |
| LFUCache | scan | `RHMap` |        827,875 | 1207.9 | 0.000 |
| LRUCache | uniform | `RHMap` |        771,567 | 1296.1 | 0.041 |
| LRUCache | zipf | `SwissTable` |      1,288,580 | 776.0 | 0.876 |
| LRUCache | loop | `RHMap` |      2,646,096 | 377.9 | 1.000 |
| LRUCache | scan | `RHMap` |      1,229,323 | 813.5 | 0.000 |
| MRUCache | uniform | `RHMap` |        803,096 | 1245.2 | 0.041 |
| MRUCache | zipf | `RHMap` |        949,905 | 1052.7 | 0.126 |
| MRUCache | loop | `RHMap` |      2,454,076 | 407.5 | 1.000 |
| MRUCache | scan | `SwissTable` |        989,578 | 1010.5 | 0.004 |
| TTLCache | uniform | `RHMap` |        440,899 | 2268.1 | 0.041 |
| TTLCache | zipf | `SwissTable` |        975,602 | 1025.0 | 0.853 |
| TTLCache | loop | `RHMap` |      2,333,452 | 428.5 | 1.000 |
| TTLCache | scan | `RHMap` |        551,907 | 1811.9 | 0.000 |
