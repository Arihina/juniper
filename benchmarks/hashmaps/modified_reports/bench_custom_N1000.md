# Insert rnd (Key/Value, N=1,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 0.1 мс | 7.90M ops/s | 0.13 мкс |
| HashMap | 0.2 мс | 4.59M ops/s | 0.22 мкс |
| RHMap | 0.1 мс | 6.77M ops/s | 0.15 мкс |
| BTHashMap | 0.5 мс | 1.95M ops/s | 0.51 мкс |
| SwissTable | 0.2 мс | 4.98M ops/s | 0.20 мкс |

# Lookup hit (Key/Value, N=1,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 0.1 мс | 12.52M ops/s | 0.08 мкс |
| HashMap | 0.1 мс | 10.57M ops/s | 0.09 мкс |
| RHMap | 0.1 мс | 12.06M ops/s | 0.08 мкс |
| BTHashMap | 0.2 мс | 4.64M ops/s | 0.22 мкс |
| SwissTable | 0.1 мс | 9.61M ops/s | 0.10 мкс |

# Lookup miss (Key/Value, N=1,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 0.1 мс | 10.78M ops/s | 0.09 мкс |
| HashMap | 0.1 мс | 12.72M ops/s | 0.08 мкс |
| RHMap | 0.1 мс | 13.34M ops/s | 0.07 мкс |
| BTHashMap | 0.3 мс | 3.30M ops/s | 0.30 мкс |
| SwissTable | 0.1 мс | 11.63M ops/s | 0.09 мкс |

# Lookup Zipf (Key/Value, N=1,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 0.1 мс | 10.73M ops/s | 0.09 мкс |
| HashMap | 0.1 мс | 12.25M ops/s | 0.08 мкс |
| RHMap | 0.1 мс | 12.87M ops/s | 0.08 мкс |
| BTHashMap | 0.2 мс | 6.30M ops/s | 0.16 мкс |
| SwissTable | 0.1 мс | 11.38M ops/s | 0.09 мкс |

# Update all (Key/Value, N=1,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 0.1 мс | 12.10M ops/s | 0.08 мкс |
| HashMap | 0.1 мс | 8.79M ops/s | 0.11 мкс |
| RHMap | 0.1 мс | 12.24M ops/s | 0.08 мкс |
| BTHashMap | 0.2 мс | 4.06M ops/s | 0.25 мкс |
| SwissTable | 0.1 мс | 9.75M ops/s | 0.10 мкс |

# Insert+Delete (Key/Value, N=1,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 0.2 мс | 10.46M ops/s | 0.10 мкс |
| HashMap | 0.4 мс | 5.64M ops/s | 0.18 мкс |
| RHMap | 0.2 мс | 9.34M ops/s | 0.11 мкс |
| BTHashMap | 0.9 мс | 2.14M ops/s | 0.47 мкс |
| SwissTable | 0.3 мс | 6.07M ops/s | 0.16 мкс |

# Mixed 80R/20W (Key/Value, N=1,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 0.1 мс | 10.42M ops/s | 0.10 мкс |
| HashMap | 0.1 мс | 8.98M ops/s | 0.11 мкс |
| RHMap | 0.1 мс | 9.59M ops/s | 0.10 мкс |
| BTHashMap | 0.3 мс | 3.83M ops/s | 0.26 мкс |
| SwissTable | 0.2 мс | 5.18M ops/s | 0.19 мкс |

# Latency percentiles (Key/Value, N=1,000)

| Структура | avg | p50 | p90 | p99 | p99.9 |
| --- | --- | --- | --- | --- | --- |
| dict | 0.14μs | 0.14μs | 0.17μs | 0.20μs | 0.55μs |
| HashMap | 0.20μs | 0.16μs | 0.26μs | 0.68μs | 1.11μs |
| RHMap | 0.14μs | 0.14μs | 0.17μs | 0.20μs | 0.93μs |
| BTHashMap | 0.32μs | 0.29μs | 0.40μs | 0.76μs | 1.75μs |
| SwissTable | 0.18μs | 0.17μs | 0.19μs | 0.24μs | 1.26μs |

