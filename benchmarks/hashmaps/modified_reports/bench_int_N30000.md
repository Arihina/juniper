# Insert rnd (int, N=30,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 2.5 мс | 11.96M ops/s | 0.08 мкс |
| HashMap | 11.2 мс | 2.67M ops/s | 0.37 мкс |
| RHMap | 9.5 мс | 3.15M ops/s | 0.32 мкс |
| BTHashMap | 8.8 мс | 3.41M ops/s | 0.29 мкс |
| SwissTable | 6.9 мс | 4.37M ops/s | 0.23 мкс |

# Lookup hit (int, N=30,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 2.6 мс | 11.57M ops/s | 0.09 мкс |
| HashMap | 5.8 мс | 5.14M ops/s | 0.19 мкс |
| RHMap | 2.4 мс | 12.67M ops/s | 0.08 мкс |
| BTHashMap | 2.8 мс | 10.58M ops/s | 0.09 мкс |
| SwissTable | 3.1 мс | 9.79M ops/s | 0.10 мкс |

# Lookup miss (int, N=30,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 1.0 мс | 29.79M ops/s | 0.03 мкс |
| HashMap | 1.2 мс | 25.48M ops/s | 0.04 мкс |
| RHMap | 1.2 мс | 24.34M ops/s | 0.04 мкс |
| BTHashMap | 3.8 мс | 7.80M ops/s | 0.13 мкс |
| SwissTable | 2.5 мс | 12.20M ops/s | 0.08 мкс |

# Lookup Zipf (int, N=30,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 0.7 мс | 42.45M ops/s | 0.02 мкс |
| HashMap | 0.8 мс | 37.22M ops/s | 0.03 мкс |
| RHMap | 0.6 мс | 50.11M ops/s | 0.02 мкс |
| BTHashMap | 1.9 мс | 15.59M ops/s | 0.06 мкс |
| SwissTable | 1.1 мс | 27.76M ops/s | 0.04 мкс |

# Update all (int, N=30,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 1.1 мс | 27.61M ops/s | 0.04 мкс |
| HashMap | 2.8 мс | 10.61M ops/s | 0.09 мкс |
| RHMap | 1.5 мс | 19.88M ops/s | 0.05 мкс |
| BTHashMap | 3.2 мс | 9.47M ops/s | 0.11 мкс |
| SwissTable | 2.2 мс | 13.53M ops/s | 0.07 мкс |

# Insert+Delete (int, N=30,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 2.6 мс | 23.24M ops/s | 0.04 мкс |
| HashMap | 13.6 мс | 4.42M ops/s | 0.23 мкс |
| RHMap | 3.7 мс | 16.34M ops/s | 0.06 мкс |
| BTHashMap | 15.8 мс | 3.79M ops/s | 0.26 мкс |
| SwissTable | 8.1 мс | 7.41M ops/s | 0.13 мкс |

# Mixed 80R/20W (int, N=30,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 2.9 мс | 10.26M ops/s | 0.10 мкс |
| HashMap | 5.6 мс | 5.33M ops/s | 0.19 мкс |
| RHMap | 2.9 мс | 10.52M ops/s | 0.10 мкс |
| BTHashMap | 10.4 мс | 2.89M ops/s | 0.35 мкс |
| SwissTable | 3.3 мс | 9.08M ops/s | 0.11 мкс |

# Latency percentiles (int, N=30,000)

| Структура | avg | p50 | p90 | p99 | p99.9 |
| --- | --- | --- | --- | --- | --- |
| dict | 0.20μs | 0.19μs | 0.25μs | 0.71μs | 1.13μs |
| HashMap | 0.35μs | 0.31μs | 0.47μs | 1.02μs | 8.97μs |
| RHMap | 0.15μs | 0.14μs | 0.19μs | 0.67μs | 1.40μs |
| BTHashMap | 0.28μs | 0.23μs | 0.46μs | 0.96μs | 2.25μs |
| SwissTable | 0.19μs | 0.17μs | 0.23μs | 0.67μs | 1.36μs |

# Insert rnd (int, N=30,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 4.8 мс | 6.24M ops/s | 0.16 мкс |
| HashMap | 15.1 мс | 1.99M ops/s | 0.50 мкс |
| RHMap | 5.3 мс | 5.62M ops/s | 0.18 мкс |
| BTHashMap | 8.8 мс | 3.43M ops/s | 0.29 мкс |
| SwissTable | 4.3 мс | 6.95M ops/s | 0.14 мкс |

# Lookup hit (int, N=30,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 2.7 мс | 10.99M ops/s | 0.09 мкс |
| HashMap | 5.8 мс | 5.13M ops/s | 0.19 мкс |
| RHMap | 2.8 мс | 10.85M ops/s | 0.09 мкс |
| BTHashMap | 4.2 мс | 7.18M ops/s | 0.14 мкс |
| SwissTable | 2.5 мс | 12.06M ops/s | 0.08 мкс |

# Lookup miss (int, N=30,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 1.5 мс | 19.39M ops/s | 0.05 мкс |
| HashMap | 1.1 мс | 28.32M ops/s | 0.04 мкс |
| RHMap | 1.9 мс | 15.59M ops/s | 0.06 мкс |
| BTHashMap | 5.4 мс | 5.56M ops/s | 0.18 мкс |
| SwissTable | 2.0 мс | 15.30M ops/s | 0.07 мкс |

# Lookup Zipf (int, N=30,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 0.6 мс | 47.55M ops/s | 0.02 мкс |
| HashMap | 0.8 мс | 38.59M ops/s | 0.03 мкс |
| RHMap | 0.7 мс | 42.15M ops/s | 0.02 мкс |
| BTHashMap | 1.1 мс | 26.15M ops/s | 0.04 мкс |
| SwissTable | 0.9 мс | 32.67M ops/s | 0.03 мкс |

# Update all (int, N=30,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 1.3 мс | 23.41M ops/s | 0.04 мкс |
| HashMap | 10.5 мс | 2.86M ops/s | 0.35 мкс |
| RHMap | 1.2 мс | 25.59M ops/s | 0.04 мкс |
| BTHashMap | 4.4 мс | 6.78M ops/s | 0.15 мкс |
| SwissTable | 2.5 мс | 12.09M ops/s | 0.08 мкс |

# Insert+Delete (int, N=30,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 2.9 мс | 20.62M ops/s | 0.05 мкс |
| HashMap | 16.5 мс | 3.64M ops/s | 0.27 мкс |
| RHMap | 6.5 мс | 9.20M ops/s | 0.11 мкс |
| BTHashMap | 18.0 мс | 3.33M ops/s | 0.30 мкс |
| SwissTable | 8.6 мс | 6.97M ops/s | 0.14 мкс |

# Mixed 80R/20W (int, N=30,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 4.7 мс | 6.33M ops/s | 0.16 мкс |
| HashMap | 12.3 мс | 2.45M ops/s | 0.41 мкс |
| RHMap | 3.6 мс | 8.23M ops/s | 0.12 мкс |
| BTHashMap | 10.2 мс | 2.93M ops/s | 0.34 мкс |
| SwissTable | 4.4 мс | 6.84M ops/s | 0.15 мкс |

# Latency percentiles (int, N=30,000)

| Структура | avg | p50 | p90 | p99 | p99.9 |
| --- | --- | --- | --- | --- | --- |
| dict | 0.21μs | 0.17μs | 0.26μs | 0.73μs | 2.02μs |
| HashMap | 0.32μs | 0.29μs | 0.41μs | 0.91μs | 2.87μs |
| RHMap | 0.20μs | 0.16μs | 0.23μs | 0.70μs | 11.97μs |
| BTHashMap | 0.45μs | 0.35μs | 0.53μs | 1.13μs | 39.97μs |
| SwissTable | 0.33μs | 0.28μs | 0.43μs | 1.02μs | 14.46μs |

