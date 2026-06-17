# Insert rnd (Key/Value, N=2,000,000)

| Структура  | Время    | Throughput  | мкс/op |
| ---------- | -------- | ----------- | ------ |
| dict       | 567.9 мс | 3.52M ops/s | 0.28   |
| RHMap      | 528.4 мс | 3.78M ops/s | 0.26   |
| SwissTable | 502.1 мс | 3.98M ops/s | 0.25   |


# Lookup hit (Key/Value, N=2,000,000)

| Структура  | Время     | Throughput  | мкс/op |
| ---------- | --------- | ----------- | ------ |
| dict       | 1053.1 мс | 1.90M ops/s | 0.53   |
| RHMap      | 691.9 мс  | 2.89M ops/s | 0.35   |
| SwissTable | 645.4 мс  | 3.10M ops/s | 0.32   |


# Lookup miss (Key/Value, N=2,000,000)

| Структура  | Время    | Throughput  | мкс/op |
| ---------- | -------- | ----------- | ------ |
| dict       | 395.7 мс | 5.05M ops/s | 0.20   |
| RHMap      | 360.2 мс | 5.55M ops/s | 0.18   |
| SwissTable | 219.2 мс | 9.12M ops/s | 0.11   |


# Lookup Zipf (Key/Value, N=2,000,000)

| Структура  | Время    | Throughput   | мкс/op |
| ---------- | -------- | ------------ | ------ |
| dict       | 173.0 мс | 11.56M ops/s | 0.09   |
| RHMap      | 161.8 мс | 12.36M ops/s | 0.08   |
| SwissTable | 156.2 мс | 12.80M ops/s | 0.08   |


# Update all (Key/Value, N=2,000,000)

| Структура  | Время    | Throughput  | мкс/op |
| ---------- | -------- | ----------- | ------ |
| dict       | 504.1 мс | 3.97M ops/s | 0.25   |
| RHMap      | 486.5 мс | 4.11M ops/s | 0.24   |
| SwissTable | 522.4 мс | 3.83M ops/s | 0.26   |


# Insert+Delete (Key/Value, N=2,000,000)

| Структура  | Время    | Throughput  | мкс/op |
| ---------- | -------- | ----------- | ------ |
| dict       | 864.2 мс | 4.63M ops/s | 0.22   |
| RHMap      | 798.6 мс | 5.01M ops/s | 0.20   |
| SwissTable | 776.4 мс | 5.15M ops/s | 0.19   |


# Mixed 80R/20W (Key/Value, N=2,000,000)

| Структура  | Время    | Throughput  | мкс/op |
| ---------- | -------- | ----------- | ------ |
| dict       | 598.9 мс | 3.34M ops/s | 0.30   |
| RHMap      | 507.3 мс | 3.94M ops/s | 0.25   |
| SwissTable | 558.0 мс | 3.58M ops/s | 0.28   |


# Latency percentiles (Key/Value, N=2,000,000)

| Структура  | avg    | p50    | p90    | p99    | p99.9  |
| ---------- | ------ | ------ | ------ | ------ | ------ |
| dict       | 0.60μs | 0.53μs | 0.83μs | 1.25μs | 6.15μs |
| RHMap      | 0.46μs | 0.43μs | 0.51μs | 0.96μs | 1.72μs |
| SwissTable | 0.53μs | 0.49μs | 0.58μs | 1.08μs | 2.32μs |
