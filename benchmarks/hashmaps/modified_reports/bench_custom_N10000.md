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

# Insert rnd (Key/Value, N=10,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 1.3 мс | 7.43M ops/s | 0.13 мкс |
| HashMap | 2.8 мс | 3.53M ops/s | 0.28 мкс |
| RHMap | 2.3 мс | 4.36M ops/s | 0.23 мкс |
| BTHashMap | 18.0 мс | 554.9K ops/s | 1.80 мкс |
| SwissTable | 3.5 мс | 2.90M ops/s | 0.35 мкс |

# Lookup hit (Key/Value, N=10,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 2.0 мс | 5.11M ops/s | 0.20 мкс |
| HashMap | 2.9 мс | 3.40M ops/s | 0.29 мкс |
| RHMap | 1.6 мс | 6.40M ops/s | 0.16 мкс |
| BTHashMap | 6.9 мс | 1.44M ops/s | 0.69 мкс |
| SwissTable | 2.2 мс | 4.63M ops/s | 0.22 мкс |

# Lookup miss (Key/Value, N=10,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 0.9 мс | 10.93M ops/s | 0.09 мкс |
| HashMap | 1.4 мс | 7.04M ops/s | 0.14 мкс |
| RHMap | 3.0 мс | 3.34M ops/s | 0.30 мкс |
| BTHashMap | 5.2 мс | 1.94M ops/s | 0.52 мкс |
| SwissTable | 6.9 мс | 1.45M ops/s | 0.69 мкс |

# Lookup Zipf (Key/Value, N=10,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 1.3 мс | 7.91M ops/s | 0.13 мкс |
| HashMap | 1.3 мс | 7.41M ops/s | 0.13 мкс |
| RHMap | 1.3 мс | 7.57M ops/s | 0.13 мкс |
| BTHashMap | 1.3 мс | 7.79M ops/s | 0.13 мкс |
| SwissTable | 0.9 мс | 11.23M ops/s | 0.09 мкс |

# Update all (Key/Value, N=10,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 2.3 мс | 4.26M ops/s | 0.23 мкс |
| HashMap | 2.1 мс | 4.76M ops/s | 0.21 мкс |
| RHMap | 1.5 мс | 6.72M ops/s | 0.15 мкс |
| BTHashMap | 10.3 мс | 975.0K ops/s | 1.03 мкс |
| SwissTable | 3.5 мс | 2.82M ops/s | 0.35 мкс |

# Insert+Delete (Key/Value, N=10,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 2.7 мс | 7.31M ops/s | 0.14 мкс |
| HashMap | 6.4 мс | 3.12M ops/s | 0.32 мкс |
| RHMap | 5.0 мс | 4.04M ops/s | 0.25 мкс |
| BTHashMap | 15.8 мс | 1.27M ops/s | 0.79 мкс |
| SwissTable | 4.2 мс | 4.77M ops/s | 0.21 мкс |

# Mixed 80R/20W (Key/Value, N=10,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 1.3 мс | 7.52M ops/s | 0.13 мкс |
| HashMap | 1.7 мс | 6.04M ops/s | 0.17 мкс |
| RHMap | 1.6 мс | 6.08M ops/s | 0.16 мкс |
| BTHashMap | 6.4 мс | 1.56M ops/s | 0.64 мкс |
| SwissTable | 1.9 мс | 5.32M ops/s | 0.19 мкс |

# Latency percentiles (Key/Value, N=10,000)

| Структура | avg | p50 | p90 | p99 | p99.9 |
| --- | --- | --- | --- | --- | --- |
| dict | 0.35μs | 0.29μs | 0.49μs | 1.05μs | 12.56μs |
| HashMap | 1.51μs | 0.54μs | 0.90μs | 1.56μs | 24.10μs |
| RHMap | 0.91μs | 0.62μs | 1.09μs | 1.78μs | 45.23μs |
| BTHashMap | 0.71μs | 0.61μs | 1.00μs | 1.71μs | 18.00μs |
| SwissTable | 0.58μs | 0.33μs | 0.55μs | 1.12μs | 17.96μs |

