# Insert rnd (str, N=500,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 104.0 мс | 4.81M ops/s | 0.21 мкс |
| HashMap | 525.2 мс | 952.0K ops/s | 1.05 мкс |
| RHMap | 121.2 мс | 4.13M ops/s | 0.24 мкс |
| BTHashMap | 391.0 мс | 1.28M ops/s | 0.78 мкс |
| SwissTable | 103.3 мс | 4.84M ops/s | 0.21 мкс |

# Lookup hit (str, N=500,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 149.3 мс | 3.35M ops/s | 0.30 мкс |
| HashMap | 203.7 мс | 2.45M ops/s | 0.41 мкс |
| RHMap | 110.0 мс | 4.54M ops/s | 0.22 мкс |
| BTHashMap | 237.3 мс | 2.11M ops/s | 0.47 мкс |
| SwissTable | 127.0 мс | 3.94M ops/s | 0.25 мкс |

# Lookup miss (str, N=500,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 71.6 мс | 6.99M ops/s | 0.14 мкс |
| HashMap | 82.5 мс | 6.06M ops/s | 0.16 мкс |
| RHMap | 61.3 мс | 8.16M ops/s | 0.12 мкс |
| BTHashMap | 227.2 мс | 2.20M ops/s | 0.45 мкс |
| SwissTable | 25.8 мс | 19.38M ops/s | 0.05 мкс |

# Lookup Zipf (str, N=500,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 9.3 мс | 53.49M ops/s | 0.02 мкс |
| HashMap | 14.0 мс | 35.71M ops/s | 0.03 мкс |
| RHMap | 12.1 мс | 41.44M ops/s | 0.02 мкс |
| BTHashMap | 25.7 мс | 19.49M ops/s | 0.05 мкс |
| SwissTable | 18.0 мс | 27.79M ops/s | 0.04 мкс |

# Update all (str, N=500,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 63.5 мс | 7.88M ops/s | 0.13 мкс |
| HashMap | 116.3 мс | 4.30M ops/s | 0.23 мкс |
| RHMap | 61.1 мс | 8.19M ops/s | 0.12 мкс |
| BTHashMap | 172.1 мс | 2.90M ops/s | 0.34 мкс |
| SwissTable | 66.2 мс | 7.55M ops/s | 0.13 мкс |

# Insert+Delete (str, N=500,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 138.6 мс | 7.22M ops/s | 0.14 мкс |
| HashMap | 427.7 мс | 2.34M ops/s | 0.43 мкс |
| RHMap | 167.4 мс | 5.97M ops/s | 0.17 мкс |
| BTHashMap | 551.5 мс | 1.81M ops/s | 0.55 мкс |
| SwissTable | 152.6 мс | 6.55M ops/s | 0.15 мкс |

# Mixed 80R/20W (str, N=500,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 107.9 мс | 4.64M ops/s | 0.22 мкс |
| HashMap | 153.4 мс | 3.26M ops/s | 0.31 мкс |
| RHMap | 87.2 мс | 5.74M ops/s | 0.17 мкс |
| BTHashMap | 216.1 мс | 2.31M ops/s | 0.43 мкс |
| SwissTable | 97.4 мс | 5.13M ops/s | 0.19 мкс |

# Latency percentiles (str, N=500,000)

| Структура | avg | p50 | p90 | p99 | p99.9 |
| --- | --- | --- | --- | --- | --- |
| dict | 0.39μs | 0.29μs | 0.54μs | 1.31μs | 11.98μs |
| HashMap | 0.48μs | 0.42μs | 0.62μs | 1.17μs | 8.74μs |
| RHMap | 0.27μs | 0.23μs | 0.30μs | 0.80μs | 2.06μs |
| BTHashMap | 0.53μs | 0.47μs | 0.69μs | 1.26μs | 8.95μs |
| SwissTable | 0.34μs | 0.27μs | 0.36μs | 0.95μs | 14.08μs |

# Insert rnd (str, N=500,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 124.4 мс | 4.02M ops/s | 0.25 мкс |
| HashMap | 603.7 мс | 828.3K ops/s | 1.21 мкс |
| RHMap | 158.5 мс | 3.15M ops/s | 0.32 мкс |
| BTHashMap | 453.2 мс | 1.10M ops/s | 0.91 мкс |
| SwissTable | 95.4 мс | 5.24M ops/s | 0.19 мкс |

# Lookup hit (str, N=500,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 172.2 мс | 2.90M ops/s | 0.34 мкс |
| HashMap | 235.9 мс | 2.12M ops/s | 0.47 мкс |
| RHMap | 130.4 мс | 3.83M ops/s | 0.26 мкс |
| BTHashMap | 312.1 мс | 1.60M ops/s | 0.62 мкс |
| SwissTable | 147.3 мс | 3.39M ops/s | 0.29 мкс |

# Lookup miss (str, N=500,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 85.6 мс | 5.84M ops/s | 0.17 мкс |
| HashMap | 95.9 мс | 5.22M ops/s | 0.19 мкс |
| RHMap | 62.2 мс | 8.04M ops/s | 0.12 мкс |
| BTHashMap | 253.9 мс | 1.97M ops/s | 0.51 мкс |
| SwissTable | 19.8 мс | 25.31M ops/s | 0.04 мкс |

# Lookup Zipf (str, N=500,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 10.5 мс | 47.40M ops/s | 0.02 мкс |
| HashMap | 12.7 мс | 39.25M ops/s | 0.03 мкс |
| RHMap | 12.8 мс | 39.03M ops/s | 0.03 мкс |
| BTHashMap | 27.8 мс | 17.99M ops/s | 0.06 мкс |
| SwissTable | 16.8 мс | 29.69M ops/s | 0.03 мкс |

# Update all (str, N=500,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 72.3 мс | 6.92M ops/s | 0.14 мкс |
| HashMap | 130.0 мс | 3.85M ops/s | 0.26 мкс |
| RHMap | 71.2 мс | 7.02M ops/s | 0.14 мкс |
| BTHashMap | 194.3 мс | 2.57M ops/s | 0.39 мкс |
| SwissTable | 60.5 мс | 8.26M ops/s | 0.12 мкс |

# Insert+Delete (str, N=500,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 135.7 мс | 7.37M ops/s | 0.14 мкс |
| HashMap | 457.8 мс | 2.18M ops/s | 0.46 мкс |
| RHMap | 186.1 мс | 5.37M ops/s | 0.19 мкс |
| BTHashMap | 646.5 мс | 1.55M ops/s | 0.65 мкс |
| SwissTable | 145.5 мс | 6.87M ops/s | 0.15 мкс |

# Mixed 80R/20W (str, N=500,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 135.0 мс | 3.70M ops/s | 0.27 мкс |
| HashMap | 178.9 мс | 2.80M ops/s | 0.36 мкс |
| RHMap | 99.2 мс | 5.04M ops/s | 0.20 мкс |
| BTHashMap | 238.8 мс | 2.09M ops/s | 0.48 мкс |
| SwissTable | 107.8 мс | 4.64M ops/s | 0.22 мкс |

# Latency percentiles (str, N=500,000)

| Структура | avg | p50 | p90 | p99 | p99.9 |
| --- | --- | --- | --- | --- | --- |
| dict | 0.43μs | 0.35μs | 0.58μs | 1.20μs | 11.83μs |
| HashMap | 0.58μs | 0.52μs | 0.74μs | 1.25μs | 9.97μs |
| RHMap | 0.30μs | 0.27μs | 0.36μs | 0.85μs | 1.42μs |
| BTHashMap | 0.66μs | 0.56μs | 0.82μs | 1.40μs | 22.94μs |
| SwissTable | 0.33μs | 0.28μs | 0.37μs | 0.92μs | 8.26μs |

