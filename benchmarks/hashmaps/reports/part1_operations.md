# Seq insert (N=1,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 0.1 мс | 15.12M ops/s | 0.07 мкс |
| HashMap | 0.1 мс | 7.12M ops/s | 0.14 мкс |
| RHMap | 0.1 мс | 12.30M ops/s | 0.08 мкс |
| BTHashMap | 0.1 мс | 7.24M ops/s | 0.14 мкс |
| SwissTable | 0.1 мс | 8.40M ops/s | 0.12 мкс |

# Rnd insert (N=1,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 0.0 мс | 31.03M ops/s | 0.03 мкс |
| HashMap | 0.1 мс | 7.84M ops/s | 0.13 мкс |
| RHMap | 0.1 мс | 12.20M ops/s | 0.08 мкс |
| BTHashMap | 0.1 мс | 7.53M ops/s | 0.13 мкс |
| SwissTable | 0.1 мс | 6.98M ops/s | 0.14 мкс |

# Lookup hit (N=1,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 0.0 мс | 51.59M ops/s | 0.02 мкс |
| HashMap | 0.1 мс | 17.11M ops/s | 0.06 мкс |
| RHMap | 0.0 мс | 48.91M ops/s | 0.02 мкс |
| BTHashMap | 0.0 мс | 21.49M ops/s | 0.05 мкс |
| SwissTable | 0.0 мс | 21.35M ops/s | 0.05 мкс |

# Lookup miss (N=1,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 0.0 мс | 54.74M ops/s | 0.02 мкс |
| HashMap | 0.0 мс | 54.96M ops/s | 0.02 мкс |
| RHMap | 0.0 мс | 26.52M ops/s | 0.04 мкс |
| BTHashMap | 0.0 мс | 21.85M ops/s | 0.05 мкс |
| SwissTable | 0.0 мс | 21.43M ops/s | 0.05 мкс |

# Lookup Zipf (N=1,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 0.0 мс | 53.86M ops/s | 0.02 мкс |
| HashMap | 0.0 мс | 41.40M ops/s | 0.02 мкс |
| RHMap | 0.0 мс | 51.44M ops/s | 0.02 мкс |
| BTHashMap | 0.1 мс | 13.39M ops/s | 0.07 мкс |
| SwissTable | 0.0 мс | 23.06M ops/s | 0.04 мкс |

# Update all (N=1,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 0.0 мс | 45.99M ops/s | 0.02 мкс |
| HashMap | 0.0 мс | 29.73M ops/s | 0.03 мкс |
| RHMap | 0.0 мс | 43.72M ops/s | 0.02 мкс |
| BTHashMap | 0.0 мс | 20.57M ops/s | 0.05 мкс |
| SwissTable | 0.0 мс | 21.75M ops/s | 0.05 мкс |

# Insert+Delete (N=1,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 0.1 мс | 38.09M ops/s | 0.03 мкс |
| HashMap | 0.3 мс | 7.36M ops/s | 0.14 мкс |
| RHMap | 0.1 мс | 16.67M ops/s | 0.06 мкс |
| BTHashMap | 0.2 мс | 9.61M ops/s | 0.10 мкс |
| SwissTable | 0.3 мс | 6.82M ops/s | 0.15 мкс |

# Mixed 80R/20W (N=1,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 0.0 мс | 27.24M ops/s | 0.04 мкс |
| HashMap | 0.1 мс | 17.79M ops/s | 0.06 мкс |
| RHMap | 0.1 мс | 10.36M ops/s | 0.10 мкс |
| BTHashMap | 0.1 мс | 8.36M ops/s | 0.12 мкс |
| SwissTable | 0.1 мс | 16.22M ops/s | 0.06 мкс |

# Seq insert (N=10,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 0.6 мс | 16.14M ops/s | 0.06 мкс |
| HashMap | 3.2 мс | 3.16M ops/s | 0.32 мкс |
| RHMap | 0.9 мс | 10.90M ops/s | 0.09 мкс |
| BTHashMap | 2.1 мс | 4.74M ops/s | 0.21 мкс |
| SwissTable | 1.3 мс | 7.98M ops/s | 0.13 мкс |

# Rnd insert (N=10,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 0.4 мс | 23.39M ops/s | 0.04 мкс |
| HashMap | 1.8 мс | 5.69M ops/s | 0.18 мкс |
| RHMap | 1.6 мс | 6.40M ops/s | 0.16 мкс |
| BTHashMap | 5.2 мс | 1.91M ops/s | 0.52 мкс |
| SwissTable | 2.3 мс | 4.28M ops/s | 0.23 мкс |

# Lookup hit (N=10,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 0.5 мс | 21.81M ops/s | 0.05 мкс |
| HashMap | 0.5 мс | 20.38M ops/s | 0.05 мкс |
| RHMap | 0.3 мс | 31.90M ops/s | 0.03 мкс |
| BTHashMap | 0.7 мс | 14.38M ops/s | 0.07 мкс |
| SwissTable | 0.6 мс | 16.14M ops/s | 0.06 мкс |

# Lookup miss (N=10,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 0.4 мс | 23.83M ops/s | 0.04 мкс |
| HashMap | 0.5 мс | 20.68M ops/s | 0.05 мкс |
| RHMap | 0.4 мс | 22.54M ops/s | 0.04 мкс |
| BTHashMap | 0.8 мс | 11.97M ops/s | 0.08 мкс |
| SwissTable | 0.7 мс | 14.83M ops/s | 0.07 мкс |

# Lookup Zipf (N=10,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 0.2 мс | 53.12M ops/s | 0.02 мкс |
| HashMap | 0.3 мс | 34.08M ops/s | 0.03 мкс |
| RHMap | 0.2 мс | 43.63M ops/s | 0.02 мкс |
| BTHashMap | 0.3 мс | 29.60M ops/s | 0.03 мкс |
| SwissTable | 0.5 мс | 19.45M ops/s | 0.05 мкс |

# Update all (N=10,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 0.3 мс | 39.10M ops/s | 0.03 мкс |
| HashMap | 0.5 мс | 20.86M ops/s | 0.05 мкс |
| RHMap | 0.4 мс | 28.01M ops/s | 0.04 мкс |
| BTHashMap | 0.7 мс | 13.62M ops/s | 0.07 мкс |
| SwissTable | 0.7 мс | 15.02M ops/s | 0.07 мкс |

# Insert+Delete (N=10,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 0.6 мс | 31.32M ops/s | 0.03 мкс |
| HashMap | 2.1 мс | 9.53M ops/s | 0.10 мкс |
| RHMap | 2.0 мс | 9.92M ops/s | 0.10 мкс |
| BTHashMap | 3.5 мс | 5.79M ops/s | 0.17 мкс |
| SwissTable | 5.0 мс | 4.03M ops/s | 0.25 мкс |

# Mixed 80R/20W (N=10,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 1.0 мс | 9.67M ops/s | 0.10 мкс |
| HashMap | 1.6 мс | 6.20M ops/s | 0.16 мкс |
| RHMap | 8.2 мс | 1.22M ops/s | 0.82 мкс |
| BTHashMap | 1.2 мс | 8.18M ops/s | 0.12 мкс |
| SwissTable | 0.8 мс | 12.66M ops/s | 0.08 мкс |

# Seq insert (N=50,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 3.9 мс | 12.97M ops/s | 0.08 мкс |
| HashMap | 14.3 мс | 3.49M ops/s | 0.29 мкс |
| RHMap | 6.3 мс | 7.99M ops/s | 0.13 мкс |
| BTHashMap | 9.2 мс | 5.46M ops/s | 0.18 мкс |
| SwissTable | 5.5 мс | 9.17M ops/s | 0.11 мкс |

# Rnd insert (N=50,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 4.2 мс | 11.93M ops/s | 0.08 мкс |
| HashMap | 26.3 мс | 1.90M ops/s | 0.53 мкс |
| RHMap | 7.8 мс | 6.40M ops/s | 0.16 мкс |
| BTHashMap | 18.8 мс | 2.66M ops/s | 0.38 мкс |
| SwissTable | 7.5 мс | 6.65M ops/s | 0.15 мкс |

# Lookup hit (N=50,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 3.3 мс | 15.24M ops/s | 0.07 мкс |
| HashMap | 7.1 мс | 7.04M ops/s | 0.14 мкс |
| RHMap | 3.5 мс | 14.13M ops/s | 0.07 мкс |
| BTHashMap | 10.2 мс | 4.90M ops/s | 0.20 мкс |
| SwissTable | 3.7 мс | 13.38M ops/s | 0.07 мкс |

# Lookup miss (N=50,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 2.3 мс | 22.14M ops/s | 0.05 мкс |
| HashMap | 2.1 мс | 24.26M ops/s | 0.04 мкс |
| RHMap | 2.5 мс | 19.93M ops/s | 0.05 мкс |
| BTHashMap | 9.3 мс | 5.38M ops/s | 0.19 мкс |
| SwissTable | 4.5 мс | 11.22M ops/s | 0.09 мкс |

# Lookup Zipf (N=50,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 1.0 мс | 50.06M ops/s | 0.02 мкс |
| HashMap | 1.3 мс | 39.62M ops/s | 0.03 мкс |
| RHMap | 1.0 мс | 47.71M ops/s | 0.02 мкс |
| BTHashMap | 2.0 мс | 24.60M ops/s | 0.04 мкс |
| SwissTable | 2.5 мс | 19.61M ops/s | 0.05 мкс |

# Update all (N=50,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 1.8 мс | 28.48M ops/s | 0.04 мкс |
| HashMap | 5.5 мс | 9.07M ops/s | 0.11 мкс |
| RHMap | 2.1 мс | 24.23M ops/s | 0.04 мкс |
| BTHashMap | 13.2 мс | 3.80M ops/s | 0.26 мкс |
| SwissTable | 4.0 мс | 12.38M ops/s | 0.08 мкс |

# Insert+Delete (N=50,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 6.3 мс | 15.82M ops/s | 0.06 мкс |
| HashMap | 32.2 мс | 3.10M ops/s | 0.32 мкс |
| RHMap | 10.1 мс | 9.86M ops/s | 0.10 мкс |
| BTHashMap | 25.1 мс | 3.99M ops/s | 0.25 мкс |
| SwissTable | 12.0 мс | 8.34M ops/s | 0.12 мкс |

# Mixed 80R/20W (N=50,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 6.2 мс | 8.05M ops/s | 0.12 мкс |
| HashMap | 13.2 мс | 3.77M ops/s | 0.26 мкс |
| RHMap | 114.6 мс | 436.2K ops/s | 2.29 мкс |
| BTHashMap | 9.9 мс | 5.06M ops/s | 0.20 мкс |
| SwissTable | 4.8 мс | 10.51M ops/s | 0.10 мкс |

# Seq insert (N=1,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 0.1 мс | 9.60M ops/s | 0.10 мкс |
| HashMap | 0.2 мс | 4.25M ops/s | 0.24 мкс |
| RHMap | 0.1 мс | 9.09M ops/s | 0.11 мкс |
| BTHashMap | 0.2 мс | 5.01M ops/s | 0.20 мкс |
| SwissTable | 0.2 мс | 5.60M ops/s | 0.18 мкс |

# Rnd insert (N=1,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 0.0 мс | 30.11M ops/s | 0.03 мкс |
| HashMap | 0.2 мс | 5.57M ops/s | 0.18 мкс |
| RHMap | 0.1 мс | 8.97M ops/s | 0.11 мкс |
| BTHashMap | 0.2 мс | 5.32M ops/s | 0.19 мкс |
| SwissTable | 0.2 мс | 4.66M ops/s | 0.21 мкс |

# Lookup hit (N=1,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 0.0 мс | 50.35M ops/s | 0.02 мкс |
| HashMap | 0.1 мс | 15.43M ops/s | 0.06 мкс |
| RHMap | 0.0 мс | 45.42M ops/s | 0.02 мкс |
| BTHashMap | 0.1 мс | 13.98M ops/s | 0.07 мкс |
| SwissTable | 0.0 мс | 20.35M ops/s | 0.05 мкс |

# Lookup miss (N=1,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 0.0 мс | 44.83M ops/s | 0.02 мкс |
| HashMap | 0.0 мс | 24.13M ops/s | 0.04 мкс |
| RHMap | 0.0 мс | 62.32M ops/s | 0.02 мкс |
| BTHashMap | 0.0 мс | 21.18M ops/s | 0.05 мкс |
| SwissTable | 0.1 мс | 11.93M ops/s | 0.08 мкс |

# Lookup Zipf (N=1,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 0.0 мс | 49.51M ops/s | 0.02 мкс |
| HashMap | 0.0 мс | 39.12M ops/s | 0.03 мкс |
| RHMap | 0.1 мс | 16.37M ops/s | 0.06 мкс |
| BTHashMap | 0.1 мс | 13.39M ops/s | 0.07 мкс |
| SwissTable | 0.0 мс | 21.23M ops/s | 0.05 мкс |

# Update all (N=1,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 0.0 мс | 41.27M ops/s | 0.02 мкс |
| HashMap | 0.1 мс | 18.75M ops/s | 0.05 мкс |
| RHMap | 0.0 мс | 41.86M ops/s | 0.02 мкс |
| BTHashMap | 0.1 мс | 19.00M ops/s | 0.05 мкс |
| SwissTable | 0.0 мс | 20.18M ops/s | 0.05 мкс |

# Insert+Delete (N=1,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 0.1 мс | 26.32M ops/s | 0.04 мкс |
| HashMap | 3.4 мс | 588.3K ops/s | 1.70 мкс |
| RHMap | 0.1 мс | 23.96M ops/s | 0.04 мкс |
| BTHashMap | 0.2 мс | 8.79M ops/s | 0.11 мкс |
| SwissTable | 0.2 мс | 9.51M ops/s | 0.11 мкс |

# Mixed 80R/20W (N=1,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 0.0 мс | 25.69M ops/s | 0.04 мкс |
| HashMap | 0.1 мс | 8.79M ops/s | 0.11 мкс |
| RHMap | 0.1 мс | 8.66M ops/s | 0.12 мкс |
| BTHashMap | 0.1 мс | 8.04M ops/s | 0.12 мкс |
| SwissTable | 0.1 мс | 15.41M ops/s | 0.06 мкс |

# Seq insert (N=10,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 0.6 мс | 16.36M ops/s | 0.06 мкс |
| HashMap | 1.7 мс | 5.92M ops/s | 0.17 мкс |
| RHMap | 0.8 мс | 12.15M ops/s | 0.08 мкс |
| BTHashMap | 4.6 мс | 2.16M ops/s | 0.46 мкс |
| SwissTable | 1.4 мс | 7.02M ops/s | 0.14 мкс |

# Rnd insert (N=10,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 0.5 мс | 20.45M ops/s | 0.05 мкс |
| HashMap | 1.8 мс | 5.42M ops/s | 0.18 мкс |
| RHMap | 1.5 мс | 6.72M ops/s | 0.15 мкс |
| BTHashMap | 2.9 мс | 3.43M ops/s | 0.29 мкс |
| SwissTable | 1.4 мс | 7.01M ops/s | 0.14 мкс |

# Lookup hit (N=10,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 0.3 мс | 35.94M ops/s | 0.03 мкс |
| HashMap | 0.7 мс | 13.51M ops/s | 0.07 мкс |
| RHMap | 0.3 мс | 35.76M ops/s | 0.03 мкс |
| BTHashMap | 0.9 мс | 10.67M ops/s | 0.09 мкс |
| SwissTable | 0.6 мс | 16.56M ops/s | 0.06 мкс |

# Lookup miss (N=10,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 0.4 мс | 24.37M ops/s | 0.04 мкс |
| HashMap | 0.4 мс | 27.00M ops/s | 0.04 мкс |
| RHMap | 0.4 мс | 26.26M ops/s | 0.04 мкс |
| BTHashMap | 1.0 мс | 9.90M ops/s | 0.10 мкс |
| SwissTable | 0.7 мс | 14.22M ops/s | 0.07 мкс |

# Lookup Zipf (N=10,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 0.2 мс | 53.44M ops/s | 0.02 мкс |
| HashMap | 0.3 мс | 39.68M ops/s | 0.03 мкс |
| RHMap | 0.2 мс | 49.90M ops/s | 0.02 мкс |
| BTHashMap | 0.3 мс | 30.91M ops/s | 0.03 мкс |
| SwissTable | 0.4 мс | 23.23M ops/s | 0.04 мкс |

# Update all (N=10,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 0.3 мс | 39.21M ops/s | 0.03 мкс |
| HashMap | 0.5 мс | 21.87M ops/s | 0.05 мкс |
| RHMap | 0.3 мс | 31.34M ops/s | 0.03 мкс |
| BTHashMap | 0.8 мс | 12.23M ops/s | 0.08 мкс |
| SwissTable | 0.8 мс | 13.12M ops/s | 0.08 мкс |

# Insert+Delete (N=10,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 0.7 мс | 28.77M ops/s | 0.03 мкс |
| HashMap | 2.3 мс | 8.66M ops/s | 0.12 мкс |
| RHMap | 2.1 мс | 9.41M ops/s | 0.11 мкс |
| BTHashMap | 6.9 мс | 2.88M ops/s | 0.35 мкс |
| SwissTable | 2.2 мс | 9.28M ops/s | 0.11 мкс |

# Mixed 80R/20W (N=10,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 3.2 мс | 3.09M ops/s | 0.32 мкс |
| HashMap | 1.5 мс | 6.75M ops/s | 0.15 мкс |
| RHMap | 8.9 мс | 1.12M ops/s | 0.89 мкс |
| BTHashMap | 1.0 мс | 10.33M ops/s | 0.10 мкс |
| SwissTable | 0.9 мс | 11.32M ops/s | 0.09 мкс |

# Seq insert (N=50,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 4.2 мс | 11.98M ops/s | 0.08 мкс |
| HashMap | 14.0 мс | 3.57M ops/s | 0.28 мкс |
| RHMap | 6.4 мс | 7.84M ops/s | 0.13 мкс |
| BTHashMap | 9.0 мс | 5.55M ops/s | 0.18 мкс |
| SwissTable | 5.7 мс | 8.73M ops/s | 0.11 мкс |

# Rnd insert (N=50,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 4.8 мс | 10.52M ops/s | 0.10 мкс |
| HashMap | 26.9 мс | 1.86M ops/s | 0.54 мкс |
| RHMap | 7.2 мс | 6.90M ops/s | 0.14 мкс |
| BTHashMap | 15.3 мс | 3.26M ops/s | 0.31 мкс |
| SwissTable | 8.8 мс | 5.71M ops/s | 0.18 мкс |

# Lookup hit (N=50,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 2.3 мс | 21.44M ops/s | 0.05 мкс |
| HashMap | 13.1 мс | 3.80M ops/s | 0.26 мкс |
| RHMap | 2.3 мс | 21.31M ops/s | 0.05 мкс |
| BTHashMap | 7.6 мс | 6.55M ops/s | 0.15 мкс |
| SwissTable | 3.4 мс | 14.60M ops/s | 0.07 мкс |

# Lookup miss (N=50,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 2.0 мс | 24.90M ops/s | 0.04 мкс |
| HashMap | 1.9 мс | 26.35M ops/s | 0.04 мкс |
| RHMap | 2.5 мс | 20.17M ops/s | 0.05 мкс |
| BTHashMap | 11.9 мс | 4.20M ops/s | 0.24 мкс |
| SwissTable | 6.7 мс | 7.49M ops/s | 0.13 мкс |

# Lookup Zipf (N=50,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 1.0 мс | 52.27M ops/s | 0.02 мкс |
| HashMap | 1.3 мс | 39.53M ops/s | 0.03 мкс |
| RHMap | 1.0 мс | 48.32M ops/s | 0.02 мкс |
| BTHashMap | 2.1 мс | 23.26M ops/s | 0.04 мкс |
| SwissTable | 2.7 мс | 18.40M ops/s | 0.05 мкс |

# Update all (N=50,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 1.7 мс | 29.56M ops/s | 0.03 мкс |
| HashMap | 6.1 мс | 8.21M ops/s | 0.12 мкс |
| RHMap | 2.1 мс | 23.40M ops/s | 0.04 мкс |
| BTHashMap | 10.1 мс | 4.93M ops/s | 0.20 мкс |
| SwissTable | 5.6 мс | 8.85M ops/s | 0.11 мкс |

# Insert+Delete (N=50,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 4.4 мс | 22.86M ops/s | 0.04 мкс |
| HashMap | 28.3 мс | 3.53M ops/s | 0.28 мкс |
| RHMap | 11.2 мс | 8.93M ops/s | 0.11 мкс |
| BTHashMap | 28.6 мс | 3.50M ops/s | 0.29 мкс |
| SwissTable | 12.9 мс | 7.76M ops/s | 0.13 мкс |

# Mixed 80R/20W (N=50,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 4.5 мс | 11.02M ops/s | 0.09 мкс |
| HashMap | 10.0 мс | 4.99M ops/s | 0.20 мкс |
| RHMap | 116.8 мс | 428.1K ops/s | 2.34 мкс |
| BTHashMap | 10.0 мс | 5.00M ops/s | 0.20 мкс |
| SwissTable | 4.9 мс | 10.21M ops/s | 0.10 мкс |

