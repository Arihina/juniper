#include <Python.h>
#include "slist.h"

typedef struct {
    PyObject_HEAD
    SList *list;
} PySListObject;

typedef struct {
    PyObject_HEAD
    PySListObject *owner;
    SListNode     *cur;
    size_t         snap_size;
} PySListIterObject;

static PyTypeObject PySListIterType;

static void PySListIter_dealloc(PySListIterObject *it)
{
    Py_XDECREF(it->owner);
    Py_TYPE(it)->tp_free((PyObject *)it);
}

static PyObject *PySListIter_next(PySListIterObject *it)
{
    if (!it->owner || !it->owner->list)
        return NULL;

    if (it->owner->list->size != it->snap_size) {
        PyErr_SetString(PyExc_RuntimeError,
                        "SList changed size during iteration");
        return NULL;
    }

    if (!it->cur)
        return NULL;

    PyObject *val = it->cur->value;
    Py_INCREF(val);
    it->cur = it->cur->next;
    return val;
}

static PyObject *PySListIter_iter(PyObject *self)
{
    Py_INCREF(self);
    return self;
}

static PyTypeObject PySListIterType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name      = "juniper._native.SListIterator",
    .tp_basicsize = sizeof(PySListIterObject),
    .tp_flags     = Py_TPFLAGS_DEFAULT,
    .tp_dealloc   = (destructor)PySListIter_dealloc,
    .tp_iter      = PySListIter_iter,
    .tp_iternext  = (iternextfunc)PySListIter_next,
};

static PyObject *PySList_new(PyTypeObject *type,
                             PyObject *args, PyObject *kwds)
{
    PySListObject *self;
    PyObject *iterable = NULL;

    if (!PyArg_ParseTuple(args, "|O", &iterable))
        return NULL;

    self = (PySListObject *)type->tp_alloc(type, 0);
    if (!self) return NULL;

    self->list = slist_create();
    if (!self->list) {
        Py_DECREF(self);
        return PyErr_NoMemory();
    }

    if (iterable) {
        PyObject *iter = PyObject_GetIter(iterable);
        if (!iter) { Py_DECREF(self); return NULL; }
        PyObject *item;
        while ((item = PyIter_Next(iter))) {
            int rc = slist_push_back(self->list, item);
            Py_DECREF(item);
            if (rc < 0) {
                Py_DECREF(iter);
                Py_DECREF(self);
                return PyErr_NoMemory();
            }
        }
        Py_DECREF(iter);
        if (PyErr_Occurred()) { Py_DECREF(self); return NULL; }
    }

    return (PyObject *)self;
}

static void PySList_dealloc(PySListObject *self)
{
    if (self->list)
        slist_free(self->list);
    Py_TYPE(self)->tp_free((PyObject *)self);
}

static Py_ssize_t PySList_len(PySListObject *self)
{
    return (Py_ssize_t)self->list->size;
}

static int PySList_sq_contains(PySListObject *self, PyObject *value)
{
    return slist_contains(self->list, value);
}

static Py_ssize_t normalize_index(PySListObject *self, Py_ssize_t i)
{
    Py_ssize_t sz = (Py_ssize_t)self->list->size;
    if (i < 0) i += sz;
    if (i < 0 || i >= sz) return -1;
    return i;
}

static PyObject *PySList_getitem(PySListObject *self, Py_ssize_t i)
{
    i = normalize_index(self, i);
    if (i < 0) {
        PyErr_SetString(PyExc_IndexError, "SList index out of range");
        return NULL;
    }
    return slist_get(self->list, (size_t)i);
}

static PySequenceMethods PySList_as_sequence = {
    .sq_length   = (lenfunc)PySList_len,
    .sq_item     = (ssizeargfunc)PySList_getitem,
    .sq_contains = (objobjproc)PySList_sq_contains,
};


static PyObject *PySList_push_front(PySListObject *self, PyObject *args)
{
    PyObject *value;
    if (!PyArg_ParseTuple(args, "O", &value))
        return NULL;
    if (slist_push_front(self->list, value) < 0)
        return PyErr_NoMemory();
    Py_RETURN_NONE;
}

static PyObject *PySList_push_back(PySListObject *self, PyObject *args)
{
    PyObject *value;
    if (!PyArg_ParseTuple(args, "O", &value))
        return NULL;
    if (slist_push_back(self->list, value) < 0)
        return PyErr_NoMemory();
    Py_RETURN_NONE;
}

static PyObject *PySList_insert(PySListObject *self, PyObject *args)
{
    Py_ssize_t index;
    PyObject *value;
    if (!PyArg_ParseTuple(args, "nO", &index, &value))
        return NULL;
    if (index < 0)
        index += (Py_ssize_t)self->list->size;
    if (index < 0) index = 0;
    if (slist_insert(self->list, (size_t)index, value) < 0)
        return PyErr_NoMemory();
    Py_RETURN_NONE;
}

static PyObject *PySList_pop_front(PySListObject *self,
                                   PyObject *Py_UNUSED(ignored))
{
    PyObject *val = slist_pop_front(self->list);
    if (!val) {
        PyErr_SetString(PyExc_IndexError, "pop from empty SList");
        return NULL;
    }
    return val;
}

static PyObject *PySList_pop_back(PySListObject *self,
                                  PyObject *Py_UNUSED(ignored))
{
    PyObject *val = slist_pop_back(self->list);
    if (!val) {
        PyErr_SetString(PyExc_IndexError, "pop from empty SList");
        return NULL;
    }
    return val;
}

static PyObject *PySList_remove(PySListObject *self, PyObject *args)
{
    PyObject *value;
    if (!PyArg_ParseTuple(args, "O", &value))
        return NULL;
    int rc = slist_remove(self->list, value);
    if (rc < 0) return NULL;
    if (rc == 0) {
        PyErr_SetObject(PyExc_ValueError, value);
        return NULL;
    }
    Py_RETURN_NONE;
}

static PyObject *PySList_index_method(PySListObject *self, PyObject *args)
{
    PyObject *value;
    if (!PyArg_ParseTuple(args, "O", &value))
        return NULL;
    Py_ssize_t idx = slist_index(self->list, value);
    if (idx == -2) return NULL;
    if (idx == -1) {
        PyErr_SetObject(PyExc_ValueError, value);
        return NULL;
    }
    return PyLong_FromSsize_t(idx);
}

static PyObject *PySList_clear(PySListObject *self,
                               PyObject *Py_UNUSED(ignored))
{
    slist_clear(self->list);
    Py_RETURN_NONE;
}

static PyObject *PySList_reverse(PySListObject *self,
                                 PyObject *Py_UNUSED(ignored))
{
    slist_reverse(self->list);
    Py_RETURN_NONE;
}

static PyObject *PySList_is_empty(PySListObject *self,
                                  PyObject *Py_UNUSED(ignored))
{
    return PyBool_FromLong(slist_is_empty(self->list));
}

static PyObject *PySList__len(PySListObject *self,
                              PyObject *Py_UNUSED(ignored))
{
    return PyLong_FromSize_t(slist_len(self->list));
}

static PyMethodDef PySList_methods[] = {
    {"push_front", (PyCFunction)PySList_push_front, METH_VARARGS,
     "Add an element to the front."},
    {"push_back",  (PyCFunction)PySList_push_back,  METH_VARARGS,
     "Add an element to the back."},
    {"insert",     (PyCFunction)PySList_insert,     METH_VARARGS,
     "Insert an element at the given index."},
    {"pop_front",  (PyCFunction)PySList_pop_front,  METH_NOARGS,
     "Remove and return the first element."},
    {"pop_back",   (PyCFunction)PySList_pop_back,   METH_NOARGS,
     "Remove and return the last element."},
    {"remove",     (PyCFunction)PySList_remove,     METH_VARARGS,
     "Remove first occurrence of value; raise ValueError if missing."},
    {"index",      (PyCFunction)PySList_index_method, METH_VARARGS,
     "Return index of first occurrence; raise ValueError if missing."},
    {"clear",      (PyCFunction)PySList_clear,      METH_NOARGS,
     "Remove all elements."},
    {"reverse",    (PyCFunction)PySList_reverse,    METH_NOARGS,
     "Reverse the list in-place."},
    {"is_empty",   (PyCFunction)PySList_is_empty,   METH_NOARGS,
     "Return True if list is empty."},
    {"_len",       (PyCFunction)PySList__len,       METH_NOARGS,
     "Return number of elements (method form of len())."},
    {NULL}
};

static PyObject *PySList_iter(PySListObject *self)
{
    if (PyType_Ready(&PySListIterType) < 0)
        return NULL;

    PySListIterObject *it = PyObject_New(PySListIterObject,
                                         &PySListIterType);
    if (!it) return NULL;

    Py_INCREF(self);
    it->owner     = self;
    it->cur       = self->list->head;
    it->snap_size = self->list->size;
    return (PyObject *)it;
}

static PyObject *PySList_repr(PySListObject *self)
{
    if (self->list->size == 0)
        return PyUnicode_FromString("SList([])");

    PyObject *parts = PyList_New(0);
    if (!parts) return NULL;

    SListNode *cur = self->list->head;
    while (cur) {
        PyObject *r = PyObject_Repr(cur->value);
        if (!r) { Py_DECREF(parts); return NULL; }
        if (PyList_Append(parts, r) < 0) {
            Py_DECREF(r); Py_DECREF(parts); return NULL;
        }
        Py_DECREF(r);
        cur = cur->next;
    }

    PyObject *sep = PyUnicode_FromString(", ");
    PyObject *joined = PyUnicode_Join(sep, parts);
    Py_DECREF(sep);
    Py_DECREF(parts);
    if (!joined) return NULL;

    PyObject *result = PyUnicode_FromFormat("SList([%U])", joined);
    Py_DECREF(joined);
    return result;
}

static PyObject *PySList_richcmp(PyObject *self, PyObject *other, int op)
{
    if (op != Py_EQ && op != Py_NE)
        Py_RETURN_NOTIMPLEMENTED;

    if (!PyObject_TypeCheck(other, Py_TYPE(self)))
        Py_RETURN_NOTIMPLEMENTED;

    PySListObject *a = (PySListObject *)self;
    PySListObject *b = (PySListObject *)other;

    if (a->list->size != b->list->size) {
        if (op == Py_EQ) Py_RETURN_FALSE;
        Py_RETURN_TRUE;
    }

    SListNode *na = a->list->head;
    SListNode *nb = b->list->head;
    while (na) {
        int eq = PyObject_RichCompareBool(na->value, nb->value, Py_EQ);
        if (eq < 0) return NULL;
        if (!eq) {
            if (op == Py_EQ) Py_RETURN_FALSE;
            Py_RETURN_TRUE;
        }
        na = na->next;
        nb = nb->next;
    }

    if (op == Py_EQ) Py_RETURN_TRUE;
    Py_RETURN_FALSE;
}

PyTypeObject PySListType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name        = "juniper._native.SList",
    .tp_basicsize   = sizeof(PySListObject),
    .tp_flags       = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    .tp_new         = PySList_new,
    .tp_dealloc     = (destructor)PySList_dealloc,
    .tp_methods     = PySList_methods,
    .tp_as_sequence = &PySList_as_sequence,
    .tp_iter        = (getiterfunc)PySList_iter,
    .tp_repr        = (reprfunc)PySList_repr,
    .tp_richcompare = PySList_richcmp,
    .tp_doc         = "Singly linked list implemented in C.",
};
