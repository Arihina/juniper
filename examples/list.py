from junipre import data_structures

lst = data_structures.slist_create()
data_structures.slist_push_front(lst, 10)
data_structures.slist_push_front(lst, 20)

print(data_structures.slist_pop_front(lst))
print(data_structures.slist_pop_front(lst))
