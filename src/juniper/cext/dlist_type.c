#include <Python.h>
#include "dlist.h"

extern PyTypeObject PyDListNodeType;
static PyTypeObject PyDListIterType;
static PyTypeObject PyDListRevIterType;

typedef struct
{
    PyObject_HEAD DList *list;
} PyDListObject;

typedef struct
{
    PyObject_HEAD DListNode *node;
    PyDListObject *owner;
} PyDListNodeObject;

static int node_is_attached(PyDListNodeObject *self)
{
    return self->node != NULL && self->owner != NULL;
}

static PyDListNodeObject *wrap_node(PyDListObject *owner, DListNode *raw)
{
    if (!raw)
        return NULL;
    if (PyType_Ready(&PyDListNodeType) < 0)
        return NULL;
    PyDListNodeObject *h = PyObject_New(PyDListNodeObject, &PyDListNodeType);
    if (!h)
        return NULL;
    Py_INCREF(owner);
    h->owner = owner;
    h->node = raw;
    return h;
}

static void detach_handle(PyDListNodeObject *h)
{
    Py_XDECREF(h->owner);
    h->owner = NULL;
    h->node = NULL;
}

static void PyDListNode_dealloc(PyDListNodeObject *self)
{
    Py_XDECREF(self->owner);
    Py_TYPE(self)->tp_free((PyObject *)self);
}

static PyObject *PyDListNode_get_value(PyDListNodeObject *self, void *closure)
{
    (void)closure;
    if (!self->node)
    {
        PyErr_SetString(PyExc_RuntimeError, "node is detached");
        return NULL;
    }
    Py_INCREF(self->node->value);
    return self->node->value;
}

static int PyDListNode_set_value(PyDListNodeObject *self,
                                 PyObject *value, void *closure)
{
    (void)closure;
    if (!self->node)
    {
        PyErr_SetString(PyExc_RuntimeError, "node is detached");
        return -1;
    }
    if (!value)
    {
        PyErr_SetString(PyExc_AttributeError, "cannot delete value");
        return -1;
    }
    Py_INCREF(value);
    Py_DECREF(self->node->value);
    self->node->value = value;
    return 0;
}

static PyObject *PyDListNode_get_is_attached(PyDListNodeObject *self,
                                             void *closure)
{
    (void)closure;
    return PyBool_FromLong(node_is_attached(self));
}

static PyGetSetDef PyDListNode_getset[] = {
    {"value", (getter)PyDListNode_get_value, (setter)PyDListNode_set_value,
     "The value stored in this node.", NULL},
    {"is_attached", (getter)PyDListNode_get_is_attached, NULL,
     "True if this node is still linked in a DList.", NULL},
    {NULL}};

static PyObject *PyDListNode_repr(PyDListNodeObject *self)
{
    if (!self->node)
        return PyUnicode_FromString("DListNode(<detached>)");
    PyObject *vr = PyObject_Repr(self->node->value);
    if (!vr)
        return NULL;
    PyObject *result = PyUnicode_FromFormat("DListNode(%U)", vr);
    Py_DECREF(vr);
    return result;
}

PyTypeObject PyDListNodeType = {
    PyVarObject_HEAD_INIT(NULL, 0)
        .tp_name = "juniper._native.DListNode",
    .tp_basicsize = sizeof(PyDListNodeObject),
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_dealloc = (destructor)PyDListNode_dealloc,
    .tp_getset = PyDListNode_getset,
    .tp_repr = (reprfunc)PyDListNode_repr,
    .tp_doc = "Handle to a node in a DList. Has .value (r/w) and .is_attached (r/o).",
};

typedef struct
{
    PyObject_HEAD PyDListObject *owner;
    DListNode *cur;
    size_t snap_size;
} PyDListIterObject;

static void PyDListIter_dealloc(PyDListIterObject *it)
{
    Py_XDECREF(it->owner);
    Py_TYPE(it)->tp_free((PyObject *)it);
}

static PyObject *PyDListIter_next(PyDListIterObject *it)
{
    if (!it->owner || !it->owner->list)
        return NULL;
    if (it->owner->list->size != it->snap_size)
    {
        PyErr_SetString(PyExc_RuntimeError,
                        "DList changed size during iteration");
        return NULL;
    }
    if (!it->cur)
        return NULL;
    PyObject *val = it->cur->value;
    Py_INCREF(val);
    it->cur = it->cur->next;
    return val;
}

static PyObject *PyDListIter_iter(PyObject *self)
{
    Py_INCREF(self);
    return self;
}

static PyTypeObject PyDListIterType = {
    PyVarObject_HEAD_INIT(NULL, 0)
        .tp_name = "juniper._native.DListIterator",
    .tp_basicsize = sizeof(PyDListIterObject),
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_dealloc = (destructor)PyDListIter_dealloc,
    .tp_iter = PyDListIter_iter,
    .tp_iternext = (iternextfunc)PyDListIter_next,
};

typedef struct
{
    PyObject_HEAD PyDListObject *owner;
    DListNode *cur;
    size_t snap_size;
} PyDListRevIterObject;

static void PyDListRevIter_dealloc(PyDListRevIterObject *it)
{
    Py_XDECREF(it->owner);
    Py_TYPE(it)->tp_free((PyObject *)it);
}

static PyObject *PyDListRevIter_next(PyDListRevIterObject *it)
{
    if (!it->owner || !it->owner->list)
        return NULL;
    if (it->owner->list->size != it->snap_size)
    {
        PyErr_SetString(PyExc_RuntimeError,
                        "DList changed size during iteration");
        return NULL;
    }
    if (!it->cur)
        return NULL;
    PyObject *val = it->cur->value;
    Py_INCREF(val);
    it->cur = it->cur->prev;
    return val;
}

static PyObject *PyDListRevIter_iter(PyObject *self)
{
    Py_INCREF(self);
    return self;
}

static PyTypeObject PyDListRevIterType = {
    PyVarObject_HEAD_INIT(NULL, 0)
        .tp_name = "juniper._native.DListReverseIterator",
    .tp_basicsize = sizeof(PyDListRevIterObject),
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_dealloc = (destructor)PyDListRevIter_dealloc,
    .tp_iter = PyDListRevIter_iter,
    .tp_iternext = (iternextfunc)PyDListRevIter_next,
};

static PyObject *PyDList_new(PyTypeObject *type,
                             PyObject *args, PyObject *kwds)
{
    PyDListObject *self;
    PyObject *iterable = NULL;
    if (!PyArg_ParseTuple(args, "|O", &iterable))
        return NULL;

    self = (PyDListObject *)type->tp_alloc(type, 0);
    if (!self)
        return NULL;
    self->list = dlist_create();
    if (!self->list)
    {
        Py_DECREF(self);
        return PyErr_NoMemory();
    }

    if (iterable)
    {
        PyObject *iter = PyObject_GetIter(iterable);
        if (!iter)
        {
            Py_DECREF(self);
            return NULL;
        }
        PyObject *item;
        while ((item = PyIter_Next(iter)))
        {
            int rc = dlist_push_back(self->list, item);
            Py_DECREF(item);
            if (rc < 0)
            {
                Py_DECREF(iter);
                Py_DECREF(self);
                return PyErr_NoMemory();
            }
        }
        Py_DECREF(iter);
        if (PyErr_Occurred())
        {
            Py_DECREF(self);
            return NULL;
        }
    }
    return (PyObject *)self;
}

static void PyDList_dealloc(PyDListObject *self)
{
    if (self->list)
        dlist_free(self->list);
    Py_TYPE(self)->tp_free((PyObject *)self);
}

static Py_ssize_t PyDList_len(PyDListObject *self)
{
    return (Py_ssize_t)self->list->size;
}

static int PyDList_sq_contains(PyDListObject *self, PyObject *value)
{
    return dlist_contains(self->list, value);
}

static Py_ssize_t normalize_index(PyDListObject *self, Py_ssize_t i)
{
    Py_ssize_t sz = (Py_ssize_t)self->list->size;
    if (i < 0)
        i += sz;
    if (i < 0 || i >= sz)
        return -1;
    return i;
}

static PyObject *PyDList_getitem(PyDListObject *self, Py_ssize_t i)
{
    i = normalize_index(self, i);
    if (i < 0)
    {
        PyErr_SetString(PyExc_IndexError, "DList index out of range");
        return NULL;
    }
    return dlist_get(self->list, (size_t)i);
}

static PySequenceMethods PyDList_as_sequence = {
    .sq_length = (lenfunc)PyDList_len,
    .sq_item = (ssizeargfunc)PyDList_getitem,
    .sq_contains = (objobjproc)PyDList_sq_contains,
};

#define CHECK_NODE_HANDLE(self, h, ret)                                      \
    do                                                                       \
    {                                                                        \
        if (!PyObject_TypeCheck((h), &PyDListNodeType))                      \
        {                                                                    \
            PyErr_SetString(PyExc_TypeError, "expected a DListNode handle"); \
            return (ret);                                                    \
        }                                                                    \
        if (!(h)->node || (h)->owner != (self))                              \
        {                                                                    \
            PyErr_SetString(PyExc_ValueError,                                \
                            "node is detached or belongs to another list");  \
            return (ret);                                                    \
        }                                                                    \
    } while (0)

static PyObject *PyDList_push_front(PyDListObject *self, PyObject *args)
{
    PyObject *value;
    if (!PyArg_ParseTuple(args, "O", &value))
        return NULL;
    if (dlist_push_front(self->list, value) < 0)
        return PyErr_NoMemory();
    Py_RETURN_NONE;
}

static PyObject *PyDList_push_back(PyDListObject *self, PyObject *args)
{
    PyObject *value;
    if (!PyArg_ParseTuple(args, "O", &value))
        return NULL;
    if (dlist_push_back(self->list, value) < 0)
        return PyErr_NoMemory();
    Py_RETURN_NONE;
}

static PyObject *PyDList_insert(PyDListObject *self, PyObject *args)
{
    Py_ssize_t index;
    PyObject *value;
    if (!PyArg_ParseTuple(args, "nO", &index, &value))
        return NULL;
    if (index < 0)
        index += (Py_ssize_t)self->list->size;
    if (index < 0)
        index = 0;
    if (dlist_insert(self->list, (size_t)index, value) < 0)
        return PyErr_NoMemory();
    Py_RETURN_NONE;
}

static PyObject *PyDList_pop_front(PyDListObject *self, PyObject *Py_UNUSED(ig))
{
    PyObject *val = dlist_pop_front(self->list);
    if (!val)
    {
        PyErr_SetString(PyExc_IndexError, "pop from empty DList");
        return NULL;
    }
    return val;
}

static PyObject *PyDList_pop_back(PyDListObject *self, PyObject *Py_UNUSED(ig))
{
    PyObject *val = dlist_pop_back(self->list);
    if (!val)
    {
        PyErr_SetString(PyExc_IndexError, "pop from empty DList");
        return NULL;
    }
    return val;
}

static PyObject *PyDList_remove(PyDListObject *self, PyObject *args)
{
    PyObject *value;
    if (!PyArg_ParseTuple(args, "O", &value))
        return NULL;
    int rc = dlist_remove(self->list, value);
    if (rc < 0)
        return NULL;
    if (rc == 0)
    {
        PyErr_SetObject(PyExc_ValueError, value);
        return NULL;
    }
    Py_RETURN_NONE;
}

static PyObject *PyDList_index_method(PyDListObject *self, PyObject *args)
{
    PyObject *value;
    if (!PyArg_ParseTuple(args, "O", &value))
        return NULL;
    Py_ssize_t idx = dlist_index(self->list, value);
    if (idx == -2)
        return NULL;
    if (idx == -1)
    {
        PyErr_SetObject(PyExc_ValueError, value);
        return NULL;
    }
    return PyLong_FromSsize_t(idx);
}

static PyObject *PyDList_clear(PyDListObject *self, PyObject *Py_UNUSED(ig))
{
    dlist_clear(self->list);
    Py_RETURN_NONE;
}

static PyObject *PyDList_reverse(PyDListObject *self, PyObject *Py_UNUSED(ig))
{
    dlist_reverse(self->list);
    Py_RETURN_NONE;
}

static PyObject *PyDList_is_empty(PyDListObject *self, PyObject *Py_UNUSED(ig))
{
    return PyBool_FromLong(dlist_is_empty(self->list));
}

static PyObject *PyDList__len(PyDListObject *self, PyObject *Py_UNUSED(ig))
{
    return PyLong_FromSize_t(dlist_len(self->list));
}

static PyObject *PyDList_move_to_front_idx(PyDListObject *self, PyObject *args)
{
    Py_ssize_t index;
    if (!PyArg_ParseTuple(args, "n", &index))
        return NULL;
    Py_ssize_t sz = (Py_ssize_t)self->list->size;
    if (index < 0)
        index += sz;
    if (index < 0 || index >= sz)
    {
        PyErr_SetString(PyExc_IndexError, "DList index out of range");
        return NULL;
    }
    DListNode *node;
    if ((size_t)index <= self->list->size / 2)
    {
        node = self->list->head;
        for (Py_ssize_t i = 0; i < index; i++)
            node = node->next;
    }
    else
    {
        node = self->list->tail;
        for (Py_ssize_t i = sz - 1; i > index; i--)
            node = node->prev;
    }
    dlist_move_to_front(self->list, node);
    Py_RETURN_NONE;
}

static PyObject *PyDList_push_front_node(PyDListObject *self, PyObject *args)
{
    PyObject *value;
    if (!PyArg_ParseTuple(args, "O", &value))
        return NULL;
    DListNode *raw = dlist_push_front_node(self->list, value);
    if (!raw)
        return PyErr_NoMemory();
    PyDListNodeObject *h = wrap_node(self, raw);
    if (!h)
        return NULL;
    return (PyObject *)h;
}

static PyObject *PyDList_push_back_node(PyDListObject *self, PyObject *args)
{
    PyObject *value;
    if (!PyArg_ParseTuple(args, "O", &value))
        return NULL;
    DListNode *raw = dlist_push_back_node(self->list, value);
    if (!raw)
        return PyErr_NoMemory();
    PyDListNodeObject *h = wrap_node(self, raw);
    if (!h)
        return NULL;
    return (PyObject *)h;
}

static PyObject *PyDList_front_node(PyDListObject *self, PyObject *Py_UNUSED(ig))
{
    if (!self->list->head)
        Py_RETURN_NONE;
    PyDListNodeObject *h = wrap_node(self, self->list->head);
    if (!h)
        return NULL;
    return (PyObject *)h;
}

static PyObject *PyDList_back_node(PyDListObject *self, PyObject *Py_UNUSED(ig))
{
    if (!self->list->tail)
        Py_RETURN_NONE;
    PyDListNodeObject *h = wrap_node(self, self->list->tail);
    if (!h)
        return NULL;
    return (PyObject *)h;
}

static PyObject *PyDList_pop_front_node(PyDListObject *self, PyObject *Py_UNUSED(ig))
{
    if (!self->list->head)
    {
        PyErr_SetString(PyExc_IndexError, "pop from empty DList");
        return NULL;
    }
    DListNode *raw = self->list->head;

    PyDListNodeObject *h = wrap_node(self, raw);
    if (!h)
        return NULL;
    dlist_unlink_keep(self->list, raw);

    h->node = raw;
    h->owner = NULL;
    Py_DECREF(self);
    return (PyObject *)h;
}

static PyObject *PyDList_pop_back_node(PyDListObject *self, PyObject *Py_UNUSED(ig))
{
    if (!self->list->tail)
    {
        PyErr_SetString(PyExc_IndexError, "pop from empty DList");
        return NULL;
    }
    DListNode *raw = self->list->tail;
    PyDListNodeObject *h = wrap_node(self, raw);
    if (!h)
        return NULL;
    dlist_unlink_keep(self->list, raw);
    h->node = raw;
    h->owner = NULL;
    Py_DECREF(self);
    return (PyObject *)h;
}

static PyObject *PyDList_move_to_front_node(PyDListObject *self, PyObject *args)
{
    PyDListNodeObject *h;
    if (!PyArg_ParseTuple(args, "O!", &PyDListNodeType, &h))
        return NULL;
    CHECK_NODE_HANDLE(self, h, NULL);
    dlist_move_to_front(self->list, h->node);
    Py_RETURN_NONE;
}

static PyObject *PyDList_move_to_back_node(PyDListObject *self, PyObject *args)
{
    PyDListNodeObject *h;
    if (!PyArg_ParseTuple(args, "O!", &PyDListNodeType, &h))
        return NULL;
    CHECK_NODE_HANDLE(self, h, NULL);
    dlist_move_to_back(self->list, h->node);
    Py_RETURN_NONE;
}

static PyObject *PyDList_unlink_node(PyDListObject *self, PyObject *args)
{
    PyDListNodeObject *h;
    if (!PyArg_ParseTuple(args, "O!", &PyDListNodeType, &h))
        return NULL;
    CHECK_NODE_HANDLE(self, h, NULL);

    PyObject *value = h->node->value;
    Py_INCREF(value);
    dlist_unlink_keep(self->list, h->node);

    Py_DECREF(h->node->value);
    free(h->node);
    detach_handle(h);

    return value;
}

static PyObject *PyDList_reversed(PyDListObject *self, PyObject *Py_UNUSED(ig))
{
    if (PyType_Ready(&PyDListRevIterType) < 0)
        return NULL;
    PyDListRevIterObject *it = PyObject_New(PyDListRevIterObject, &PyDListRevIterType);
    if (!it)
        return NULL;
    Py_INCREF(self);
    it->owner = self;
    it->cur = self->list->tail;
    it->snap_size = self->list->size;
    return (PyObject *)it;
}

static PyMethodDef PyDList_methods[] = {
    {"push_front", (PyCFunction)PyDList_push_front, METH_VARARGS,
     "Add an element to the front."},
    {"push_back", (PyCFunction)PyDList_push_back, METH_VARARGS,
     "Add an element to the back."},
    {"insert", (PyCFunction)PyDList_insert, METH_VARARGS,
     "Insert an element at the given index."},
    {"pop_front", (PyCFunction)PyDList_pop_front, METH_NOARGS,
     "Remove and return the first element."},
    {"pop_back", (PyCFunction)PyDList_pop_back, METH_NOARGS,
     "Remove and return the last element."},
    {"remove", (PyCFunction)PyDList_remove, METH_VARARGS,
     "Remove first occurrence; raise ValueError if missing."},
    {"index", (PyCFunction)PyDList_index_method, METH_VARARGS,
     "Return index of first occurrence; raise ValueError if missing."},
    {"clear", (PyCFunction)PyDList_clear, METH_NOARGS,
     "Remove all elements."},
    {"reverse", (PyCFunction)PyDList_reverse, METH_NOARGS,
     "Reverse the list in-place."},
    {"move_to_front", (PyCFunction)PyDList_move_to_front_idx, METH_VARARGS,
     "Move element at given index to the front."},
    {"is_empty", (PyCFunction)PyDList_is_empty, METH_NOARGS,
     "Return True if list is empty."},
    {"_len", (PyCFunction)PyDList__len, METH_NOARGS,
     "Return number of elements."},
    {"__reversed__", (PyCFunction)PyDList_reversed, METH_NOARGS,
     "Return a reverse iterator."},

    {"push_front_node", (PyCFunction)PyDList_push_front_node, METH_VARARGS,
     "Insert value at front, return DListNode handle."},
    {"push_back_node", (PyCFunction)PyDList_push_back_node, METH_VARARGS,
     "Insert value at back, return DListNode handle."},
    {"front_node", (PyCFunction)PyDList_front_node, METH_NOARGS,
     "Return handle to head node, or None if empty."},
    {"back_node", (PyCFunction)PyDList_back_node, METH_NOARGS,
     "Return handle to tail node, or None if empty."},
    {"pop_front_node", (PyCFunction)PyDList_pop_front_node, METH_NOARGS,
     "Unlink head node and return its (detached) handle."},
    {"pop_back_node", (PyCFunction)PyDList_pop_back_node, METH_NOARGS,
     "Unlink tail node and return its (detached) handle."},
    {"move_to_front_node", (PyCFunction)PyDList_move_to_front_node, METH_VARARGS,
     "O(1) move node handle to front."},
    {"move_to_back_node", (PyCFunction)PyDList_move_to_back_node, METH_VARARGS,
     "O(1) move node handle to back."},
    {"unlink_node", (PyCFunction)PyDList_unlink_node, METH_VARARGS,
     "Unlink node by handle, return its value. Handle becomes detached."},
    {NULL}};

static PyObject *PyDList_iter(PyDListObject *self)
{
    if (PyType_Ready(&PyDListIterType) < 0)
        return NULL;
    PyDListIterObject *it = PyObject_New(PyDListIterObject, &PyDListIterType);
    if (!it)
        return NULL;
    Py_INCREF(self);
    it->owner = self;
    it->cur = self->list->head;
    it->snap_size = self->list->size;
    return (PyObject *)it;
}

static PyObject *PyDList_repr(PyDListObject *self)
{
    if (self->list->size == 0)
        return PyUnicode_FromString("DList([])");

    PyObject *parts = PyList_New(0);
    if (!parts)
        return NULL;
    DListNode *cur = self->list->head;
    while (cur)
    {
        PyObject *r = PyObject_Repr(cur->value);
        if (!r)
        {
            Py_DECREF(parts);
            return NULL;
        }
        if (PyList_Append(parts, r) < 0)
        {
            Py_DECREF(r);
            Py_DECREF(parts);
            return NULL;
        }
        Py_DECREF(r);
        cur = cur->next;
    }
    PyObject *sep = PyUnicode_FromString(", ");
    PyObject *joined = PyUnicode_Join(sep, parts);
    Py_DECREF(sep);
    Py_DECREF(parts);
    if (!joined)
        return NULL;
    PyObject *result = PyUnicode_FromFormat("DList([%U])", joined);
    Py_DECREF(joined);
    return result;
}

static PyObject *PyDList_richcmp(PyObject *self, PyObject *other, int op)
{
    if (op != Py_EQ && op != Py_NE)
        Py_RETURN_NOTIMPLEMENTED;
    if (!PyObject_TypeCheck(other, Py_TYPE(self)))
        Py_RETURN_NOTIMPLEMENTED;

    PyDListObject *a = (PyDListObject *)self;
    PyDListObject *b = (PyDListObject *)other;
    if (a->list->size != b->list->size)
    {
        if (op == Py_EQ)
            Py_RETURN_FALSE;
        Py_RETURN_TRUE;
    }
    DListNode *na = a->list->head, *nb = b->list->head;
    while (na)
    {
        int eq = PyObject_RichCompareBool(na->value, nb->value, Py_EQ);
        if (eq < 0)
            return NULL;
        if (!eq)
        {
            if (op == Py_EQ)
                Py_RETURN_FALSE;
            Py_RETURN_TRUE;
        }
        na = na->next;
        nb = nb->next;
    }
    if (op == Py_EQ)
        Py_RETURN_TRUE;
    Py_RETURN_FALSE;
}

PyTypeObject PyDListType = {
    PyVarObject_HEAD_INIT(NULL, 0)
        .tp_name = "juniper._native.DList",
    .tp_basicsize = sizeof(PyDListObject),
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    .tp_new = PyDList_new,
    .tp_dealloc = (destructor)PyDList_dealloc,
    .tp_methods = PyDList_methods,
    .tp_as_sequence = &PyDList_as_sequence,
    .tp_iter = (getiterfunc)PyDList_iter,
    .tp_repr = (reprfunc)PyDList_repr,
    .tp_richcompare = PyDList_richcmp,
    .tp_doc = "Doubly linked list implemented in C.",
};