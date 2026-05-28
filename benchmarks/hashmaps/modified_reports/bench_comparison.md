# Lookup hit: int vs str vs Key/Value (N=500,000)

| Структура | int | str | Key/Value |
| --- | --- | --- | --- |
| dict | 125.3 мс | 125.3 мс | 182.0 мс |
| HashMap | 178.3 мс | 231.5 мс | 250.4 мс |
| RHMap | 101.0 мс | 108.3 мс | 162.0 мс |
| BTHashMap | 172.1 мс | 199.9 мс | 358.5 мс |
| SwissTable | 107.4 мс | 119.1 мс | 168.3 мс |

