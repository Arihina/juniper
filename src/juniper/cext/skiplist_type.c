#include <Python.h>
#include "skiplist.h"

typedef struct {
    PyObject_HEAD
    SkipList *list;
} PySkipListSetObject;

typedef struct {
    PyObject_HEAD
    PySkipListSetObject *set;
    SkipNode            *next;
    size_t               snap_size;
} PySkipListSetIterObject;

static PyTypeObject PySkipListSetIterType;

static void PySkipListSetIter_dealloc(PySkipListSetIterObject *it)
{
    Py_XDECREF(it->set);
    Py_TYPE(it)->tp_free((PyObject *)it);
}

static PyObject *PySkipListSetIter_next(PySkipListSetIterObject *it)
{
    if (!it->set || !it->set->list) return NULL;

    if (it->set->list->size != it->snap_size) {
        PyErr_SetString(PyExc_RuntimeError,
                        "SkipListSet changed size during iteration");
        return NULL;
    }
    if (!it->next) return NULL;

    PyObject *key = it->next->key;
    Py_INCREF(key);
    it->next = skiplist_next(it->next);
    return key;
}

static PyObject *PySkipListSetIter_iter(PyObject *self)
{ Py_INCREF(self); return self; }

static PyTypeObject PySkipListSetIterType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name      = "juniper._native.SkipListSetIterator",
    .tp_basicsize = sizeof(PySkipListSetIterObject),
    .tp_flags     = Py_TPFLAGS_DEFAULT,
    .tp_dealloc   = (destructor)PySkipListSetIter_dealloc,
    .tp_iter      = PySkipListSetIter_iter,
    .tp_iternext  = (iternextfunc)PySkipListSetIter_next,
};

static PyObject *PySkipListSet_new(PyTypeObject *type,
                                   PyObject *args, PyObject *kwds)
{
    PySkipListSetObject *self;
    PyObject *iterable = NULL;
    if (!PyArg_ParseTuple(args, "|O", &iterable)) return NULL;

    self = (PySkipListSetObject *)type->tp_alloc(type, 0);
    if (!self) return NULL;

    self->list = skiplist_create();
    if (!self->list) { Py_DECREF(self); return PyErr_NoMemory(); }

    if (iterable) {
        PyObject *iter = PyObject_GetIter(iterable);
        if (!iter) { Py_DECREF(self); return NULL; }
        PyObject *item;
        while ((item = PyIter_Next(iter))) {
            int rc = skiplist_insert(self->list, item);
            Py_DECREF(item);
            if (rc < 0) { Py_DECREF(iter); Py_DECREF(self); return NULL; }
        }
        Py_DECREF(iter);
        if (PyErr_Occurred()) { Py_DECREF(self); return NULL; }
    }
    return (PyObject *)self;
}

static void PySkipListSet_dealloc(PySkipListSetObject *self)
{
    if (self->list) skiplist_free(self->list);
    Py_TYPE(self)->tp_free((PyObject *)self);
}

static Py_ssize_t PySkipListSet_len(PySkipListSetObject *self)
{ return (Py_ssize_t)self->list->size; }

static int PySkipListSet_contains(PySkipListSetObject *self, PyObject *key)
{ return skiplist_contains(self->list, key); }

static PySequenceMethods PySkipListSet_as_sequence = {
    .sq_length   = (lenfunc)PySkipListSet_len,
    .sq_contains = (objobjproc)PySkipListSet_contains,
};

static PyObject *PySkipListSet_add(PySkipListSetObject *self, PyObject *args)
{
    PyObject *key;
    if (!PyArg_ParseTuple(args, "O", &key)) return NULL;
    int rc = skiplist_insert(self->list, key);
    if (rc < 0) return NULL;
    Py_RETURN_NONE;
}

static PyObject *PySkipListSet_discard(PySkipListSetObject *self, PyObject *args)
{
    PyObject *key;
    if (!PyArg_ParseTuple(args, "O", &key)) return NULL;
    int rc = skiplist_remove(self->list, key);
    if (rc < 0) return NULL;
    Py_RETURN_NONE;
}

static PyObject *PySkipListSet_remove(PySkipListSetObject *self, PyObject *args)
{
    PyObject *key;
    if (!PyArg_ParseTuple(args, "O", &key)) return NULL;
    int rc = skiplist_remove(self->list, key);
    if (rc < 0) return NULL;
    if (rc == 0) { PyErr_SetObject(PyExc_KeyError, key); return NULL; }
    Py_RETURN_NONE;
}

static PyObject *PySkipListSet_contains_method(PySkipListSetObject *self,
                                               PyObject *args)
{
    PyObject *key;
    if (!PyArg_ParseTuple(args, "O", &key)) return NULL;
    int rc = skiplist_contains(self->list, key);
    if (rc < 0) return NULL;
    return PyBool_FromLong(rc);
}

static PyObject *PySkipListSet_clear(PySkipListSetObject *self,
                                     PyObject *Py_UNUSED(ignored))
{
    if (self->list) {
        skiplist_free(self->list);
        self->list = skiplist_create();
        if (!self->list) return PyErr_NoMemory();
    }
    Py_RETURN_NONE;
}

static PyObject *PySkipListSet_pop(PySkipListSetObject *self,
                                   PyObject *Py_UNUSED(ignored))
{
    if (self->list->size == 0) {
        PyErr_SetString(PyExc_KeyError, "pop from an empty set");
        return NULL;
    }
    SkipNode *first = skiplist_first(self->list);
    PyObject *key = first->key;
    Py_INCREF(key);
    int rc = skiplist_remove(self->list, key);
    if (rc < 0) { Py_DECREF(key); return NULL; }
    return key;
}

static PyObject *PySkipListSet_height(PySkipListSetObject *self,
                                      PyObject *Py_UNUSED(ignored))
{
    return PyLong_FromLong(skiplist_height(self->list));
}

static PyMethodDef PySkipListSet_methods[] = {
    {"add",      (PyCFunction)PySkipListSet_add,      METH_VARARGS,
     "Add an element to the set."},
    {"discard",  (PyCFunction)PySkipListSet_discard,   METH_VARARGS,
     "Remove an element if present (no error if missing)."},
    {"remove",   (PyCFunction)PySkipListSet_remove,    METH_VARARGS,
     "Remove an element; raise KeyError if missing."},
    {"contains", (PyCFunction)PySkipListSet_contains_method, METH_VARARGS,
     "Return True if element is in the set."},
    {"clear",    (PyCFunction)PySkipListSet_clear,     METH_NOARGS,
     "Remove all elements."},
    {"pop",      (PyCFunction)PySkipListSet_pop,       METH_NOARGS,
     "Remove and return the smallest element."},
    {"height",   (PyCFunction)PySkipListSet_height,    METH_NOARGS,
     "Return the current max level of the skip list."},
    {NULL}
};

static PyObject *PySkipListSet_iter(PySkipListSetObject *self)
{
    if (PyType_Ready(&PySkipListSetIterType) < 0) return NULL;
    PySkipListSetIterObject *it = PyObject_New(PySkipListSetIterObject,
                                               &PySkipListSetIterType);
    if (!it) return NULL;
    Py_INCREF(self);
    it->set       = self;
    it->snap_size = self->list->size;
    it->next      = skiplist_first(self->list);
    return (PyObject *)it;
}

static PyObject *PySkipListSet_repr(PySkipListSetObject *self)
{
    if (self->list->size == 0)
        return PyUnicode_FromString("SkipListSet()");

    PyObject *parts = PyList_New(0);
    if (!parts) return NULL;

    SkipNode *node = skiplist_first(self->list);
    while (node) {
        PyObject *r = PyObject_Repr(node->key);
        if (!r) { Py_DECREF(parts); return NULL; }
        if (PyList_Append(parts, r) < 0) {
            Py_DECREF(r); Py_DECREF(parts); return NULL;
        }
        Py_DECREF(r);
        node = skiplist_next(node);
    }

    PyObject *sep = PyUnicode_FromString(", ");
    PyObject *joined = PyUnicode_Join(sep, parts);
    Py_DECREF(sep); Py_DECREF(parts);
    if (!joined) return NULL;

    PyObject *result = PyUnicode_FromFormat("SkipListSet({%U})", joined);
    Py_DECREF(joined);
    return result;
}

static PyObject *PySkipListSet_richcmp(PyObject *self, PyObject *other, int op)
{
    if (op != Py_EQ && op != Py_NE) Py_RETURN_NOTIMPLEMENTED;
    if (!PyObject_TypeCheck(other, Py_TYPE(self))) Py_RETURN_NOTIMPLEMENTED;

    PySkipListSetObject *a = (PySkipListSetObject *)self;
    PySkipListSetObject *b = (PySkipListSetObject *)other;

    if (a->list->size != b->list->size) {
        if (op == Py_EQ) Py_RETURN_FALSE;
        Py_RETURN_TRUE;
    }

    SkipNode *na = skiplist_first(a->list);
    SkipNode *nb = skiplist_first(b->list);
    while (na) {
        int eq = PyObject_RichCompareBool(na->key, nb->key, Py_EQ);
        if (eq < 0) return NULL;
        if (!eq) {
            if (op == Py_EQ) Py_RETURN_FALSE;
            Py_RETURN_TRUE;
        }
        na = skiplist_next(na);
        nb = skiplist_next(nb);
    }
    if (op == Py_EQ) Py_RETURN_TRUE;
    Py_RETURN_FALSE;
}

PyTypeObject PySkipListSetType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name        = "juniper._native.SkipListSet",
    .tp_basicsize   = sizeof(PySkipListSetObject),
    .tp_flags       = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    .tp_new         = PySkipListSet_new,
    .tp_dealloc     = (destructor)PySkipListSet_dealloc,
    .tp_methods     = PySkipListSet_methods,
    .tp_as_sequence = &PySkipListSet_as_sequence,
    .tp_iter        = (getiterfunc)PySkipListSet_iter,
    .tp_repr        = (reprfunc)PySkipListSet_repr,
    .tp_richcompare = PySkipListSet_richcmp,
    .tp_doc         = "Sorted set based on a skip list.",
};