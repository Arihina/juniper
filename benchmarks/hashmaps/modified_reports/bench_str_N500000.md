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

