# Lookup hit: int vs str vs Key/Value (N=500,000)

| Структура | int | str | Key/Value |
| --- | --- | --- | --- |
| dict | 125.3 мс | 125.3 мс | 182.0 мс |
| HashMap | 178.3 мс | 231.5 мс | 250.4 мс |
| RHMap | 101.0 мс | 108.3 мс | 162.0 мс |
| BTHashMap | 172.1 мс | 199.9 мс | 358.5 мс |
| SwissTable | 107.4 мс | 119.1 мс | 168.3 мс |

# Lookup hit: int vs str vs Key/Value (N=500,000)

| Структура | int | str | Key/Value |
| --- | --- | --- | --- |
| dict | 152.8 мс | 185.5 мс | 230.4 мс |
| HashMap | 229.2 мс | 251.8 мс | 333.1 мс |
| RHMap | 126.8 мс | 135.3 мс | 195.2 мс |
| BTHashMap | 279.2 мс | 337.8 мс | 526.2 мс |
| SwissTable | 137.9 мс | 152.5 мс | 185.9 мс |

# Lookup hit: int vs str vs Key/Value (N=500,000)

| Структура | int | str | Key/Value |
| --- | --- | --- | --- |
| dict | 115.8 мс | 128.6 мс | 190.8 мс |
| RHMap | 110.4 мс | 112.2 мс | 158.6 мс |
| SwissTable | 119.3 мс | 113.8 мс | 182.6 мс |

