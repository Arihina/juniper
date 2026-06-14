# Insert rnd (str, N=1,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 0.1 мс | 13.46M ops/s | 0.07 мкс |
| HashMap | 0.2 мс | 5.86M ops/s | 0.17 мкс |
| RHMap | 0.1 мс | 10.66M ops/s | 0.09 мкс |
| BTHashMap | 0.2 мс | 5.03M ops/s | 0.20 мкс |
| SwissTable | 3.7 мс | 272.1K ops/s | 3.68 мкс |

# Lookup hit (str, N=1,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 0.0 мс | 39.30M ops/s | 0.03 мкс |
| HashMap | 0.0 мс | 26.51M ops/s | 0.04 мкс |
| RHMap | 0.0 мс | 34.86M ops/s | 0.03 мкс |
| BTHashMap | 0.1 мс | 14.52M ops/s | 0.07 мкс |
| SwissTable | 0.1 мс | 16.29M ops/s | 0.06 мкс |

# Lookup miss (str, N=1,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 0.0 мс | 35.01M ops/s | 0.03 мкс |
| HashMap | 0.0 мс | 31.92M ops/s | 0.03 мкс |
| RHMap | 0.0 мс | 37.52M ops/s | 0.03 мкс |
| BTHashMap | 0.1 мс | 13.82M ops/s | 0.07 мкс |
| SwissTable | 0.1 мс | 19.99M ops/s | 0.05 мкс |

# Lookup Zipf (str, N=1,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 0.0 мс | 50.98M ops/s | 0.02 мкс |
| HashMap | 0.0 мс | 38.42M ops/s | 0.03 мкс |
| RHMap | 0.0 мс | 48.22M ops/s | 0.02 мкс |
| BTHashMap | 0.1 мс | 19.72M ops/s | 0.05 мкс |
| SwissTable | 0.0 мс | 27.90M ops/s | 0.04 мкс |

# Update all (str, N=1,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 0.0 мс | 35.84M ops/s | 0.03 мкс |
| HashMap | 0.0 мс | 26.07M ops/s | 0.04 мкс |
| RHMap | 0.0 мс | 32.10M ops/s | 0.03 мкс |
| BTHashMap | 0.1 мс | 14.17M ops/s | 0.07 мкс |
| SwissTable | 0.0 мс | 20.86M ops/s | 0.05 мкс |

# Insert+Delete (str, N=1,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 0.1 мс | 27.04M ops/s | 0.04 мкс |
| HashMap | 0.2 мс | 10.09M ops/s | 0.10 мкс |
| RHMap | 0.1 мс | 18.03M ops/s | 0.06 мкс |
| BTHashMap | 0.3 мс | 6.68M ops/s | 0.15 мкс |
| SwissTable | 0.2 мс | 10.14M ops/s | 0.10 мкс |

# Mixed 80R/20W (str, N=1,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 0.0 мс | 25.03M ops/s | 0.04 мкс |
| HashMap | 0.1 мс | 15.66M ops/s | 0.06 мкс |
| RHMap | 0.0 мс | 22.93M ops/s | 0.04 мкс |
| BTHashMap | 0.1 мс | 11.29M ops/s | 0.09 мкс |
| SwissTable | 0.1 мс | 15.76M ops/s | 0.06 мкс |

# Latency percentiles (str, N=1,000)

| Структура | avg | p50 | p90 | p99 | p99.9 |
| --- | --- | --- | --- | --- | --- |
| dict | 0.09μs | 0.07μs | 0.09μs | 0.13μs | 0.34μs |
| HashMap | 0.09μs | 0.09μs | 0.11μs | 0.16μs | 0.54μs |
| RHMap | 0.09μs | 0.08μs | 0.10μs | 0.14μs | 0.70μs |
| BTHashMap | 0.23μs | 0.20μs | 0.28μs | 0.79μs | 2.45μs |
| SwissTable | 0.12μs | 0.10μs | 0.11μs | 0.19μs | 1.76μs |

# Insert rnd (str, N=1,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 0.1 мс | 11.58M ops/s | 0.09 мкс |
| HashMap | 0.2 мс | 6.22M ops/s | 0.16 мкс |
| RHMap | 0.1 мс | 10.61M ops/s | 0.09 мкс |
| BTHashMap | 0.2 мс | 5.52M ops/s | 0.18 мкс |
| SwissTable | 0.1 мс | 8.23M ops/s | 0.12 мкс |

# Lookup hit (str, N=1,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 0.0 мс | 37.92M ops/s | 0.03 мкс |
| HashMap | 0.0 мс | 28.34M ops/s | 0.04 мкс |
| RHMap | 0.0 мс | 33.84M ops/s | 0.03 мкс |
| BTHashMap | 0.1 мс | 15.40M ops/s | 0.06 мкс |
| SwissTable | 0.0 мс | 23.92M ops/s | 0.04 мкс |

# Lookup miss (str, N=1,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 0.1 мс | 14.66M ops/s | 0.07 мкс |
| HashMap | 0.0 мс | 36.36M ops/s | 0.03 мкс |
| RHMap | 0.0 мс | 37.19M ops/s | 0.03 мкс |
| BTHashMap | 0.1 мс | 13.82M ops/s | 0.07 мкс |
| SwissTable | 0.0 мс | 28.07M ops/s | 0.04 мкс |

# Lookup Zipf (str, N=1,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 0.0 мс | 50.93M ops/s | 0.02 мкс |
| HashMap | 0.0 мс | 39.54M ops/s | 0.03 мкс |
| RHMap | 0.0 мс | 38.58M ops/s | 0.03 мкс |
| BTHashMap | 0.0 мс | 20.37M ops/s | 0.05 мкс |
| SwissTable | 0.0 мс | 25.62M ops/s | 0.04 мкс |

# Update all (str, N=1,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 0.0 мс | 35.79M ops/s | 0.03 мкс |
| HashMap | 0.0 мс | 26.19M ops/s | 0.04 мкс |
| RHMap | 0.1 мс | 16.42M ops/s | 0.06 мкс |
| BTHashMap | 0.1 мс | 14.70M ops/s | 0.07 мкс |
| SwissTable | 0.0 мс | 24.34M ops/s | 0.04 мкс |

# Insert+Delete (str, N=1,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 0.1 мс | 27.29M ops/s | 0.04 мкс |
| HashMap | 0.3 мс | 7.32M ops/s | 0.14 мкс |
| RHMap | 0.1 мс | 18.29M ops/s | 0.05 мкс |
| BTHashMap | 0.3 мс | 7.39M ops/s | 0.14 мкс |
| SwissTable | 0.2 мс | 12.50M ops/s | 0.08 мкс |

# Mixed 80R/20W (str, N=1,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 0.0 мс | 23.17M ops/s | 0.04 мкс |
| HashMap | 0.1 мс | 18.90M ops/s | 0.05 мкс |
| RHMap | 0.0 мс | 24.07M ops/s | 0.04 мкс |
| BTHashMap | 0.1 мс | 8.28M ops/s | 0.12 мкс |
| SwissTable | 0.1 мс | 18.09M ops/s | 0.06 мкс |

# Latency percentiles (str, N=1,000)

| Структура | avg | p50 | p90 | p99 | p99.9 |
| --- | --- | --- | --- | --- | --- |
| dict | 0.09μs | 0.07μs | 0.10μs | 0.13μs | 0.41μs |
| HashMap | 0.13μs | 0.09μs | 0.17μs | 0.35μs | 14.56μs |
| RHMap | 0.09μs | 0.08μs | 0.10μs | 0.18μs | 0.77μs |
| BTHashMap | 0.15μs | 0.12μs | 0.16μs | 0.28μs | 2.11μs |
| SwissTable | 0.11μs | 0.09μs | 0.13μs | 0.23μs | 1.80μs |

