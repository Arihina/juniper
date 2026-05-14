# Seq insert (from range) (N=1,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| RBSet | 0.3 мс | 4.00M ops/s | 0.25 мкс |
| AVLSet | 0.2 мс | 5.38M ops/s | 0.19 мкс |
| BTreeSet | 0.1 мс | 7.01M ops/s | 0.14 мкс |
| BSTSet | 5.8 мс | 172.5K ops/s | 5.80 мкс |
| SkipListSet | 0.2 мс | 5.63M ops/s | 0.18 мкс |

# Rnd insert (from list) (N=1,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| RBSet | 0.2 мс | 4.74M ops/s | 0.21 мкс |
| AVLSet | 0.2 мс | 5.53M ops/s | 0.18 мкс |
| BTreeSet | 0.1 мс | 6.69M ops/s | 0.15 мкс |
| BSTSet | 0.2 мс | 5.65M ops/s | 0.18 мкс |
| SkipListSet | 0.3 мс | 3.95M ops/s | 0.25 мкс |

# Rnd insert (one-by-one) (N=1,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| RBSet | 0.2 мс | 4.20M ops/s | 0.24 мкс |
| AVLSet | 0.2 мс | 4.38M ops/s | 0.23 мкс |
| BTreeSet | 0.2 мс | 4.98M ops/s | 0.20 мкс |
| BSTSet | 0.3 мс | 3.74M ops/s | 0.27 мкс |
| SkipListSet | 0.3 мс | 3.38M ops/s | 0.30 мкс |

# Contains (100% hit) (N=1,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| RBSet | 0.1 мс | 7.63M ops/s | 0.13 мкс |
| AVLSet | 0.2 мс | 6.28M ops/s | 0.16 мкс |
| BTreeSet | 0.1 мс | 7.50M ops/s | 0.13 мкс |
| BSTSet | 0.2 мс | 6.01M ops/s | 0.17 мкс |
| SkipListSet | 0.2 мс | 5.59M ops/s | 0.18 мкс |

# Contains (100% miss) (N=1,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| RBSet | 0.1 мс | 7.47M ops/s | 0.13 мкс |
| AVLSet | 0.1 мс | 7.22M ops/s | 0.14 мкс |
| BTreeSet | 0.1 мс | 8.51M ops/s | 0.12 мкс |
| BSTSet | 0.1 мс | 7.24M ops/s | 0.14 мкс |
| SkipListSet | 0.1 мс | 8.45M ops/s | 0.12 мкс |

# Iteration (full scan) (N=1,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| RBSet | 0.0 мс | 42.74M ops/s | 0.02 мкс |
| AVLSet | 0.0 мс | 60.10M ops/s | 0.02 мкс |
| BTreeSet | 0.0 мс | 71.32M ops/s | 0.01 мкс |
| BSTSet | 0.0 мс | 41.38M ops/s | 0.02 мкс |
| SkipListSet | 0.0 мс | 86.34M ops/s | 0.01 мкс |

# Discard all (N=1,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| RBSet | 0.4 мс | 2.56M ops/s | 0.39 мкс |
| AVLSet | 2.6 мс | 381.2K ops/s | 2.62 мкс |
| BTreeSet | 0.4 мс | 2.35M ops/s | 0.43 мкс |
| BSTSet | 0.4 мс | 2.45M ops/s | 0.41 мкс |
| SkipListSet | 0.5 мс | 1.97M ops/s | 0.51 мкс |

# Pop min until empty (N=1,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| RBSet | 0.3 мс | 3.08M ops/s | 0.32 мкс |
| AVLSet | 0.3 мс | 2.87M ops/s | 0.35 мкс |
| BTreeSet | 0.3 мс | 3.26M ops/s | 0.31 мкс |
| BSTSet | 0.3 мс | 3.22M ops/s | 0.31 мкс |
| SkipListSet | 0.4 мс | 2.80M ops/s | 0.36 мкс |

# Mixed 50C/25A/25D (N=1,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| RBSet | 0.2 мс | 4.46M ops/s | 0.22 мкс |
| AVLSet | 0.2 мс | 4.90M ops/s | 0.20 мкс |
| BTreeSet | 0.2 мс | 5.08M ops/s | 0.20 мкс |
| BSTSet | 0.3 мс | 3.57M ops/s | 0.28 мкс |
| SkipListSet | 0.3 мс | 3.78M ops/s | 0.26 мкс |

# Seq insert (from range) (N=10,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| RBSet | 3.3 мс | 3.00M ops/s | 0.33 мкс |
| AVLSet | 2.2 мс | 4.52M ops/s | 0.22 мкс |
| BTreeSet | 1.8 мс | 5.44M ops/s | 0.18 мкс |
| BSTSet | 593.1 мс | 16.9K ops/s | 59.31 мкс |
| SkipListSet | 2.2 мс | 4.58M ops/s | 0.22 мкс |

# Rnd insert (from list) (N=10,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| RBSet | 2.6 мс | 3.83M ops/s | 0.26 мкс |
| AVLSet | 2.9 мс | 3.45M ops/s | 0.29 мкс |
| BTreeSet | 2.2 мс | 4.46M ops/s | 0.22 мкс |
| BSTSet | 3.0 мс | 3.30M ops/s | 0.30 мкс |
| SkipListSet | 3.8 мс | 2.65M ops/s | 0.38 мкс |

# Rnd insert (one-by-one) (N=10,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| RBSet | 3.1 мс | 3.20M ops/s | 0.31 мкс |
| AVLSet | 3.4 мс | 2.93M ops/s | 0.34 мкс |
| BTreeSet | 2.7 мс | 3.73M ops/s | 0.27 мкс |
| BSTSet | 3.4 мс | 2.94M ops/s | 0.34 мкс |
| SkipListSet | 4.2 мс | 2.36M ops/s | 0.42 мкс |

# Contains (100% hit) (N=10,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| RBSet | 2.2 мс | 4.55M ops/s | 0.22 мкс |
| AVLSet | 2.2 мс | 4.53M ops/s | 0.22 мкс |
| BTreeSet | 2.1 мс | 4.71M ops/s | 0.21 мкс |
| BSTSet | 2.8 мс | 3.59M ops/s | 0.28 мкс |
| SkipListSet | 3.5 мс | 2.88M ops/s | 0.35 мкс |

# Contains (100% miss) (N=10,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| RBSet | 1.9 мс | 5.36M ops/s | 0.19 мкс |
| AVLSet | 2.2 мс | 4.62M ops/s | 0.22 мкс |
| BTreeSet | 1.7 мс | 5.79M ops/s | 0.17 мкс |
| BSTSet | 1.2 мс | 8.21M ops/s | 0.12 мкс |
| SkipListSet | 1.2 мс | 8.66M ops/s | 0.12 мкс |

# Iteration (full scan) (N=10,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| RBSet | 0.2 мс | 53.84M ops/s | 0.02 мкс |
| AVLSet | 0.2 мс | 56.44M ops/s | 0.02 мкс |
| BTreeSet | 0.1 мс | 82.19M ops/s | 0.01 мкс |
| BSTSet | 0.2 мс | 46.95M ops/s | 0.02 мкс |
| SkipListSet | 0.1 мс | 75.53M ops/s | 0.01 мкс |

# Discard all (N=10,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| RBSet | 6.1 мс | 1.63M ops/s | 0.61 мкс |
| AVLSet | 8.5 мс | 1.17M ops/s | 0.85 мкс |
| BTreeSet | 5.3 мс | 1.89M ops/s | 0.53 мкс |
| BSTSet | 5.9 мс | 1.70M ops/s | 0.59 мкс |
| SkipListSet | 7.9 мс | 1.27M ops/s | 0.79 мкс |

# Pop min until empty (N=10,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| RBSet | 4.8 мс | 2.06M ops/s | 0.48 мкс |
| AVLSet | 4.9 мс | 2.06M ops/s | 0.49 мкс |
| BTreeSet | 3.7 мс | 2.70M ops/s | 0.37 мкс |
| BSTSet | 4.1 мс | 2.43M ops/s | 0.41 мкс |
| SkipListSet | 4.8 мс | 2.09M ops/s | 0.48 мкс |

# Mixed 50C/25A/25D (N=10,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| RBSet | 3.1 мс | 3.22M ops/s | 0.31 мкс |
| AVLSet | 3.4 мс | 2.93M ops/s | 0.34 мкс |
| BTreeSet | 2.8 мс | 3.52M ops/s | 0.28 мкс |
| BSTSet | 3.3 мс | 2.99M ops/s | 0.33 мкс |
| SkipListSet | 6.0 мс | 1.66M ops/s | 0.60 мкс |

# Seq insert (from range) (N=50,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| RBSet | 19.8 мс | 2.53M ops/s | 0.40 мкс |
| AVLSet | 14.8 мс | 3.37M ops/s | 0.30 мкс |
| BTreeSet | 11.0 мс | 4.53M ops/s | 0.22 мкс |
| BSTSet | 15269.7 мс | 3.3K ops/s | 305.39 мкс |
| SkipListSet | 14.7 мс | 3.40M ops/s | 0.29 мкс |

# Rnd insert (from list) (N=50,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| RBSet | 18.7 мс | 2.67M ops/s | 0.37 мкс |
| AVLSet | 22.3 мс | 2.24M ops/s | 0.45 мкс |
| BTreeSet | 18.1 мс | 2.77M ops/s | 0.36 мкс |
| BSTSet | 21.5 мс | 2.32M ops/s | 0.43 мкс |
| SkipListSet | 35.4 мс | 1.41M ops/s | 0.71 мкс |

# Rnd insert (one-by-one) (N=50,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| RBSet | 22.2 мс | 2.25M ops/s | 0.44 мкс |
| AVLSet | 26.5 мс | 1.89M ops/s | 0.53 мкс |
| BTreeSet | 19.7 мс | 2.54M ops/s | 0.39 мкс |
| BSTSet | 24.8 мс | 2.01M ops/s | 0.50 мкс |
| SkipListSet | 31.9 мс | 1.57M ops/s | 0.64 мкс |

# Contains (100% hit) (N=50,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| RBSet | 16.1 мс | 3.11M ops/s | 0.32 мкс |
| AVLSet | 17.7 мс | 2.83M ops/s | 0.35 мкс |
| BTreeSet | 14.1 мс | 3.54M ops/s | 0.28 мкс |
| BSTSet | 20.4 мс | 2.45M ops/s | 0.41 мкс |
| SkipListSet | 25.8 мс | 1.93M ops/s | 0.52 мкс |

# Contains (100% miss) (N=50,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| RBSet | 12.1 мс | 4.15M ops/s | 0.24 мкс |
| AVLSet | 12.3 мс | 4.07M ops/s | 0.25 мкс |
| BTreeSet | 11.6 мс | 4.31M ops/s | 0.23 мкс |
| BSTSet | 8.0 мс | 6.22M ops/s | 0.16 мкс |
| SkipListSet | 12.1 мс | 4.12M ops/s | 0.24 мкс |

# Iteration (full scan) (N=50,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| RBSet | 1.1 мс | 44.66M ops/s | 0.02 мкс |
| AVLSet | 2.6 мс | 19.04M ops/s | 0.05 мкс |
| BTreeSet | 0.7 мс | 71.46M ops/s | 0.01 мкс |
| BSTSet | 1.2 мс | 40.93M ops/s | 0.02 мкс |
| SkipListSet | 0.9 мс | 57.46M ops/s | 0.02 мкс |

# Discard all (N=50,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| RBSet | 37.9 мс | 1.32M ops/s | 0.76 мкс |
| AVLSet | 42.1 мс | 1.19M ops/s | 0.84 мкс |
| BTreeSet | 34.8 мс | 1.44M ops/s | 0.70 мкс |
| BSTSet | 40.3 мс | 1.24M ops/s | 0.81 мкс |
| SkipListSet | 52.1 мс | 960.1K ops/s | 1.04 мкс |

# Pop min until empty (N=50,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| RBSet | 25.4 мс | 1.97M ops/s | 0.51 мкс |
| AVLSet | 32.0 мс | 1.56M ops/s | 0.64 мкс |
| BTreeSet | 25.9 мс | 1.93M ops/s | 0.52 мкс |
| BSTSet | 27.9 мс | 1.79M ops/s | 0.56 мкс |
| SkipListSet | 34.3 мс | 1.46M ops/s | 0.69 мкс |

# Mixed 50C/25A/25D (N=50,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| RBSet | 21.2 мс | 2.36M ops/s | 0.42 мкс |
| AVLSet | 19.8 мс | 2.52M ops/s | 0.40 мкс |
| BTreeSet | 18.2 мс | 2.75M ops/s | 0.36 мкс |
| BSTSet | 24.1 мс | 2.08M ops/s | 0.48 мкс |
| SkipListSet | 30.0 мс | 1.67M ops/s | 0.60 мкс |

# Seq insert (from range) (N=1,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| RBSet | 0.2 мс | 4.07M ops/s | 0.25 мкс |
| AVLSet | 0.2 мс | 5.91M ops/s | 0.17 мкс |
| BTreeSet | 0.1 мс | 7.42M ops/s | 0.13 мкс |
| BSTSet | 5.7 мс | 176.1K ops/s | 5.68 мкс |
| SkipListSet | 0.2 мс | 5.70M ops/s | 0.18 мкс |

# Rnd insert (from list) (N=1,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| RBSet | 0.2 мс | 5.62M ops/s | 0.18 мкс |
| AVLSet | 0.2 мс | 5.56M ops/s | 0.18 мкс |
| BTreeSet | 0.2 мс | 6.22M ops/s | 0.16 мкс |
| BSTSet | 0.2 мс | 5.59M ops/s | 0.18 мкс |
| SkipListSet | 0.3 мс | 3.19M ops/s | 0.31 мкс |

# Rnd insert (one-by-one) (N=1,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| RBSet | 0.2 мс | 4.43M ops/s | 0.23 мкс |
| AVLSet | 0.2 мс | 4.32M ops/s | 0.23 мкс |
| BTreeSet | 0.2 мс | 4.43M ops/s | 0.23 мкс |
| BSTSet | 0.2 мс | 4.00M ops/s | 0.25 мкс |
| SkipListSet | 0.3 мс | 3.38M ops/s | 0.30 мкс |

# Contains (100% hit) (N=1,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| RBSet | 0.1 мс | 7.55M ops/s | 0.13 мкс |
| AVLSet | 0.1 мс | 7.81M ops/s | 0.13 мкс |
| BTreeSet | 0.1 мс | 7.45M ops/s | 0.13 мкс |
| BSTSet | 0.2 мс | 4.98M ops/s | 0.20 мкс |
| SkipListSet | 0.2 мс | 5.80M ops/s | 0.17 мкс |

# Contains (100% miss) (N=1,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| RBSet | 0.2 мс | 5.27M ops/s | 0.19 мкс |
| AVLSet | 0.1 мс | 7.77M ops/s | 0.13 мкс |
| BTreeSet | 0.1 мс | 7.90M ops/s | 0.13 мкс |
| BSTSet | 0.2 мс | 5.89M ops/s | 0.17 мкс |
| SkipListSet | 0.1 мс | 8.10M ops/s | 0.12 мкс |

# Iteration (full scan) (N=1,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| RBSet | 0.0 мс | 47.82M ops/s | 0.02 мкс |
| AVLSet | 0.0 мс | 62.56M ops/s | 0.02 мкс |
| BTreeSet | 0.0 мс | 73.03M ops/s | 0.01 мкс |
| BSTSet | 0.0 мс | 53.53M ops/s | 0.02 мкс |
| SkipListSet | 0.0 мс | 91.41M ops/s | 0.01 мкс |

# Discard all (N=1,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| RBSet | 0.4 мс | 2.48M ops/s | 0.40 мкс |
| AVLSet | 0.4 мс | 2.32M ops/s | 0.43 мкс |
| BTreeSet | 0.4 мс | 2.41M ops/s | 0.41 мкс |
| BSTSet | 0.4 мс | 2.51M ops/s | 0.40 мкс |
| SkipListSet | 0.5 мс | 2.02M ops/s | 0.49 мкс |

# Pop min until empty (N=1,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| RBSet | 0.3 мс | 3.27M ops/s | 0.31 мкс |
| AVLSet | 0.3 мс | 2.94M ops/s | 0.34 мкс |
| BTreeSet | 0.3 мс | 3.20M ops/s | 0.31 мкс |
| BSTSet | 0.3 мс | 3.10M ops/s | 0.32 мкс |
| SkipListSet | 0.4 мс | 2.82M ops/s | 0.35 мкс |

# Mixed 50C/25A/25D (N=1,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| RBSet | 0.2 мс | 4.86M ops/s | 0.21 мкс |
| AVLSet | 0.2 мс | 4.94M ops/s | 0.20 мкс |
| BTreeSet | 0.2 мс | 4.50M ops/s | 0.22 мкс |
| BSTSet | 0.2 мс | 4.21M ops/s | 0.24 мкс |
| SkipListSet | 0.2 мс | 4.10M ops/s | 0.24 мкс |

# Seq insert (from range) (N=10,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| RBSet | 6.0 мс | 1.67M ops/s | 0.60 мкс |
| AVLSet | 3.1 мс | 3.28M ops/s | 0.31 мкс |
| BTreeSet | 1.9 мс | 5.32M ops/s | 0.19 мкс |
| BSTSet | 586.6 мс | 17.0K ops/s | 58.66 мкс |
| SkipListSet | 2.2 мс | 4.61M ops/s | 0.22 мкс |

# Rnd insert (from list) (N=10,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| RBSet | 2.8 мс | 3.57M ops/s | 0.28 мкс |
| AVLSet | 3.0 мс | 3.35M ops/s | 0.30 мкс |
| BTreeSet | 2.7 мс | 3.72M ops/s | 0.27 мкс |
| BSTSet | 2.9 мс | 3.41M ops/s | 0.29 мкс |
| SkipListSet | 4.0 мс | 2.50M ops/s | 0.40 мкс |

# Rnd insert (one-by-one) (N=10,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| RBSet | 3.2 мс | 3.16M ops/s | 0.32 мкс |
| AVLSet | 3.6 мс | 2.81M ops/s | 0.36 мкс |
| BTreeSet | 2.8 мс | 3.58M ops/s | 0.28 мкс |
| BSTSet | 3.6 мс | 2.80M ops/s | 0.36 мкс |
| SkipListSet | 4.3 мс | 2.34M ops/s | 0.43 мкс |

# Contains (100% hit) (N=10,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| RBSet | 2.2 мс | 4.45M ops/s | 0.22 мкс |
| AVLSet | 2.5 мс | 4.07M ops/s | 0.25 мкс |
| BTreeSet | 2.3 мс | 4.41M ops/s | 0.23 мкс |
| BSTSet | 2.8 мс | 3.56M ops/s | 0.28 мкс |
| SkipListSet | 3.6 мс | 2.75M ops/s | 0.36 мкс |

# Contains (100% miss) (N=10,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| RBSet | 2.3 мс | 4.43M ops/s | 0.23 мкс |
| AVLSet | 1.8 мс | 5.41M ops/s | 0.18 мкс |
| BTreeSet | 1.8 мс | 5.43M ops/s | 0.18 мкс |
| BSTSet | 1.7 мс | 5.79M ops/s | 0.17 мкс |
| SkipListSet | 1.1 мс | 8.83M ops/s | 0.11 мкс |

# Iteration (full scan) (N=10,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| RBSet | 0.2 мс | 56.06M ops/s | 0.02 мкс |
| AVLSet | 0.2 мс | 52.64M ops/s | 0.02 мкс |
| BTreeSet | 0.1 мс | 79.39M ops/s | 0.01 мкс |
| BSTSet | 0.3 мс | 39.20M ops/s | 0.03 мкс |
| SkipListSet | 0.1 мс | 67.90M ops/s | 0.01 мкс |

# Discard all (N=10,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| RBSet | 5.8 мс | 1.73M ops/s | 0.58 мкс |
| AVLSet | 6.2 мс | 1.61M ops/s | 0.62 мкс |
| BTreeSet | 5.6 мс | 1.79M ops/s | 0.56 мкс |
| BSTSet | 6.1 мс | 1.63M ops/s | 0.61 мкс |
| SkipListSet | 7.3 мс | 1.36M ops/s | 0.73 мкс |

# Pop min until empty (N=10,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| RBSet | 4.4 мс | 2.28M ops/s | 0.44 мкс |
| AVLSet | 4.9 мс | 2.02M ops/s | 0.49 мкс |
| BTreeSet | 4.0 мс | 2.52M ops/s | 0.40 мкс |
| BSTSet | 4.2 мс | 2.39M ops/s | 0.42 мкс |
| SkipListSet | 4.6 мс | 2.16M ops/s | 0.46 мкс |

# Mixed 50C/25A/25D (N=10,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| RBSet | 3.0 мс | 3.31M ops/s | 0.30 мкс |
| AVLSet | 3.1 мс | 3.22M ops/s | 0.31 мкс |
| BTreeSet | 2.7 мс | 3.64M ops/s | 0.27 мкс |
| BSTSet | 3.9 мс | 2.53M ops/s | 0.39 мкс |
| SkipListSet | 4.2 мс | 2.36M ops/s | 0.42 мкс |

# Seq insert (from range) (N=50,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| RBSet | 19.8 мс | 2.53M ops/s | 0.40 мкс |
| AVLSet | 15.2 мс | 3.28M ops/s | 0.30 мкс |
| BTreeSet | 11.2 мс | 4.48M ops/s | 0.22 мкс |
| BSTSet | 15566.4 мс | 3.2K ops/s | 311.33 мкс |
| SkipListSet | 15.6 мс | 3.21M ops/s | 0.31 мкс |

# Rnd insert (from list) (N=50,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| RBSet | 21.8 мс | 2.29M ops/s | 0.44 мкс |
| AVLSet | 23.2 мс | 2.15M ops/s | 0.46 мкс |
| BTreeSet | 19.4 мс | 2.58M ops/s | 0.39 мкс |
| BSTSet | 39.1 мс | 1.28M ops/s | 0.78 мкс |
| SkipListSet | 68.0 мс | 735.8K ops/s | 1.36 мкс |

# Rnd insert (one-by-one) (N=50,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| RBSet | 22.5 мс | 2.22M ops/s | 0.45 мкс |
| AVLSet | 27.0 мс | 1.85M ops/s | 0.54 мкс |
| BTreeSet | 19.5 мс | 2.56M ops/s | 0.39 мкс |
| BSTSet | 22.4 мс | 2.23M ops/s | 0.45 мкс |
| SkipListSet | 40.7 мс | 1.23M ops/s | 0.81 мкс |

# Contains (100% hit) (N=50,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| RBSet | 15.9 мс | 3.15M ops/s | 0.32 мкс |
| AVLSet | 18.3 мс | 2.73M ops/s | 0.37 мкс |
| BTreeSet | 14.0 мс | 3.56M ops/s | 0.28 мкс |
| BSTSet | 19.8 мс | 2.53M ops/s | 0.40 мкс |
| SkipListSet | 25.7 мс | 1.95M ops/s | 0.51 мкс |

# Contains (100% miss) (N=50,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| RBSet | 10.8 мс | 4.62M ops/s | 0.22 мкс |
| AVLSet | 11.8 мс | 4.23M ops/s | 0.24 мкс |
| BTreeSet | 10.4 мс | 4.81M ops/s | 0.21 мкс |
| BSTSet | 12.1 мс | 4.14M ops/s | 0.24 мкс |
| SkipListSet | 10.7 мс | 4.68M ops/s | 0.21 мкс |

# Iteration (full scan) (N=50,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| RBSet | 1.6 мс | 30.60M ops/s | 0.03 мкс |
| AVLSet | 1.5 мс | 33.65M ops/s | 0.03 мкс |
| BTreeSet | 0.7 мс | 71.49M ops/s | 0.01 мкс |
| BSTSet | 1.3 мс | 37.79M ops/s | 0.03 мкс |
| SkipListSet | 1.3 мс | 37.44M ops/s | 0.03 мкс |

# Discard all (N=50,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| RBSet | 38.0 мс | 1.31M ops/s | 0.76 мкс |
| AVLSet | 44.1 мс | 1.13M ops/s | 0.88 мкс |
| BTreeSet | 33.2 мс | 1.51M ops/s | 0.66 мкс |
| BSTSet | 42.0 мс | 1.19M ops/s | 0.84 мкс |
| SkipListSet | 57.7 мс | 866.4K ops/s | 1.15 мкс |

# Pop min until empty (N=50,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| RBSet | 25.7 мс | 1.94M ops/s | 0.51 мкс |
| AVLSet | 33.6 мс | 1.49M ops/s | 0.67 мкс |
| BTreeSet | 26.3 мс | 1.90M ops/s | 0.53 мкс |
| BSTSet | 26.1 мс | 1.92M ops/s | 0.52 мкс |
| SkipListSet | 31.8 мс | 1.57M ops/s | 0.64 мкс |

# Mixed 50C/25A/25D (N=50,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| RBSet | 22.5 мс | 2.22M ops/s | 0.45 мкс |
| AVLSet | 25.4 мс | 1.97M ops/s | 0.51 мкс |
| BTreeSet | 17.6 мс | 2.85M ops/s | 0.35 мкс |
| BSTSet | 34.7 мс | 1.44M ops/s | 0.69 мкс |
| SkipListSet | 30.8 мс | 1.63M ops/s | 0.62 мкс |

