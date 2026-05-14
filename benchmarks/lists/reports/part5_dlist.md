# DList specific operations (N=1,000)

| Операция | Время | Примечание |
| --- | --- | --- |
| move_to_front(index) | 0.4 мс | O(n) поиск + O(1) move |
| node handles LRU | 0.4 мс | O(1) operations |
| reversed iteration | 0.0 мс | native reverse iterator |

# DList specific operations (N=10,000)

| Операция | Время | Примечание |
| --- | --- | --- |
| move_to_front(index) | 94.7 мс | O(n) поиск + O(1) move |
| node handles LRU | 4.5 мс | O(1) operations |
| reversed iteration | 0.1 мс | native reverse iterator |

# DList specific operations (N=50,000)

| Операция | Время | Примечание |
| --- | --- | --- |
| move_to_front(index) | 3575.2 мс | O(n) поиск + O(1) move |
| node handles LRU | 22.2 мс | O(1) operations |
| reversed iteration | 0.4 мс | native reverse iterator |

