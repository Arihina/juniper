# Insert rnd (int, N=500,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 95.7 мс | 5.22M ops/s | 0.19 мкс |
| HashMap | 560.0 мс | 892.9K ops/s | 1.12 мкс |
| RHMap | 146.7 мс | 3.41M ops/s | 0.29 мкс |
| BTHashMap | 377.5 мс | 1.32M ops/s | 0.75 мкс |
| SwissTable | 163.6 мс | 3.06M ops/s | 0.33 мкс |

# Lookup hit (int, N=500,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 121.7 мс | 4.11M ops/s | 0.24 мкс |
| HashMap | 187.2 мс | 2.67M ops/s | 0.37 мкс |
| RHMap | 103.6 мс | 4.83M ops/s | 0.21 мкс |
| BTHashMap | 203.0 мс | 2.46M ops/s | 0.41 мкс |
| SwissTable | 118.6 мс | 4.21M ops/s | 0.24 мкс |

# Lookup miss (int, N=500,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 46.7 мс | 10.70M ops/s | 0.09 мкс |
| HashMap | 40.1 мс | 12.46M ops/s | 0.08 мкс |
| RHMap | 46.4 мс | 10.78M ops/s | 0.09 мкс |
| BTHashMap | 212.3 мс | 2.35M ops/s | 0.42 мкс |
| SwissTable | 111.7 мс | 4.48M ops/s | 0.22 мкс |

# Lookup Zipf (int, N=500,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 10.3 мс | 48.52M ops/s | 0.02 мкс |
| HashMap | 13.3 мс | 37.51M ops/s | 0.03 мкс |
| RHMap | 10.8 мс | 46.51M ops/s | 0.02 мкс |
| BTHashMap | 24.1 мс | 20.78M ops/s | 0.05 мкс |
| SwissTable | 18.8 мс | 26.65M ops/s | 0.04 мкс |

# Update all (int, N=500,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 64.7 мс | 7.73M ops/s | 0.13 мкс |
| HashMap | 129.7 мс | 3.86M ops/s | 0.26 мкс |
| RHMap | 93.1 мс | 5.37M ops/s | 0.19 мкс |
| BTHashMap | 173.5 мс | 2.88M ops/s | 0.35 мкс |
| SwissTable | 113.5 мс | 4.41M ops/s | 0.23 мкс |

# Insert+Delete (int, N=500,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 133.9 мс | 7.47M ops/s | 0.13 мкс |
| HashMap | 467.1 мс | 2.14M ops/s | 0.47 мкс |
| RHMap | 217.8 мс | 4.59M ops/s | 0.22 мкс |
| BTHashMap | 526.7 мс | 1.90M ops/s | 0.53 мкс |
| SwissTable | 263.0 мс | 3.80M ops/s | 0.26 мкс |

# Mixed 80R/20W (int, N=500,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 101.4 мс | 4.93M ops/s | 0.20 мкс |
| HashMap | 159.2 мс | 3.14M ops/s | 0.32 мкс |
| RHMap | 112.2 мс | 4.45M ops/s | 0.22 мкс |
| BTHashMap | 200.8 мс | 2.49M ops/s | 0.40 мкс |
| SwissTable | 132.2 мс | 3.78M ops/s | 0.26 мкс |

# Latency percentiles (int, N=500,000)

| Структура | avg | p50 | p90 | p99 | p99.9 |
| --- | --- | --- | --- | --- | --- |
| dict | 0.34μs | 0.29μs | 0.46μs | 0.94μs | 9.54μs |
| HashMap | 0.56μs | 0.43μs | 0.98μs | 1.81μs | 11.45μs |
| RHMap | 0.26μs | 0.22μs | 0.29μs | 0.79μs | 1.21μs |
| BTHashMap | 0.44μs | 0.39μs | 0.52μs | 1.01μs | 2.14μs |
| SwissTable | 0.30μs | 0.26μs | 0.33μs | 0.85μs | 10.05μs |

