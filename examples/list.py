from junipre import _native

lst = _native.create()
_native.push_front(lst, 10)
_native.push_front(lst, 20)

print(_native.pop_front(lst))
print(_native.pop_front(lst))
