# push_back ×N (N=1,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| list (builtin) | 0.2 мс | 4.91M ops/s | 0.20 мкс |
| SList | 0.2 мс | 6.19M ops/s | 0.16 мкс |
| DList | 0.1 мс | 9.99M ops/s | 0.10 мкс |

# push_front ×N (N=1,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| list (builtin) | 0.2 мс | 4.02M ops/s | 0.25 мкс |
| SList | 0.1 мс | 7.05M ops/s | 0.14 мкс |
| DList | 0.1 мс | 9.94M ops/s | 0.10 мкс |

# pop_front ×N (N=1,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| list (builtin) | 0.1 мс | 13.58M ops/s | 0.07 мкс |
| SList | 0.1 мс | 10.65M ops/s | 0.09 мкс |
| DList | 0.1 мс | 14.18M ops/s | 0.07 мкс |

# pop_back ×N (N=1,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| list (builtin) | 0.0 мс | 31.32M ops/s | 0.03 мкс |
| SList | 1.4 мс | 717.2K ops/s | 1.39 мкс |
| DList | 0.0 мс | 20.05M ops/s | 0.05 мкс |

# Iteration (N=1,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| list (builtin) | 0.0 мс | 172.27M ops/s | 0.01 мкс |
| SList | 0.0 мс | 100.58M ops/s | 0.01 мкс |
| DList | 0.0 мс | 133.10M ops/s | 0.01 мкс |

# Reverse (N=1,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| list (builtin) | 0.0 мс | 82.91M ops/s | 0.01 мкс |
| SList | 0.0 мс | 28.56M ops/s | 0.04 мкс |
| DList | 0.0 мс | 39.52M ops/s | 0.03 мкс |

# From iterable (N=1,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| list (builtin) | 0.0 мс | 46.46M ops/s | 0.02 мкс |
| SList | 0.1 мс | 14.05M ops/s | 0.07 мкс |
| DList | 0.0 мс | 40.37M ops/s | 0.02 мкс |

# Contains linear scan ×1000 (N=1,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| list (builtin) | 2.6 мс | 381.1K ops/s | 2.62 мкс |
| SList | 3.0 мс | 333.3K ops/s | 3.00 мкс |
| DList | 3.2 мс | 315.7K ops/s | 3.17 мкс |

# Index access sequential (N=1,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| list (builtin) | 0.0 мс | 6.44M ops/s | 0.16 мкс |
| SList | 0.0 мс | 1.21M ops/s | 0.83 мкс |
| DList | 0.0 мс | 2.31M ops/s | 0.43 мкс |

# push_back ×N (N=10,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| list (builtin) | 0.4 мс | 26.29M ops/s | 0.04 мкс |
| SList | 0.8 мс | 12.67M ops/s | 0.08 мкс |
| DList | 0.7 мс | 14.61M ops/s | 0.07 мкс |

# push_front ×N (N=10,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| list (builtin) | 10.3 мс | 966.7K ops/s | 1.03 мкс |
| SList | 0.8 мс | 12.73M ops/s | 0.08 мкс |
| DList | 0.6 мс | 16.13M ops/s | 0.06 мкс |

# pop_front ×N (N=10,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| list (builtin) | 5.3 мс | 1.89M ops/s | 0.53 мкс |
| SList | 0.6 мс | 17.38M ops/s | 0.06 мкс |
| DList | 0.5 мс | 19.47M ops/s | 0.05 мкс |

# pop_back ×N (N=10,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| list (builtin) | 0.4 мс | 22.81M ops/s | 0.04 мкс |
| SList | 83.6 мс | 119.7K ops/s | 8.36 мкс |
| DList | 0.7 мс | 14.49M ops/s | 0.07 мкс |

# Iteration (N=10,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| list (builtin) | 0.1 мс | 190.03M ops/s | 0.01 мкс |
| SList | 0.1 мс | 167.01M ops/s | 0.01 мкс |
| DList | 0.1 мс | 130.21M ops/s | 0.01 мкс |

# Reverse (N=10,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| list (builtin) | 0.1 мс | 122.81M ops/s | 0.01 мкс |
| SList | 0.3 мс | 32.92M ops/s | 0.03 мкс |
| DList | 0.3 мс | 39.70M ops/s | 0.03 мкс |

# From iterable (N=10,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| list (builtin) | 0.1 мс | 120.79M ops/s | 0.01 мкс |
| SList | 0.3 мс | 34.67M ops/s | 0.03 мкс |
| DList | 0.2 мс | 44.61M ops/s | 0.02 мкс |

# Contains linear scan ×1000 (N=10,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| list (builtin) | 22.3 мс | 44.8K ops/s | 22.34 мкс |
| SList | 29.0 мс | 34.4K ops/s | 29.04 мкс |
| DList | 28.6 мс | 35.0K ops/s | 28.59 мкс |

# Index access sequential (N=10,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| list (builtin) | 0.0 мс | 43.82M ops/s | 0.02 мкс |
| SList | 0.8 мс | 124.1K ops/s | 8.06 мкс |
| DList | 0.5 мс | 210.5K ops/s | 4.75 мкс |

# push_back ×N (N=50,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| list (builtin) | 1.4 мс | 35.54M ops/s | 0.03 мкс |
| SList | 4.1 мс | 12.05M ops/s | 0.08 мкс |
| DList | 3.9 мс | 12.99M ops/s | 0.08 мкс |

# push_front ×N (N=50,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| list (builtin) | 255.1 мс | 196.0K ops/s | 5.10 мкс |
| SList | 3.8 мс | 13.11M ops/s | 0.08 мкс |
| DList | 3.2 мс | 15.84M ops/s | 0.06 мкс |

# pop_front ×N (N=50,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| list (builtin) | 171.9 мс | 290.9K ops/s | 3.44 мкс |
| SList | 2.8 мс | 17.96M ops/s | 0.06 мкс |
| DList | 2.3 мс | 21.36M ops/s | 0.05 мкс |

# pop_back ×N (N=50,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| list (builtin) | 1.4 мс | 34.60M ops/s | 0.03 мкс |
| SList | 1973.0 мс | 25.3K ops/s | 39.46 мкс |
| DList | 2.5 мс | 20.22M ops/s | 0.05 мкс |

# Iteration (N=50,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| list (builtin) | 0.3 мс | 191.60M ops/s | 0.01 мкс |
| SList | 0.3 мс | 168.48M ops/s | 0.01 мкс |
| DList | 0.3 мс | 178.67M ops/s | 0.01 мкс |

# Reverse (N=50,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| list (builtin) | 0.4 мс | 119.00M ops/s | 0.01 мкс |
| SList | 1.4 мс | 35.13M ops/s | 0.03 мкс |
| DList | 1.4 мс | 35.43M ops/s | 0.03 мкс |

# From iterable (N=50,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| list (builtin) | 0.4 мс | 117.14M ops/s | 0.01 мкс |
| SList | 1.4 мс | 35.87M ops/s | 0.03 мкс |
| DList | 1.2 мс | 43.28M ops/s | 0.02 мкс |

# Contains linear scan ×1000 (N=50,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| list (builtin) | 112.6 мс | 8.9K ops/s | 112.58 мкс |
| SList | 140.6 мс | 7.1K ops/s | 140.57 мкс |
| DList | 148.4 мс | 6.7K ops/s | 148.43 мкс |

# Index access sequential (N=50,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| list (builtin) | 0.0 мс | 15.41M ops/s | 0.06 мкс |
| SList | 18.9 мс | 26.4K ops/s | 37.88 мкс |
| DList | 14.0 мс | 35.7K ops/s | 28.03 мкс |

