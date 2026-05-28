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

