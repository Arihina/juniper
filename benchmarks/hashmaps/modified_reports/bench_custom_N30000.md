# Insert rnd (Key/Value, N=30,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 4.1 мс | 7.30M ops/s | 0.14 мкс |
| HashMap | 12.9 мс | 2.33M ops/s | 0.43 мкс |
| RHMap | 7.4 мс | 4.05M ops/s | 0.25 мкс |
| BTHashMap | 35.3 мс | 850.9K ops/s | 1.18 мкс |
| SwissTable | 6.6 мс | 4.53M ops/s | 0.22 мкс |

# Lookup hit (Key/Value, N=30,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 7.6 мс | 3.93M ops/s | 0.25 мкс |
| HashMap | 11.0 мс | 2.74M ops/s | 0.37 мкс |
| RHMap | 7.7 мс | 3.89M ops/s | 0.26 мкс |
| BTHashMap | 16.0 мс | 1.87M ops/s | 0.53 мкс |
| SwissTable | 8.2 мс | 3.68M ops/s | 0.27 мкс |

# Lookup miss (Key/Value, N=30,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 3.0 мс | 9.86M ops/s | 0.10 мкс |
| HashMap | 3.3 мс | 9.02M ops/s | 0.11 мкс |
| RHMap | 3.3 мс | 9.13M ops/s | 0.11 мкс |
| BTHashMap | 21.5 мс | 1.40M ops/s | 0.72 мкс |
| SwissTable | 2.9 мс | 10.37M ops/s | 0.10 мкс |

# Lookup Zipf (Key/Value, N=30,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 2.7 мс | 11.26M ops/s | 0.09 мкс |
| HashMap | 2.7 мс | 11.21M ops/s | 0.09 мкс |
| RHMap | 2.5 мс | 11.89M ops/s | 0.08 мкс |
| BTHashMap | 7.0 мс | 4.30M ops/s | 0.23 мкс |
| SwissTable | 2.9 мс | 10.29M ops/s | 0.10 мкс |

# Update all (Key/Value, N=30,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 5.8 мс | 5.20M ops/s | 0.19 мкс |
| HashMap | 7.0 мс | 4.29M ops/s | 0.23 мкс |
| RHMap | 4.0 мс | 7.44M ops/s | 0.13 мкс |
| BTHashMap | 16.8 мс | 1.79M ops/s | 0.56 мкс |
| SwissTable | 6.5 мс | 4.62M ops/s | 0.22 мкс |

# Insert+Delete (Key/Value, N=30,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 8.2 мс | 7.27M ops/s | 0.14 мкс |
| HashMap | 21.2 мс | 2.82M ops/s | 0.35 мкс |
| RHMap | 9.5 мс | 6.32M ops/s | 0.16 мкс |
| BTHashMap | 48.2 мс | 1.25M ops/s | 0.80 мкс |
| SwissTable | 12.1 мс | 4.97M ops/s | 0.20 мкс |

# Mixed 80R/20W (Key/Value, N=30,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 6.9 мс | 4.35M ops/s | 0.23 мкс |
| HashMap | 9.5 мс | 3.16M ops/s | 0.32 мкс |
| RHMap | 5.5 мс | 5.45M ops/s | 0.18 мкс |
| BTHashMap | 15.3 мс | 1.96M ops/s | 0.51 мкс |
| SwissTable | 5.6 мс | 5.34M ops/s | 0.19 мкс |

# Latency percentiles (Key/Value, N=30,000)

| Структура | avg | p50 | p90 | p99 | p99.9 |
| --- | --- | --- | --- | --- | --- |
| dict | 0.34μs | 0.31μs | 0.46μs | 0.96μs | 1.63μs |
| HashMap | 0.42μs | 0.39μs | 0.52μs | 1.02μs | 7.38μs |
| RHMap | 0.47μs | 0.41μs | 0.64μs | 1.14μs | 10.39μs |
| BTHashMap | 0.73μs | 0.65μs | 0.99μs | 1.48μs | 14.79μs |
| SwissTable | 0.37μs | 0.33μs | 0.47μs | 0.99μs | 2.06μs |

