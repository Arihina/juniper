# Insert rnd (int, N=30,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 2.5 мс | 11.96M ops/s | 0.08 мкс |
| HashMap | 11.2 мс | 2.67M ops/s | 0.37 мкс |
| RHMap | 9.5 мс | 3.15M ops/s | 0.32 мкс |
| BTHashMap | 8.8 мс | 3.41M ops/s | 0.29 мкс |
| SwissTable | 6.9 мс | 4.37M ops/s | 0.23 мкс |

# Lookup hit (int, N=30,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 2.6 мс | 11.57M ops/s | 0.09 мкс |
| HashMap | 5.8 мс | 5.14M ops/s | 0.19 мкс |
| RHMap | 2.4 мс | 12.67M ops/s | 0.08 мкс |
| BTHashMap | 2.8 мс | 10.58M ops/s | 0.09 мкс |
| SwissTable | 3.1 мс | 9.79M ops/s | 0.10 мкс |

# Lookup miss (int, N=30,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 1.0 мс | 29.79M ops/s | 0.03 мкс |
| HashMap | 1.2 мс | 25.48M ops/s | 0.04 мкс |
| RHMap | 1.2 мс | 24.34M ops/s | 0.04 мкс |
| BTHashMap | 3.8 мс | 7.80M ops/s | 0.13 мкс |
| SwissTable | 2.5 мс | 12.20M ops/s | 0.08 мкс |

# Lookup Zipf (int, N=30,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 0.7 мс | 42.45M ops/s | 0.02 мкс |
| HashMap | 0.8 мс | 37.22M ops/s | 0.03 мкс |
| RHMap | 0.6 мс | 50.11M ops/s | 0.02 мкс |
| BTHashMap | 1.9 мс | 15.59M ops/s | 0.06 мкс |
| SwissTable | 1.1 мс | 27.76M ops/s | 0.04 мкс |

# Update all (int, N=30,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 1.1 мс | 27.61M ops/s | 0.04 мкс |
| HashMap | 2.8 мс | 10.61M ops/s | 0.09 мкс |
| RHMap | 1.5 мс | 19.88M ops/s | 0.05 мкс |
| BTHashMap | 3.2 мс | 9.47M ops/s | 0.11 мкс |
| SwissTable | 2.2 мс | 13.53M ops/s | 0.07 мкс |

# Insert+Delete (int, N=30,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 2.6 мс | 23.24M ops/s | 0.04 мкс |
| HashMap | 13.6 мс | 4.42M ops/s | 0.23 мкс |
| RHMap | 3.7 мс | 16.34M ops/s | 0.06 мкс |
| BTHashMap | 15.8 мс | 3.79M ops/s | 0.26 мкс |
| SwissTable | 8.1 мс | 7.41M ops/s | 0.13 мкс |

# Mixed 80R/20W (int, N=30,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 2.9 мс | 10.26M ops/s | 0.10 мкс |
| HashMap | 5.6 мс | 5.33M ops/s | 0.19 мкс |
| RHMap | 2.9 мс | 10.52M ops/s | 0.10 мкс |
| BTHashMap | 10.4 мс | 2.89M ops/s | 0.35 мкс |
| SwissTable | 3.3 мс | 9.08M ops/s | 0.11 мкс |

# Latency percentiles (int, N=30,000)

| Структура | avg | p50 | p90 | p99 | p99.9 |
| --- | --- | --- | --- | --- | --- |
| dict | 0.20μs | 0.19μs | 0.25μs | 0.71μs | 1.13μs |
| HashMap | 0.35μs | 0.31μs | 0.47μs | 1.02μs | 8.97μs |
| RHMap | 0.15μs | 0.14μs | 0.19μs | 0.67μs | 1.40μs |
| BTHashMap | 0.28μs | 0.23μs | 0.46μs | 0.96μs | 2.25μs |
| SwissTable | 0.19μs | 0.17μs | 0.23μs | 0.67μs | 1.36μs |

