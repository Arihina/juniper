# Insert rnd (int, N=500,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 95.7 мс | 5.22M ops/s | 0.19 мкс |
| HashMap | 560.0 мс | 892.9K ops/s | 1.12 мкс |
| RHMap | 146.7 мс | 3.41M ops/s | 0.29 мкс |
| BTHashMap | 377.5 мс | 1.32M ops/s | 0.75 мкс |
| SwissTable | 163.6 мс | 3.06M ops/s | 0.33 мкс |

# Lookup hit (int, N=500,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 121.7 мс | 4.11M ops/s | 0.24 мкс |
| HashMap | 187.2 мс | 2.67M ops/s | 0.37 мкс |
| RHMap | 103.6 мс | 4.83M ops/s | 0.21 мкс |
| BTHashMap | 203.0 мс | 2.46M ops/s | 0.41 мкс |
| SwissTable | 118.6 мс | 4.21M ops/s | 0.24 мкс |

# Lookup miss (int, N=500,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 46.7 мс | 10.70M ops/s | 0.09 мкс |
| HashMap | 40.1 мс | 12.46M ops/s | 0.08 мкс |
| RHMap | 46.4 мс | 10.78M ops/s | 0.09 мкс |
| BTHashMap | 212.3 мс | 2.35M ops/s | 0.42 мкс |
| SwissTable | 111.7 мс | 4.48M ops/s | 0.22 мкс |

# Lookup Zipf (int, N=500,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 10.3 мс | 48.52M ops/s | 0.02 мкс |
| HashMap | 13.3 мс | 37.51M ops/s | 0.03 мкс |
| RHMap | 10.8 мс | 46.51M ops/s | 0.02 мкс |
| BTHashMap | 24.1 мс | 20.78M ops/s | 0.05 мкс |
| SwissTable | 18.8 мс | 26.65M ops/s | 0.04 мкс |

# Update all (int, N=500,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 64.7 мс | 7.73M ops/s | 0.13 мкс |
| HashMap | 129.7 мс | 3.86M ops/s | 0.26 мкс |
| RHMap | 93.1 мс | 5.37M ops/s | 0.19 мкс |
| BTHashMap | 173.5 мс | 2.88M ops/s | 0.35 мкс |
| SwissTable | 113.5 мс | 4.41M ops/s | 0.23 мкс |

# Insert+Delete (int, N=500,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 133.9 мс | 7.47M ops/s | 0.13 мкс |
| HashMap | 467.1 мс | 2.14M ops/s | 0.47 мкс |
| RHMap | 217.8 мс | 4.59M ops/s | 0.22 мкс |
| BTHashMap | 526.7 мс | 1.90M ops/s | 0.53 мкс |
| SwissTable | 263.0 мс | 3.80M ops/s | 0.26 мкс |

# Mixed 80R/20W (int, N=500,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 101.4 мс | 4.93M ops/s | 0.20 мкс |
| HashMap | 159.2 мс | 3.14M ops/s | 0.32 мкс |
| RHMap | 112.2 мс | 4.45M ops/s | 0.22 мкс |
| BTHashMap | 200.8 мс | 2.49M ops/s | 0.40 мкс |
| SwissTable | 132.2 мс | 3.78M ops/s | 0.26 мкс |

# Latency percentiles (int, N=500,000)

| Структура | avg | p50 | p90 | p99 | p99.9 |
| --- | --- | --- | --- | --- | --- |
| dict | 0.34μs | 0.29μs | 0.46μs | 0.94μs | 9.54μs |
| HashMap | 0.56μs | 0.43μs | 0.98μs | 1.81μs | 11.45μs |
| RHMap | 0.26μs | 0.22μs | 0.29μs | 0.79μs | 1.21μs |
| BTHashMap | 0.44μs | 0.39μs | 0.52μs | 1.01μs | 2.14μs |
| SwissTable | 0.30μs | 0.26μs | 0.33μs | 0.85μs | 10.05μs |

# Insert rnd (int, N=500,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 114.2 мс | 4.38M ops/s | 0.23 мкс |
| HashMap | 628.4 мс | 795.7K ops/s | 1.26 мкс |
| RHMap | 137.9 мс | 3.63M ops/s | 0.28 мкс |
| BTHashMap | 379.1 мс | 1.32M ops/s | 0.76 мкс |
| SwissTable | 156.4 мс | 3.20M ops/s | 0.31 мкс |

# Lookup hit (int, N=500,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 132.8 мс | 3.77M ops/s | 0.27 мкс |
| HashMap | 213.0 мс | 2.35M ops/s | 0.43 мкс |
| RHMap | 112.5 мс | 4.45M ops/s | 0.22 мкс |
| BTHashMap | 214.5 мс | 2.33M ops/s | 0.43 мкс |
| SwissTable | 134.2 мс | 3.73M ops/s | 0.27 мкс |

# Lookup miss (int, N=500,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 55.2 мс | 9.06M ops/s | 0.11 мкс |
| HashMap | 45.2 мс | 11.06M ops/s | 0.09 мкс |
| RHMap | 44.5 мс | 11.25M ops/s | 0.09 мкс |
| BTHashMap | 212.6 мс | 2.35M ops/s | 0.43 мкс |
| SwissTable | 113.5 мс | 4.41M ops/s | 0.23 мкс |

# Lookup Zipf (int, N=500,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 10.4 мс | 47.96M ops/s | 0.02 мкс |
| HashMap | 14.5 мс | 34.40M ops/s | 0.03 мкс |
| RHMap | 10.5 мс | 47.40M ops/s | 0.02 мкс |
| BTHashMap | 19.0 мс | 26.30M ops/s | 0.04 мкс |
| SwissTable | 14.2 мс | 35.20M ops/s | 0.03 мкс |

# Update all (int, N=500,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 79.1 мс | 6.32M ops/s | 0.16 мкс |
| HashMap | 140.2 мс | 3.57M ops/s | 0.28 мкс |
| RHMap | 94.8 мс | 5.27M ops/s | 0.19 мкс |
| BTHashMap | 176.6 мс | 2.83M ops/s | 0.35 мкс |
| SwissTable | 118.4 мс | 4.22M ops/s | 0.24 мкс |

# Insert+Delete (int, N=500,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 156.3 мс | 6.40M ops/s | 0.16 мкс |
| HashMap | 497.0 мс | 2.01M ops/s | 0.50 мкс |
| RHMap | 239.2 мс | 4.18M ops/s | 0.24 мкс |
| BTHashMap | 585.5 мс | 1.71M ops/s | 0.59 мкс |
| SwissTable | 245.7 мс | 4.07M ops/s | 0.25 мкс |

# Mixed 80R/20W (int, N=500,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 122.8 мс | 4.07M ops/s | 0.25 мкс |
| HashMap | 175.1 мс | 2.86M ops/s | 0.35 мкс |
| RHMap | 119.4 мс | 4.19M ops/s | 0.24 мкс |
| BTHashMap | 235.2 мс | 2.13M ops/s | 0.47 мкс |
| SwissTable | 139.6 мс | 3.58M ops/s | 0.28 мкс |

# Latency percentiles (int, N=500,000)

| Структура | avg | p50 | p90 | p99 | p99.9 |
| --- | --- | --- | --- | --- | --- |
| dict | 0.46μs | 0.35μs | 0.50μs | 1.01μs | 7.54μs |
| HashMap | 0.49μs | 0.43μs | 0.65μs | 1.10μs | 2.08μs |
| RHMap | 0.35μs | 0.29μs | 0.46μs | 0.87μs | 14.94μs |
| BTHashMap | 0.51μs | 0.45μs | 0.63μs | 1.17μs | 10.39μs |
| SwissTable | 0.36μs | 0.32μs | 0.43μs | 0.93μs | 2.33μs |

# Insert rnd (int, N=500,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 97.0 мс | 5.16M ops/s | 0.19 мкс |
| HashMap | 528.9 мс | 945.3K ops/s | 1.06 мкс |
| RHMap | 121.3 мс | 4.12M ops/s | 0.24 мкс |
| BTHashMap | 322.5 мс | 1.55M ops/s | 0.64 мкс |
| SwissTable | 133.4 мс | 3.75M ops/s | 0.27 мкс |

# Lookup hit (int, N=500,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 118.5 мс | 4.22M ops/s | 0.24 мкс |
| HashMap | 184.2 мс | 2.71M ops/s | 0.37 мкс |
| RHMap | 110.2 мс | 4.54M ops/s | 0.22 мкс |
| BTHashMap | 207.0 мс | 2.42M ops/s | 0.41 мкс |
| SwissTable | 114.4 мс | 4.37M ops/s | 0.23 мкс |

# Lookup miss (int, N=500,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 42.9 мс | 11.64M ops/s | 0.09 мкс |
| HashMap | 49.8 мс | 10.04M ops/s | 0.10 мкс |
| RHMap | 37.3 мс | 13.40M ops/s | 0.07 мкс |
| BTHashMap | 193.1 мс | 2.59M ops/s | 0.39 мкс |
| SwissTable | 103.1 мс | 4.85M ops/s | 0.21 мкс |

# Lookup Zipf (int, N=500,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 10.4 мс | 48.17M ops/s | 0.02 мкс |
| HashMap | 12.8 мс | 39.19M ops/s | 0.03 мкс |
| RHMap | 10.6 мс | 47.32M ops/s | 0.02 мкс |
| BTHashMap | 21.3 мс | 23.51M ops/s | 0.04 мкс |
| SwissTable | 14.0 мс | 35.78M ops/s | 0.03 мкс |

# Insert rnd (int, N=500,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 97.6 мс | 5.12M ops/s | 0.20 мкс |
| RHMap | 131.9 мс | 3.79M ops/s | 0.26 мкс |
| SwissTable | 148.4 мс | 3.37M ops/s | 0.30 мкс |

# Lookup hit (int, N=500,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 120.8 мс | 4.14M ops/s | 0.24 мкс |
| RHMap | 103.9 мс | 4.81M ops/s | 0.21 мкс |
| SwissTable | 115.3 мс | 4.34M ops/s | 0.23 мкс |

# Lookup miss (int, N=500,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 44.3 мс | 11.29M ops/s | 0.09 мкс |
| RHMap | 39.6 мс | 12.63M ops/s | 0.08 мкс |
| SwissTable | 104.9 мс | 4.77M ops/s | 0.21 мкс |

# Lookup Zipf (int, N=500,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 10.0 мс | 50.04M ops/s | 0.02 мкс |
| RHMap | 10.3 мс | 48.33M ops/s | 0.02 мкс |
| SwissTable | 15.0 мс | 33.29M ops/s | 0.03 мкс |

# Update all (int, N=500,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 57.4 мс | 8.72M ops/s | 0.11 мкс |
| RHMap | 92.9 мс | 5.38M ops/s | 0.19 мкс |
| SwissTable | 112.0 мс | 4.46M ops/s | 0.22 мкс |

# Insert+Delete (int, N=500,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 129.0 мс | 7.75M ops/s | 0.13 мкс |
| RHMap | 190.4 мс | 5.25M ops/s | 0.19 мкс |
| SwissTable | 226.2 мс | 4.42M ops/s | 0.23 мкс |

# Mixed 80R/20W (int, N=500,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 89.6 мс | 5.58M ops/s | 0.18 мкс |
| RHMap | 107.3 мс | 4.66M ops/s | 0.21 мкс |
| SwissTable | 126.1 мс | 3.96M ops/s | 0.25 мкс |

# Latency percentiles (int, N=500,000)

| Структура | avg | p50 | p90 | p99 | p99.9 |
| --- | --- | --- | --- | --- | --- |
| dict | 0.46μs | 0.39μs | 0.59μs | 1.17μs | 15.32μs |
| RHMap | 0.29μs | 0.25μs | 0.34μs | 0.85μs | 10.40μs |
| SwissTable | 0.31μs | 0.27μs | 0.37μs | 0.91μs | 8.67μs |

