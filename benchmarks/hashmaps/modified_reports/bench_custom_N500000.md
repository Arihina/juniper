# Insert rnd (Key/Value, N=500,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 131.9 мс | 3.79M ops/s | 0.26 мкс |
| HashMap | 645.2 мс | 774.9K ops/s | 1.29 мкс |
| RHMap | 207.3 мс | 2.41M ops/s | 0.41 мкс |
| BTHashMap | 781.5 мс | 639.8K ops/s | 1.56 мкс |
| SwissTable | 137.8 мс | 3.63M ops/s | 0.28 мкс |

# Lookup hit (Key/Value, N=500,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 182.5 мс | 2.74M ops/s | 0.37 мкс |
| HashMap | 279.7 мс | 1.79M ops/s | 0.56 мкс |
| RHMap | 174.3 мс | 2.87M ops/s | 0.35 мкс |
| BTHashMap | 381.7 мс | 1.31M ops/s | 0.76 мкс |
| SwissTable | 191.6 мс | 2.61M ops/s | 0.38 мкс |

# Lookup miss (Key/Value, N=500,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 84.4 мс | 5.93M ops/s | 0.17 мкс |
| HashMap | 111.8 мс | 4.47M ops/s | 0.22 мкс |
| RHMap | 88.8 мс | 5.63M ops/s | 0.18 мкс |
| BTHashMap | 405.3 мс | 1.23M ops/s | 0.81 мкс |
| SwissTable | 55.1 мс | 9.07M ops/s | 0.11 мкс |

# Lookup Zipf (Key/Value, N=500,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 40.6 мс | 12.31M ops/s | 0.08 мкс |
| HashMap | 42.4 мс | 11.79M ops/s | 0.08 мкс |
| RHMap | 40.9 мс | 12.21M ops/s | 0.08 мкс |
| BTHashMap | 122.4 мс | 4.09M ops/s | 0.24 мкс |
| SwissTable | 51.5 мс | 9.72M ops/s | 0.10 мкс |

# Update all (Key/Value, N=500,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 119.6 мс | 4.18M ops/s | 0.24 мкс |
| HashMap | 197.4 мс | 2.53M ops/s | 0.39 мкс |
| RHMap | 106.7 мс | 4.68M ops/s | 0.21 мкс |
| BTHashMap | 311.9 мс | 1.60M ops/s | 0.62 мкс |
| SwissTable | 132.1 мс | 3.79M ops/s | 0.26 мкс |

# Insert+Delete (Key/Value, N=500,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 175.9 мс | 5.68M ops/s | 0.18 мкс |
| HashMap | 527.6 мс | 1.90M ops/s | 0.53 мкс |
| RHMap | 262.7 мс | 3.81M ops/s | 0.26 мкс |
| BTHashMap | 1051.9 мс | 950.7K ops/s | 1.05 мкс |
| SwissTable | 246.8 мс | 4.05M ops/s | 0.25 мкс |

# Mixed 80R/20W (Key/Value, N=500,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 148.4 мс | 3.37M ops/s | 0.30 мкс |
| HashMap | 185.0 мс | 2.70M ops/s | 0.37 мкс |
| RHMap | 112.8 мс | 4.43M ops/s | 0.23 мкс |
| BTHashMap | 379.1 мс | 1.32M ops/s | 0.76 мкс |
| SwissTable | 168.9 мс | 2.96M ops/s | 0.34 мкс |

# Latency percentiles (Key/Value, N=500,000)

| Структура | avg | p50 | p90 | p99 | p99.9 |
| --- | --- | --- | --- | --- | --- |
| dict | 0.49μs | 0.44μs | 0.66μs | 1.12μs | 2.05μs |
| HashMap | 0.58μs | 0.53μs | 0.67μs | 1.18μs | 9.77μs |
| RHMap | 0.50μs | 0.41μs | 0.77μs | 1.25μs | 7.63μs |
| BTHashMap | 0.84μs | 0.77μs | 1.14μs | 1.60μs | 15.48μs |
| SwissTable | 0.43μs | 0.40μs | 0.48μs | 0.97μs | 1.77μs |

# Insert rnd (Key/Value, N=500,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 132.9 мс | 3.76M ops/s | 0.27 мкс |
| HashMap | 660.0 мс | 757.6K ops/s | 1.32 мкс |
| RHMap | 187.6 мс | 2.67M ops/s | 0.38 мкс |
| BTHashMap | 850.7 мс | 587.7K ops/s | 1.70 мкс |
| SwissTable | 134.5 мс | 3.72M ops/s | 0.27 мкс |

# Lookup hit (Key/Value, N=500,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 242.7 мс | 2.06M ops/s | 0.49 мкс |
| HashMap | 363.2 мс | 1.38M ops/s | 0.73 мкс |
| RHMap | 192.7 мс | 2.60M ops/s | 0.39 мкс |
| BTHashMap | 548.1 мс | 912.2K ops/s | 1.10 мкс |
| SwissTable | 194.1 мс | 2.58M ops/s | 0.39 мкс |

# Lookup miss (Key/Value, N=500,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 91.3 мс | 5.48M ops/s | 0.18 мкс |
| HashMap | 138.3 мс | 3.61M ops/s | 0.28 мкс |
| RHMap | 111.4 мс | 4.49M ops/s | 0.22 мкс |
| BTHashMap | 603.4 мс | 828.6K ops/s | 1.21 мкс |
| SwissTable | 55.0 мс | 9.09M ops/s | 0.11 мкс |

# Lookup Zipf (Key/Value, N=500,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 40.5 мс | 12.33M ops/s | 0.08 мкс |
| HashMap | 32.7 мс | 15.27M ops/s | 0.07 мкс |
| RHMap | 28.7 мс | 17.42M ops/s | 0.06 мкс |
| BTHashMap | 96.4 мс | 5.19M ops/s | 0.19 мкс |
| SwissTable | 31.3 мс | 15.95M ops/s | 0.06 мкс |

# Update all (Key/Value, N=500,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 144.1 мс | 3.47M ops/s | 0.29 мкс |
| HashMap | 211.2 мс | 2.37M ops/s | 0.42 мкс |
| RHMap | 118.1 мс | 4.23M ops/s | 0.24 мкс |
| BTHashMap | 405.0 мс | 1.23M ops/s | 0.81 мкс |
| SwissTable | 122.8 мс | 4.07M ops/s | 0.25 мкс |

# Insert+Delete (Key/Value, N=500,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 212.0 мс | 4.72M ops/s | 0.21 мкс |
| HashMap | 605.0 мс | 1.65M ops/s | 0.61 мкс |
| RHMap | 323.8 мс | 3.09M ops/s | 0.32 мкс |
| BTHashMap | 1166.2 мс | 857.5K ops/s | 1.17 мкс |
| SwissTable | 263.4 мс | 3.80M ops/s | 0.26 мкс |

# Mixed 80R/20W (Key/Value, N=500,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 167.8 мс | 2.98M ops/s | 0.34 мкс |
| HashMap | 217.5 мс | 2.30M ops/s | 0.44 мкс |
| RHMap | 134.3 мс | 3.72M ops/s | 0.27 мкс |
| BTHashMap | 418.8 мс | 1.19M ops/s | 0.84 мкс |
| SwissTable | 132.7 мс | 3.77M ops/s | 0.27 мкс |

# Latency percentiles (Key/Value, N=500,000)

| Структура | avg | p50 | p90 | p99 | p99.9 |
| --- | --- | --- | --- | --- | --- |
| dict | 0.59μs | 0.52μs | 0.82μs | 1.34μs | 2.09μs |
| HashMap | 0.83μs | 0.70μs | 1.06μs | 1.63μs | 17.87μs |
| RHMap | 0.69μs | 0.60μs | 0.96μs | 1.49μs | 13.99μs |
| BTHashMap | 1.08μs | 0.98μs | 1.48μs | 1.99μs | 13.77μs |
| SwissTable | 0.47μs | 0.43μs | 0.52μs | 1.04μs | 7.66μs |

# Insert rnd (Key/Value, N=500,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 121.5 мс | 4.11M ops/s | 0.24 мкс |
| RHMap | 159.8 мс | 3.13M ops/s | 0.32 мкс |
| SwissTable | 120.8 мс | 4.14M ops/s | 0.24 мкс |

# Lookup hit (Key/Value, N=500,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 208.5 мс | 2.40M ops/s | 0.42 мкс |
| RHMap | 174.9 мс | 2.86M ops/s | 0.35 мкс |
| SwissTable | 181.5 мс | 2.75M ops/s | 0.36 мкс |

# Lookup miss (Key/Value, N=500,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 71.9 мс | 6.96M ops/s | 0.14 мкс |
| RHMap | 85.0 мс | 5.88M ops/s | 0.17 мкс |
| SwissTable | 51.5 мс | 9.71M ops/s | 0.10 мкс |

# Lookup Zipf (Key/Value, N=500,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 36.1 мс | 13.87M ops/s | 0.07 мкс |
| RHMap | 36.1 мс | 13.84M ops/s | 0.07 мкс |
| SwissTable | 39.0 мс | 12.82M ops/s | 0.08 мкс |

# Update all (Key/Value, N=500,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 102.7 мс | 4.87M ops/s | 0.21 мкс |
| RHMap | 117.2 мс | 4.27M ops/s | 0.23 мкс |
| SwissTable | 120.6 мс | 4.14M ops/s | 0.24 мкс |

# Insert+Delete (Key/Value, N=500,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 156.7 мс | 6.38M ops/s | 0.16 мкс |
| RHMap | 235.6 мс | 4.25M ops/s | 0.24 мкс |
| SwissTable | 218.2 мс | 4.58M ops/s | 0.22 мкс |

# Mixed 80R/20W (Key/Value, N=500,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 117.8 мс | 4.25M ops/s | 0.24 мкс |
| RHMap | 136.0 мс | 3.68M ops/s | 0.27 мкс |
| SwissTable | 131.4 мс | 3.80M ops/s | 0.26 мкс |

# Latency percentiles (Key/Value, N=500,000)

| Структура | avg | p50 | p90 | p99 | p99.9 |
| --- | --- | --- | --- | --- | --- |
| dict | 0.50μs | 0.45μs | 0.63μs | 1.10μs | 2.10μs |
| RHMap | 0.44μs | 0.41μs | 0.48μs | 1.00μs | 2.54μs |
| SwissTable | 0.45μs | 0.42μs | 0.50μs | 1.01μs | 2.04μs |

