# Insert rnd (int, N=10,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 0.7 мс | 14.83M ops/s | 0.07 мкс |
| HashMap | 2.5 мс | 3.95M ops/s | 0.25 мкс |
| RHMap | 2.0 мс | 4.88M ops/s | 0.20 мкс |
| BTHashMap | 2.6 мс | 3.81M ops/s | 0.26 мкс |
| SwissTable | 1.7 мс | 6.05M ops/s | 0.17 мкс |

# Lookup hit (int, N=10,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 0.4 мс | 25.71M ops/s | 0.04 мкс |
| HashMap | 0.7 мс | 14.39M ops/s | 0.07 мкс |
| RHMap | 0.4 мс | 24.19M ops/s | 0.04 мкс |
| BTHashMap | 0.7 мс | 14.21M ops/s | 0.07 мкс |
| SwissTable | 0.6 мс | 15.79M ops/s | 0.06 мкс |

# Lookup miss (int, N=10,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 0.4 мс | 24.80M ops/s | 0.04 мкс |
| HashMap | 0.3 мс | 30.39M ops/s | 0.03 мкс |
| RHMap | 0.3 мс | 33.17M ops/s | 0.03 мкс |
| BTHashMap | 0.8 мс | 12.39M ops/s | 0.08 мкс |
| SwissTable | 1.1 мс | 9.00M ops/s | 0.11 мкс |

# Lookup Zipf (int, N=10,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 0.2 мс | 53.22M ops/s | 0.02 мкс |
| HashMap | 0.3 мс | 37.59M ops/s | 0.03 мкс |
| RHMap | 0.2 мс | 49.77M ops/s | 0.02 мкс |
| BTHashMap | 0.3 мс | 28.61M ops/s | 0.03 мкс |
| SwissTable | 2.2 мс | 4.59M ops/s | 0.22 мкс |

# Update all (int, N=10,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 0.3 мс | 39.10M ops/s | 0.03 мкс |
| HashMap | 0.7 мс | 14.29M ops/s | 0.07 мкс |
| RHMap | 0.3 мс | 28.71M ops/s | 0.03 мкс |
| BTHashMap | 0.7 мс | 13.63M ops/s | 0.07 мкс |
| SwissTable | 0.6 мс | 16.78M ops/s | 0.06 мкс |

# Insert+Delete (int, N=10,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 0.8 мс | 23.64M ops/s | 0.04 мкс |
| HashMap | 2.8 мс | 7.24M ops/s | 0.14 мкс |
| RHMap | 5.4 мс | 3.67M ops/s | 0.27 мкс |
| BTHashMap | 7.0 мс | 2.85M ops/s | 0.35 мкс |
| SwissTable | 2.6 мс | 7.62M ops/s | 0.13 мкс |

# Mixed 80R/20W (int, N=10,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 0.6 мс | 17.00M ops/s | 0.06 мкс |
| HashMap | 0.6 мс | 16.88M ops/s | 0.06 мкс |
| RHMap | 0.5 мс | 20.72M ops/s | 0.05 мкс |
| BTHashMap | 3.0 мс | 3.31M ops/s | 0.30 мкс |
| SwissTable | 1.5 мс | 6.67M ops/s | 0.15 мкс |

# Latency percentiles (int, N=10,000)

| Структура | avg | p50 | p90 | p99 | p99.9 |
| --- | --- | --- | --- | --- | --- |
| dict | 0.14μs | 0.10μs | 0.20μs | 0.61μs | 1.22μs |
| HashMap | 0.21μs | 0.16μs | 0.27μs | 0.71μs | 15.86μs |
| RHMap | 0.21μs | 0.16μs | 0.31μs | 0.85μs | 8.88μs |
| BTHashMap | 0.14μs | 0.13μs | 0.19μs | 0.39μs | 0.77μs |
| SwissTable | 0.13μs | 0.11μs | 0.16μs | 0.31μs | 0.73μs |

