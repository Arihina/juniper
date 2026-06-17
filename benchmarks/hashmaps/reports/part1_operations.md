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

# Seq insert (N=1,000,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 67.8 мс | 14.74M ops/s | 0.07 мкс |
| HashMap | 275.7 мс | 3.63M ops/s | 0.28 мкс |
| RHMap | 105.7 мс | 9.46M ops/s | 0.11 мкс |
| BTHashMap | 223.0 мс | 4.48M ops/s | 0.22 мкс |
| SwissTable | 126.3 мс | 7.92M ops/s | 0.13 мкс |

# Rnd insert (N=1,000,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 173.3 мс | 5.77M ops/s | 0.17 мкс |
| HashMap | 1108.4 мс | 902.2K ops/s | 1.11 мкс |
| RHMap | 217.0 мс | 4.61M ops/s | 0.22 мкс |
| BTHashMap | 616.3 мс | 1.62M ops/s | 0.62 мкс |
| SwissTable | 271.8 мс | 3.68M ops/s | 0.27 мкс |

# Lookup hit (N=1,000,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 246.9 мс | 4.05M ops/s | 0.25 мкс |
| HashMap | 361.0 мс | 2.77M ops/s | 0.36 мкс |
| RHMap | 176.2 мс | 5.67M ops/s | 0.18 мкс |
| BTHashMap | 396.4 мс | 2.52M ops/s | 0.40 мкс |
| SwissTable | 221.7 мс | 4.51M ops/s | 0.22 мкс |

# Lookup miss (N=1,000,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 104.0 мс | 9.62M ops/s | 0.10 мкс |
| HashMap | 89.3 мс | 11.20M ops/s | 0.09 мкс |
| RHMap | 89.6 мс | 11.17M ops/s | 0.09 мкс |
| BTHashMap | 432.6 мс | 2.31M ops/s | 0.43 мкс |
| SwissTable | 214.3 мс | 4.67M ops/s | 0.21 мкс |

# Lookup Zipf (N=1,000,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 18.5 мс | 54.06M ops/s | 0.02 мкс |
| HashMap | 24.5 мс | 40.77M ops/s | 0.02 мкс |
| RHMap | 19.3 мс | 51.76M ops/s | 0.02 мкс |
| BTHashMap | 37.8 мс | 26.46M ops/s | 0.04 мкс |
| SwissTable | 33.6 мс | 29.72M ops/s | 0.03 мкс |

# Update all (N=1,000,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 136.3 мс | 7.34M ops/s | 0.14 мкс |
| HashMap | 282.8 мс | 3.54M ops/s | 0.28 мкс |
| RHMap | 187.8 мс | 5.33M ops/s | 0.19 мкс |
| BTHashMap | 350.5 мс | 2.85M ops/s | 0.35 мкс |
| SwissTable | 218.3 мс | 4.58M ops/s | 0.22 мкс |

# Insert+Delete (N=1,000,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 285.2 мс | 7.01M ops/s | 0.14 мкс |
| HashMap | 1015.0 мс | 1.97M ops/s | 0.51 мкс |
| RHMap | 444.5 мс | 4.50M ops/s | 0.22 мкс |
| BTHashMap | 1023.5 мс | 1.95M ops/s | 0.51 мкс |
| SwissTable | 497.4 мс | 4.02M ops/s | 0.25 мкс |

# Mixed 80R/20W (N=1,000,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 240.9 мс | 4.15M ops/s | 0.24 мкс |
| HashMap | 360.2 мс | 2.78M ops/s | 0.36 мкс |
| RHMap | 159334.0 мс | 6.3K ops/s | 159.33 мкс |
| BTHashMap | 606.7 мс | 1.65M ops/s | 0.61 мкс |
| SwissTable | 253.6 мс | 3.94M ops/s | 0.25 мкс |

# Seq insert (N=2,000,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 151.6 мс | 13.19M ops/s | 0.08 мкс |
| HashMap | 362.7 мс | 5.51M ops/s | 0.18 мкс |
| RHMap | 314.2 мс | 6.37M ops/s | 0.16 мкс |
| BTHashMap | 508.4 мс | 3.93M ops/s | 0.25 мкс |
| SwissTable | 280.5 мс | 7.13M ops/s | 0.14 мкс |

# Rnd insert (N=2,000,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 440.2 мс | 4.54M ops/s | 0.22 мкс |
| HashMap | 1885.9 мс | 1.06M ops/s | 0.94 мкс |
| RHMap | 1221.3 мс | 1.64M ops/s | 0.61 мкс |
| BTHashMap | 1585.0 мс | 1.26M ops/s | 0.79 мкс |
| SwissTable | 668.5 мс | 2.99M ops/s | 0.33 мкс |

# Lookup hit (N=2,000,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 658.9 мс | 3.04M ops/s | 0.33 мкс |
| HashMap | 972.0 мс | 2.06M ops/s | 0.49 мкс |
| RHMap | 400.2 мс | 5.00M ops/s | 0.20 мкс |
| BTHashMap | 931.3 мс | 2.15M ops/s | 0.47 мкс |
| SwissTable | 490.1 мс | 4.08M ops/s | 0.25 мкс |

# Lookup miss (N=2,000,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 268.3 мс | 7.45M ops/s | 0.13 мкс |
| HashMap | 224.0 мс | 8.93M ops/s | 0.11 мкс |
| RHMap | 236.0 мс | 8.48M ops/s | 0.12 мкс |
| BTHashMap | 1040.0 мс | 1.92M ops/s | 0.52 мкс |
| SwissTable | 484.4 мс | 4.13M ops/s | 0.24 мкс |

# Lookup Zipf (N=2,000,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 39.4 мс | 50.73M ops/s | 0.02 мкс |
| HashMap | 50.6 мс | 39.52M ops/s | 0.03 мкс |
| RHMap | 39.5 мс | 50.57M ops/s | 0.02 мкс |
| BTHashMap | 90.1 мс | 22.19M ops/s | 0.05 мкс |
| SwissTable | 88.7 мс | 22.54M ops/s | 0.04 мкс |

# Update all (N=2,000,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 396.9 мс | 5.04M ops/s | 0.20 мкс |
| HashMap | 669.6 мс | 2.99M ops/s | 0.33 мкс |
| RHMap | 424.0 мс | 4.72M ops/s | 0.21 мкс |
| BTHashMap | 951.9 мс | 2.10M ops/s | 0.48 мкс |
| SwissTable | 542.6 мс | 3.69M ops/s | 0.27 мкс |

# Insert+Delete (N=2,000,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 697.4 мс | 5.74M ops/s | 0.17 мкс |
| HashMap | 2343.9 мс | 1.71M ops/s | 0.59 мкс |
| RHMap | 1009.2 мс | 3.96M ops/s | 0.25 мкс |
| BTHashMap | 2483.1 мс | 1.61M ops/s | 0.62 мкс |
| SwissTable | 1144.1 мс | 3.50M ops/s | 0.29 мкс |

# Mixed 80R/20W (N=2,000,000)

| Структура | Время | Throughput | мкс/op |
| --- | --- | --- | --- |
| dict | 694.6 мс | 2.88M ops/s | 0.35 мкс |
| HashMap | 790.9 мс | 2.53M ops/s | 0.40 мкс |
| RHMap | 667120.3 мс | 3.0K ops/s | 333.56 мкс |
| BTHashMap | 1173.6 мс | 1.70M ops/s | 0.59 мкс |
| SwissTable | 507.8 мс | 3.94M ops/s | 0.25 мкс |

