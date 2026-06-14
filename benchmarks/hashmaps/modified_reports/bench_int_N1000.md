# Insert rnd (int, N=1,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 0.1 мс | 11.15M ops/s | 0.09 мкс |
| HashMap | 0.2 мс | 5.73M ops/s | 0.17 мкс |
| RHMap | 0.1 мс | 10.18M ops/s | 0.10 мкс |
| BTHashMap | 0.1 мс | 7.00M ops/s | 0.14 мкс |
| SwissTable | 0.2 мс | 6.12M ops/s | 0.16 мкс |

# Lookup hit (int, N=1,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 0.0 мс | 47.13M ops/s | 0.02 мкс |
| HashMap | 0.0 мс | 30.11M ops/s | 0.03 мкс |
| RHMap | 0.0 мс | 44.70M ops/s | 0.02 мкс |
| BTHashMap | 0.0 мс | 20.83M ops/s | 0.05 мкс |
| SwissTable | 0.1 мс | 19.88M ops/s | 0.05 мкс |

# Lookup miss (int, N=1,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 0.0 мс | 48.55M ops/s | 0.02 мкс |
| HashMap | 0.0 мс | 50.28M ops/s | 0.02 мкс |
| RHMap | 0.0 мс | 59.09M ops/s | 0.02 мкс |
| BTHashMap | 0.0 мс | 21.71M ops/s | 0.05 мкс |
| SwissTable | 0.0 мс | 20.07M ops/s | 0.05 мкс |

# Lookup Zipf (int, N=1,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 0.0 мс | 50.14M ops/s | 0.02 мкс |
| HashMap | 0.0 мс | 39.69M ops/s | 0.03 мкс |
| RHMap | 0.0 мс | 48.51M ops/s | 0.02 мкс |
| BTHashMap | 0.0 мс | 23.93M ops/s | 0.04 мкс |
| SwissTable | 0.0 мс | 28.23M ops/s | 0.04 мкс |

# Update all (int, N=1,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 0.0 мс | 42.20M ops/s | 0.02 мкс |
| HashMap | 0.0 мс | 27.28M ops/s | 0.04 мкс |
| RHMap | 0.0 мс | 39.19M ops/s | 0.03 мкс |
| BTHashMap | 0.1 мс | 19.74M ops/s | 0.05 мкс |
| SwissTable | 0.0 мс | 21.20M ops/s | 0.05 мкс |

# Insert+Delete (int, N=1,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 0.1 мс | 32.58M ops/s | 0.03 мкс |
| HashMap | 0.2 мс | 10.73M ops/s | 0.09 мкс |
| RHMap | 0.1 мс | 24.69M ops/s | 0.04 мкс |
| BTHashMap | 0.2 мс | 8.70M ops/s | 0.12 мкс |
| SwissTable | 0.2 мс | 9.32M ops/s | 0.11 мкс |

# Mixed 80R/20W (int, N=1,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 0.0 мс | 26.42M ops/s | 0.04 мкс |
| HashMap | 0.0 мс | 21.44M ops/s | 0.05 мкс |
| RHMap | 0.0 мс | 27.11M ops/s | 0.04 мкс |
| BTHashMap | 0.1 мс | 14.90M ops/s | 0.07 мкс |
| SwissTable | 0.1 мс | 14.94M ops/s | 0.07 мкс |

# Latency percentiles (int, N=1,000)

| Структура | avg | p50 | p90 | p99 | p99.9 |
| --- | --- | --- | --- | --- | --- |
| dict | 0.09μs | 0.08μs | 0.12μs | 0.19μs | 0.46μs |
| HashMap | 0.10μs | 0.09μs | 0.12μs | 0.23μs | 0.70μs |
| RHMap | 0.09μs | 0.07μs | 0.08μs | 0.27μs | 0.77μs |
| BTHashMap | 0.46μs | 0.11μs | 0.13μs | 0.28μs | 0.86μs |
| SwissTable | 0.11μs | 0.10μs | 0.11μs | 0.20μs | 0.74μs |

# Insert rnd (int, N=1,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 0.1 мс | 9.64M ops/s | 0.10 мкс |
| HashMap | 0.2 мс | 4.14M ops/s | 0.24 мкс |
| RHMap | 0.1 мс | 8.49M ops/s | 0.12 мкс |
| BTHashMap | 0.1 мс | 7.04M ops/s | 0.14 мкс |
| SwissTable | 0.1 мс | 7.24M ops/s | 0.14 мкс |

# Lookup hit (int, N=1,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 0.0 мс | 44.97M ops/s | 0.02 мкс |
| HashMap | 0.0 мс | 28.66M ops/s | 0.03 мкс |
| RHMap | 0.0 мс | 44.34M ops/s | 0.02 мкс |
| BTHashMap | 0.0 мс | 20.69M ops/s | 0.05 мкс |
| SwissTable | 0.1 мс | 13.47M ops/s | 0.07 мкс |

# Lookup miss (int, N=1,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 0.0 мс | 39.67M ops/s | 0.03 мкс |
| HashMap | 2.0 мс | 502.7K ops/s | 1.99 мкс |
| RHMap | 0.0 мс | 57.19M ops/s | 0.02 мкс |
| BTHashMap | 0.0 мс | 21.41M ops/s | 0.05 мкс |
| SwissTable | 0.0 мс | 26.14M ops/s | 0.04 мкс |

# Lookup Zipf (int, N=1,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 0.0 мс | 47.42M ops/s | 0.02 мкс |
| HashMap | 0.0 мс | 27.65M ops/s | 0.04 мкс |
| RHMap | 0.0 мс | 45.13M ops/s | 0.02 мкс |
| BTHashMap | 0.0 мс | 24.54M ops/s | 0.04 мкс |
| SwissTable | 0.0 мс | 34.22M ops/s | 0.03 мкс |

# Update all (int, N=1,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 0.0 мс | 42.23M ops/s | 0.02 мкс |
| HashMap | 0.0 мс | 28.11M ops/s | 0.04 мкс |
| RHMap | 0.0 мс | 41.75M ops/s | 0.02 мкс |
| BTHashMap | 0.1 мс | 19.81M ops/s | 0.05 мкс |
| SwissTable | 0.0 мс | 24.28M ops/s | 0.04 мкс |

# Insert+Delete (int, N=1,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 0.1 мс | 32.72M ops/s | 0.03 мкс |
| HashMap | 0.2 мс | 9.94M ops/s | 0.10 мкс |
| RHMap | 0.1 мс | 23.51M ops/s | 0.04 мкс |
| BTHashMap | 0.2 мс | 8.03M ops/s | 0.12 мкс |
| SwissTable | 0.2 мс | 8.83M ops/s | 0.11 мкс |

# Mixed 80R/20W (int, N=1,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 0.0 мс | 26.01M ops/s | 0.04 мкс |
| HashMap | 0.1 мс | 19.34M ops/s | 0.05 мкс |
| RHMap | 0.0 мс | 24.07M ops/s | 0.04 мкс |
| BTHashMap | 0.1 мс | 15.00M ops/s | 0.07 мкс |
| SwissTable | 0.1 мс | 17.49M ops/s | 0.06 мкс |

# Latency percentiles (int, N=1,000)

| Структура | avg | p50 | p90 | p99 | p99.9 |
| --- | --- | --- | --- | --- | --- |
| dict | 0.08μs | 0.07μs | 0.08μs | 0.11μs | 0.57μs |
| HashMap | 0.09μs | 0.08μs | 0.10μs | 0.16μs | 0.70μs |
| RHMap | 0.08μs | 0.07μs | 0.08μs | 0.11μs | 0.41μs |
| BTHashMap | 0.11μs | 0.11μs | 0.12μs | 0.18μs | 0.75μs |
| SwissTable | 0.10μs | 0.10μs | 0.11μs | 0.12μs | 0.62μs |

