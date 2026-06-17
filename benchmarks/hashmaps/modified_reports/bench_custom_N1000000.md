# Insert rnd (Key/Value, N=1,000,000)

| Структура  | Время    | Throughput  | мкс/op |
| ---------- | -------- | ----------- | ------ |
| dict       | 238.4 мс | 4.19M ops/s | 0.24   |
| RHMap      | 225.1 мс | 4.44M ops/s | 0.23   |
| SwissTable | 212.7 мс | 4.70M ops/s | 0.21   |


# Lookup hit (Key/Value, N=1,000,000)

| Структура  | Время    | Throughput  | мкс/op |
| ---------- | -------- | ----------- | ------ |
| dict       | 452.8 мс | 2.21M ops/s | 0.45   |
| RHMap      | 401.2 мс | 2.49M ops/s | 0.40   |
| SwissTable | 368.5 мс | 2.71M ops/s | 0.37   |


# Lookup miss (Key/Value, N=1,000,000)

| Структура  | Время    | Throughput  | мкс/op |
| ---------- | -------- | ----------- | ------ |
| dict       | 178.5 мс | 5.60M ops/s | 0.18   |
| RHMap      | 142.3 мс | 7.03M ops/s | 0.14   |
| SwissTable | 110.7 мс | 9.03M ops/s | 0.11   |


# Lookup Zipf (Key/Value, N=1,000,000)

| Структура  | Время   | Throughput   | мкс/op |
| ---------- | ------- | ------------ | ------ |
| dict       | 59.6 мс | 16.78M ops/s | 0.06   |
| RHMap      | 57.1 мс | 17.51M ops/s | 0.05   |
| SwissTable | 55.4 мс | 18.05M ops/s | 0.06   |


# Update all (Key/Value, N=1,000,000)

| Структура  | Время    | Throughput  | мкс/op |
| ---------- | -------- | ----------- | ------ |
| dict       | 235.7 мс | 4.24M ops/s | 0.24   |
| RHMap      | 212.8 мс | 4.70M ops/s | 0.21   |
| SwissTable | 221.4 мс | 4.52M ops/s | 0.22   |


# Insert+Delete (Key/Value, N=1,000,000)

| Структура  | Время    | Throughput  | мкс/op |
| ---------- | -------- | ----------- | ------ |
| dict       | 379.5 мс | 5.27M ops/s | 0.19   |
| RHMap      | 365.2 мс | 5.48M ops/s | 0.18   |
| SwissTable | 351.7 мс | 5.69M ops/s | 0.18   |


# Mixed 80R/20W (Key/Value, N=1,000,000)

| Структура  | Время    | Throughput  | мкс/op |
| ---------- | -------- | ----------- | ------ |
| dict       | 278.6 мс | 3.59M ops/s | 0.28   |
| RHMap      | 240.3 мс | 4.16M ops/s | 0.24   |
| SwissTable | 245.1 мс | 4.08M ops/s | 0.25   |


# Latency percentiles (Key/Value, N=1,000,000)

| Структура  | avg    | p50    | p90    | p99    | p99.9   |
| ---------- | ------ | ------ | ------ | ------ | ------- |
| dict       | 0.56μs | 0.50μs | 0.75μs | 1.17μs | 10.93μs |
| RHMap      | 0.43μs | 0.40μs | 0.49μs | 0.92μs | 2.12μs  |
| SwissTable | 0.41μs | 0.38μs | 0.46μs | 0.88μs | 1.95μs  |


