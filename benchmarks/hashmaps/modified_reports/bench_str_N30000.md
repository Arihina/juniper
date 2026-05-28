# Insert rnd (str, N=30,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 2.5 мс | 12.18M ops/s | 0.08 мкс |
| HashMap | 10.9 мс | 2.74M ops/s | 0.36 мкс |
| RHMap | 3.3 мс | 9.15M ops/s | 0.11 мкс |
| BTHashMap | 10.8 мс | 2.77M ops/s | 0.36 мкс |
| SwissTable | 5.2 мс | 5.79M ops/s | 0.17 мкс |

# Lookup hit (str, N=30,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 2.9 мс | 10.40M ops/s | 0.10 мкс |
| HashMap | 8.7 мс | 3.46M ops/s | 0.29 мкс |
| RHMap | 3.1 мс | 9.76M ops/s | 0.10 мкс |
| BTHashMap | 9.1 мс | 3.31M ops/s | 0.30 мкс |
| SwissTable | 5.0 мс | 5.95M ops/s | 0.17 мкс |

# Lookup miss (str, N=30,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 1.7 мс | 17.68M ops/s | 0.06 мкс |
| HashMap | 2.1 мс | 14.25M ops/s | 0.07 мкс |
| RHMap | 1.4 мс | 22.13M ops/s | 0.05 мкс |
| BTHashMap | 5.4 мс | 5.58M ops/s | 0.18 мкс |
| SwissTable | 1.9 мс | 16.04M ops/s | 0.06 мкс |

# Lookup Zipf (str, N=30,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 0.6 мс | 48.28M ops/s | 0.02 мкс |
| HashMap | 0.9 мс | 33.29M ops/s | 0.03 мкс |
| RHMap | 0.7 мс | 43.89M ops/s | 0.02 мкс |
| BTHashMap | 1.3 мс | 23.57M ops/s | 0.04 мкс |
| SwissTable | 1.1 мс | 26.85M ops/s | 0.04 мкс |

# Update all (str, N=30,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 1.7 мс | 18.17M ops/s | 0.06 мкс |
| HashMap | 3.4 мс | 8.72M ops/s | 0.11 мкс |
| RHMap | 2.2 мс | 13.36M ops/s | 0.07 мкс |
| BTHashMap | 6.6 мс | 4.52M ops/s | 0.22 мкс |
| SwissTable | 2.9 мс | 10.47M ops/s | 0.10 мкс |

# Insert+Delete (str, N=30,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 9.1 мс | 6.62M ops/s | 0.15 мкс |
| HashMap | 19.0 мс | 3.15M ops/s | 0.32 мкс |
| RHMap | 5.2 мс | 11.49M ops/s | 0.09 мкс |
| BTHashMap | 24.3 мс | 2.47M ops/s | 0.41 мкс |
| SwissTable | 7.7 мс | 7.84M ops/s | 0.13 мкс |

# Mixed 80R/20W (str, N=30,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 4.7 мс | 6.37M ops/s | 0.16 мкс |
| HashMap | 6.3 мс | 4.75M ops/s | 0.21 мкс |
| RHMap | 3.7 мс | 8.17M ops/s | 0.12 мкс |
| BTHashMap | 7.6 мс | 3.97M ops/s | 0.25 мкс |
| SwissTable | 3.3 мс | 9.18M ops/s | 0.11 мкс |

# Latency percentiles (str, N=30,000)

| Структура | avg | p50 | p90 | p99 | p99.9 |
| --- | --- | --- | --- | --- | --- |
| dict | 0.16μs | 0.15μs | 0.21μs | 0.66μs | 0.83μs |
| HashMap | 0.32μs | 0.29μs | 0.40μs | 0.87μs | 1.66μs |
| RHMap | 0.19μs | 0.15μs | 0.23μs | 0.69μs | 0.99μs |
| BTHashMap | 0.31μs | 0.27μs | 0.40μs | 0.89μs | 1.84μs |
| SwissTable | 0.29μs | 0.26μs | 0.42μs | 0.98μs | 1.19μs |

