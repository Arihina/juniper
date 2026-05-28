# Insert rnd (str, N=10,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 0.6 мс | 17.08M ops/s | 0.06 мкс |
| HashMap | 5.0 мс | 2.00M ops/s | 0.50 мкс |
| RHMap | 0.9 мс | 10.57M ops/s | 0.09 мкс |
| BTHashMap | 3.7 мс | 2.70M ops/s | 0.37 мкс |
| SwissTable | 1.4 мс | 7.36M ops/s | 0.14 мкс |

# Lookup hit (str, N=10,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 0.5 мс | 20.51M ops/s | 0.05 мкс |
| HashMap | 3.6 мс | 2.75M ops/s | 0.36 мкс |
| RHMap | 0.4 мс | 22.25M ops/s | 0.04 мкс |
| BTHashMap | 1.1 мс | 9.06M ops/s | 0.11 мкс |
| SwissTable | 0.8 мс | 12.35M ops/s | 0.08 мкс |

# Lookup miss (str, N=10,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 0.4 мс | 26.24M ops/s | 0.04 мкс |
| HashMap | 0.5 мс | 21.76M ops/s | 0.05 мкс |
| RHMap | 0.5 мс | 20.74M ops/s | 0.05 мкс |
| BTHashMap | 1.7 мс | 5.92M ops/s | 0.17 мкс |
| SwissTable | 0.4 мс | 24.79M ops/s | 0.04 мкс |

# Lookup Zipf (str, N=10,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 0.2 мс | 55.13M ops/s | 0.02 мкс |
| HashMap | 0.3 мс | 38.51M ops/s | 0.03 мкс |
| RHMap | 0.2 мс | 42.08M ops/s | 0.02 мкс |
| BTHashMap | 0.5 мс | 21.34M ops/s | 0.05 мкс |
| SwissTable | 0.3 мс | 28.70M ops/s | 0.03 мкс |

# Update all (str, N=10,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 0.3 мс | 30.21M ops/s | 0.03 мкс |
| HashMap | 0.6 мс | 17.83M ops/s | 0.06 мкс |
| RHMap | 2.7 мс | 3.69M ops/s | 0.27 мкс |
| BTHashMap | 1.0 мс | 10.13M ops/s | 0.10 мкс |
| SwissTable | 0.5 мс | 19.30M ops/s | 0.05 мкс |

# Insert+Delete (str, N=10,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 1.0 мс | 20.07M ops/s | 0.05 мкс |
| HashMap | 2.2 мс | 8.93M ops/s | 0.11 мкс |
| RHMap | 1.2 мс | 17.32M ops/s | 0.06 мкс |
| BTHashMap | 5.7 мс | 3.50M ops/s | 0.29 мкс |
| SwissTable | 1.9 мс | 10.73M ops/s | 0.09 мкс |

# Mixed 80R/20W (str, N=10,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 0.7 мс | 14.99M ops/s | 0.07 мкс |
| HashMap | 1.5 мс | 6.82M ops/s | 0.15 мкс |
| RHMap | 0.8 мс | 12.92M ops/s | 0.08 мкс |
| BTHashMap | 1.0 мс | 9.60M ops/s | 0.10 мкс |
| SwissTable | 0.8 мс | 13.32M ops/s | 0.08 мкс |

# Latency percentiles (str, N=10,000)

| Структура | avg | p50 | p90 | p99 | p99.9 |
| --- | --- | --- | --- | --- | --- |
| dict | 0.11μs | 0.09μs | 0.14μs | 0.24μs | 0.80μs |
| HashMap | 0.37μs | 0.27μs | 0.53μs | 1.09μs | 23.59μs |
| RHMap | 0.10μs | 0.09μs | 0.14μs | 0.30μs | 0.66μs |
| BTHashMap | 0.21μs | 0.16μs | 0.31μs | 0.82μs | 1.91μs |
| SwissTable | 0.18μs | 0.16μs | 0.22μs | 0.68μs | 0.88μs |

