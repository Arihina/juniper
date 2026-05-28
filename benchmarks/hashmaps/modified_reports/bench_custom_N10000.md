# Insert rnd (Key/Value, N=10,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 1.3 мс | 7.95M ops/s | 0.13 мкс |
| HashMap | 2.6 мс | 3.89M ops/s | 0.26 мкс |
| RHMap | 1.8 мс | 5.68M ops/s | 0.18 мкс |
| BTHashMap | 10.6 мс | 944.0K ops/s | 1.06 мкс |
| SwissTable | 2.3 мс | 4.30M ops/s | 0.23 мкс |

# Lookup hit (Key/Value, N=10,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 2.1 мс | 4.70M ops/s | 0.21 мкс |
| HashMap | 4.4 мс | 2.28M ops/s | 0.44 мкс |
| RHMap | 2.1 мс | 4.74M ops/s | 0.21 мкс |
| BTHashMap | 4.5 мс | 2.22M ops/s | 0.45 мкс |
| SwissTable | 1.7 мс | 5.82M ops/s | 0.17 мкс |

# Lookup miss (Key/Value, N=10,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 0.9 мс | 11.36M ops/s | 0.09 мкс |
| HashMap | 1.2 мс | 8.50M ops/s | 0.12 мкс |
| RHMap | 0.9 мс | 10.77M ops/s | 0.09 мкс |
| BTHashMap | 3.0 мс | 3.38M ops/s | 0.30 мкс |
| SwissTable | 1.0 мс | 10.46M ops/s | 0.10 мкс |

# Lookup Zipf (Key/Value, N=10,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 0.8 мс | 13.27M ops/s | 0.08 мкс |
| HashMap | 1.3 мс | 7.85M ops/s | 0.13 мкс |
| RHMap | 1.5 мс | 6.63M ops/s | 0.15 мкс |
| BTHashMap | 1.5 мс | 6.76M ops/s | 0.15 мкс |
| SwissTable | 1.0 мс | 10.36M ops/s | 0.10 мкс |

# Update all (Key/Value, N=10,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 1.4 мс | 7.09M ops/s | 0.14 мкс |
| HashMap | 2.0 мс | 4.92M ops/s | 0.20 мкс |
| RHMap | 1.0 мс | 9.92M ops/s | 0.10 мкс |
| BTHashMap | 3.5 мс | 2.83M ops/s | 0.35 мкс |
| SwissTable | 1.4 мс | 6.93M ops/s | 0.14 мкс |

# Insert+Delete (Key/Value, N=10,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 2.0 мс | 9.80M ops/s | 0.10 мкс |
| HashMap | 5.2 мс | 3.84M ops/s | 0.26 мкс |
| RHMap | 2.9 мс | 6.84M ops/s | 0.15 мкс |
| BTHashMap | 13.5 мс | 1.48M ops/s | 0.68 мкс |
| SwissTable | 3.5 мс | 5.72M ops/s | 0.17 мкс |

# Mixed 80R/20W (Key/Value, N=10,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 1.8 мс | 5.62M ops/s | 0.18 мкс |
| HashMap | 2.0 мс | 5.09M ops/s | 0.20 мкс |
| RHMap | 1.7 мс | 5.84M ops/s | 0.17 мкс |
| BTHashMap | 3.3 мс | 3.05M ops/s | 0.33 мкс |
| SwissTable | 1.5 мс | 6.81M ops/s | 0.15 мкс |

# Latency percentiles (Key/Value, N=10,000)

| Структура | avg | p50 | p90 | p99 | p99.9 |
| --- | --- | --- | --- | --- | --- |
| dict | 0.18μs | 0.17μs | 0.23μs | 0.58μs | 0.89μs |
| HashMap | 0.32μs | 0.30μs | 0.43μs | 0.91μs | 1.36μs |
| RHMap | 0.24μs | 0.21μs | 0.29μs | 0.77μs | 1.70μs |
| BTHashMap | 0.76μs | 0.58μs | 1.30μs | 2.60μs | 19.47μs |
| SwissTable | 0.32μs | 0.28μs | 0.42μs | 0.97μs | 7.87μs |

