# Key patterns impact (N=1,000)

| Структура | Seq ins | Rnd ins | Rnd look | Zipf look |
| --- | --- | --- | --- | --- |
| dict | 0.0 мс | 0.0 мс | 0.0 мс | 0.0 мс |
| HashMap | 0.1 мс | 0.2 мс | 0.0 мс | 0.0 мс |
| RHMap | 0.1 мс | 0.0 мс | 0.0 мс | 0.0 мс |
| BTHashMap | 0.1 мс | 0.2 мс | 0.0 мс | 0.0 мс |
| SwissTable | 0.1 мс | 0.2 мс | 0.0 мс | 0.0 мс |

# Key patterns impact (N=10,000)

| Структура | Seq ins | Rnd ins | Rnd look | Zipf look |
| --- | --- | --- | --- | --- |
| dict | 0.5 мс | 0.4 мс | 0.3 мс | 0.2 мс |
| HashMap | 2.2 мс | 1.9 мс | 0.5 мс | 0.2 мс |
| RHMap | 0.5 мс | 1.7 мс | 0.3 мс | 0.2 мс |
| BTHashMap | 1.9 мс | 2.4 мс | 0.7 мс | 0.3 мс |
| SwissTable | 1.3 мс | 1.3 мс | 0.6 мс | 0.5 мс |

# Key patterns impact (N=50,000)

| Структура | Seq ins | Rnd ins | Rnd look | Zipf look |
| --- | --- | --- | --- | --- |
| dict | 4.0 мс | 2.9 мс | 2.2 мс | 1.0 мс |
| HashMap | 9.6 мс | 22.1 мс | 5.2 мс | 1.3 мс |
| RHMap | 3.0 мс | 7.6 мс | 2.0 мс | 1.0 мс |
| BTHashMap | 9.5 мс | 14.0 мс | 4.9 мс | 2.0 мс |
| SwissTable | 5.2 мс | 7.1 мс | 3.6 мс | 2.3 мс |

# Key patterns impact (N=1,000)

| Структура | Seq ins | Rnd ins | Rnd look | Zipf look |
| --- | --- | --- | --- | --- |
| dict | 0.0 мс | 0.0 мс | 0.0 мс | 0.0 мс |
| HashMap | 0.1 мс | 0.2 мс | 0.0 мс | 0.0 мс |
| RHMap | 0.1 мс | 0.0 мс | 0.0 мс | 0.0 мс |
| BTHashMap | 0.2 мс | 0.1 мс | 0.1 мс | 0.0 мс |
| SwissTable | 0.1 мс | 0.1 мс | 0.0 мс | 0.0 мс |

# Key patterns impact (N=10,000)

| Структура | Seq ins | Rnd ins | Rnd look | Zipf look |
| --- | --- | --- | --- | --- |
| dict | 0.4 мс | 0.5 мс | 0.3 мс | 0.2 мс |
| HashMap | 1.4 мс | 1.7 мс | 0.5 мс | 0.2 мс |
| RHMap | 0.5 мс | 1.7 мс | 0.3 мс | 0.2 мс |
| BTHashMap | 1.8 мс | 2.4 мс | 0.7 мс | 0.3 мс |
| SwissTable | 1.5 мс | 1.2 мс | 0.6 мс | 0.5 мс |

# Key patterns impact (N=50,000)

| Структура | Seq ins | Rnd ins | Rnd look | Zipf look |
| --- | --- | --- | --- | --- |
| dict | 2.9 мс | 4.3 мс | 2.3 мс | 1.0 мс |
| HashMap | 9.4 мс | 32.9 мс | 9.9 мс | 1.3 мс |
| RHMap | 3.6 мс | 6.6 мс | 2.3 мс | 1.0 мс |
| BTHashMap | 8.8 мс | 12.5 мс | 5.0 мс | 2.0 мс |
| SwissTable | 5.3 мс | 6.9 мс | 3.6 мс | 2.4 мс |

# Key patterns impact (N=1,000,000)

| Структура | Seq ins | Rnd ins | Rnd look | Zipf look |
| --- | --- | --- | --- | --- |
| dict | 51.9 мс | 158.7 мс | 220.2 мс | 19.0 мс |
| HashMap | 190.1 мс | 1095.8 мс | 369.5 мс | 24.6 мс |
| RHMap | 82.3 мс | 194.3 мс | 181.6 мс | 20.0 мс |
| BTHashMap | 202.8 мс | 631.9 мс | 404.9 мс | 38.8 мс |
| SwissTable | 121.4 мс | 269.8 мс | 227.6 мс | 35.2 мс |

# Key patterns impact (N=2,000,000)

| Структура | Seq ins | Rnd ins | Rnd look | Zipf look |
| --- | --- | --- | --- | --- |
| dict | 130.1 мс | 366.9 мс | 549.4 мс | 38.0 мс |
| HashMap | 340.9 мс | 2362.0 мс | 788.1 мс | 48.5 мс |
| RHMap | 172.3 мс | 411.1 мс | 401.2 мс | 39.8 мс |
| BTHashMap | 426.9 мс | 1383.7 мс | 903.1 мс | 77.5 мс |
| SwissTable | 244.4 мс | 615.8 мс | 506.2 мс | 74.9 мс |

