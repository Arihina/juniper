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

