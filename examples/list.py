from juniper import SList

lst = SList()

lst.push_front(10)
lst.push_front(20)

print(lst.pop_front())
print(lst.pop_front())
