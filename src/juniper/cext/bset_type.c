#include <Python.h>
#include "btree.h"

typedef struct
{
    PyObject_HEAD BTree *tree;
} PyBTreeSetObject;

typedef struct
{
    PyObject_HEAD PyBTreeSetObject *set;
    BTreePos pos;
    size_t snap_size;
} PyBTreeSetIterObject;

static PyTypeObject PyBTreeSetIterType;

static void PyBTreeSetIter_dealloc(PyBTreeSetIterObject *it)
{
    Py_XDECREF(it->set);
    Py_TYPE(it)->tp_free((PyObject *)it);
}

static PyObject *PyBTreeSetIter_next(PyBTreeSetIterObject *it)
{
    if (!it->set || !it->set->tree)
        return NULL;

    BTree *tree = it->set->tree;

    if (tree->size != it->snap_size)
    {
        PyErr_SetString(PyExc_RuntimeError,
                        "BTreeSet changed size during iteration");
        return NULL;
    }

    if (!it->pos.node)
        return NULL;

    PyObject *key = it->pos.node->keys[it->pos.idx];
    Py_INCREF(key);

    it->pos = btree_next(tree, it->pos);
    return key;
}

static PyObject *PyBTreeSetIter_iter(PyObject *self)
{
    Py_INCREF(self);
    return self;
}

static PyTypeObject PyBTreeSetIterType = {
    PyVarObject_HEAD_INIT(NULL, 0)
        .tp_name = "juniper._native.BTreeSetIterator",
    .tp_basicsize = sizeof(PyBTreeSetIterObject),
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_dealloc = (destructor)PyBTreeSetIter_dealloc,
    .tp_iter = PyBTreeSetIter_iter,
    .tp_iternext = (iternextfunc)PyBTreeSetIter_next,
};

static PyObject *PyBTreeSet_new(PyTypeObject *type,
                                PyObject *args, PyObject *kwds)
{
    static char *kwlist[] = {"iterable", "order", NULL};
    PyBTreeSetObject *self;
    PyObject *iterable = NULL;
    int order = BTREE_ORDER;

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "|Oi", kwlist,
                                     &iterable, &order))
        return NULL;

    if (order < 3)
    {
        PyErr_SetString(PyExc_ValueError,
                        "B-tree order must be at least 3");
        return NULL;
    }

    self = (PyBTreeSetObject *)type->tp_alloc(type, 0);
    if (!self)
        return NULL;

    self->tree = btree_create(order);
    if (!self->tree)
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
            int rc = btree_insert(self->tree, item);
            Py_DECREF(item);
            if (rc < 0)
            {
                Py_DECREF(iter);
                Py_DECREF(self);
                return NULL;
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

static void PyBTreeSet_dealloc(PyBTreeSetObject *self)
{
    if (self->tree)
        btree_free(self->tree);
    Py_TYPE(self)->tp_free((PyObject *)self);
}

static Py_ssize_t PyBTreeSet_len(PyBTreeSetObject *self)
{
    return (Py_ssize_t)self->tree->size;
}

static int PyBTreeSet_contains(PyBTreeSetObject *self, PyObject *key)
{
    return btree_contains(self->tree, key);
}

static PySequenceMethods PyBTreeSet_as_sequence = {
    .sq_length = (lenfunc)PyBTreeSet_len,
    .sq_contains = (objobjproc)PyBTreeSet_contains,
};

static PyObject *PyBTreeSet_add(PyBTreeSetObject *self, PyObject *args)
{
    PyObject *key;
    if (!PyArg_ParseTuple(args, "O", &key))
        return NULL;
    int rc = btree_insert(self->tree, key);
    if (rc < 0)
        return NULL;
    Py_RETURN_NONE;
}

static PyObject *PyBTreeSet_discard(PyBTreeSetObject *self, PyObject *args)
{
    PyObject *key;
    if (!PyArg_ParseTuple(args, "O", &key))
        return NULL;
    int rc = btree_remove(self->tree, key);
    if (rc < 0)
        return NULL;
    Py_RETURN_NONE;
}

static PyObject *PyBTreeSet_remove(PyBTreeSetObject *self, PyObject *args)
{
    PyObject *key;
    if (!PyArg_ParseTuple(args, "O", &key))
        return NULL;
    int rc = btree_remove(self->tree, key);
    if (rc < 0)
        return NULL;
    if (rc == 0)
    {
        PyErr_SetObject(PyExc_KeyError, key);
        return NULL;
    }
    Py_RETURN_NONE;
}

static PyObject *PyBTreeSet_contains_method(PyBTreeSetObject *self,
                                            PyObject *args)
{
    PyObject *key;
    if (!PyArg_ParseTuple(args, "O", &key))
        return NULL;
    int rc = btree_contains(self->tree, key);
    if (rc < 0)
        return NULL;
    return PyBool_FromLong(rc);
}

static PyObject *PyBTreeSet_clear(PyBTreeSetObject *self,
                                  PyObject *Py_UNUSED(ignored))
{
    if (self->tree)
    {
        int order = self->tree->order;
        btree_free(self->tree);
        self->tree = btree_create(order);
        if (!self->tree)
            return PyErr_NoMemory();
    }
    Py_RETURN_NONE;
}

static PyObject *PyBTreeSet_pop(PyBTreeSetObject *self,
                                PyObject *Py_UNUSED(ignored))
{
    if (self->tree->size == 0)
    {
        PyErr_SetString(PyExc_KeyError, "pop from an empty set");
        return NULL;
    }
    BTreePos first = btree_first(self->tree);
    PyObject *key = first.node->keys[first.idx];
    Py_INCREF(key);

    int rc = btree_remove(self->tree, key);
    if (rc < 0)
    {
        Py_DECREF(key);
        return NULL;
    }
    return key;
}

static PyObject *PyBTreeSet_peek_min(PyBTreeSetObject *self,
                                     PyObject *Py_UNUSED(ignored))
{
    if (self->tree->size == 0)
    {
        PyErr_SetString(PyExc_KeyError, "peek_min on empty BTreeSet");
        return NULL;
    }
    BTreePos first = btree_first(self->tree);
    Py_INCREF(first.node->keys[first.idx]);
    return first.node->keys[first.idx];
}

static PyObject *PyBTreeSet_height(PyBTreeSetObject *self,
                                   PyObject *Py_UNUSED(ignored))
{
    return PyLong_FromLong(btree_height(self->tree));
}

static PyObject *PyBTreeSet_order(PyBTreeSetObject *self,
                                  PyObject *Py_UNUSED(ignored))
{
    return PyLong_FromLong(self->tree->order);
}

static PyMethodDef PyBTreeSet_methods[] = {
    {"add", (PyCFunction)PyBTreeSet_add, METH_VARARGS,
     "Add an element to the set."},
    {"discard", (PyCFunction)PyBTreeSet_discard, METH_VARARGS,
     "Remove an element if present (no error if missing)."},
    {"remove", (PyCFunction)PyBTreeSet_remove, METH_VARARGS,
     "Remove an element; raise KeyError if missing."},
    {"contains", (PyCFunction)PyBTreeSet_contains_method, METH_VARARGS,
     "Return True if element is in the set."},
    {"clear", (PyCFunction)PyBTreeSet_clear, METH_NOARGS,
     "Remove all elements."},
    {"pop", (PyCFunction)PyBTreeSet_pop, METH_NOARGS,
     "Remove and return the smallest element."},
    {"peek_min", (PyCFunction)PyBTreeSet_peek_min, METH_NOARGS,
     "Return the smallest element without removing it."},
    {"height", (PyCFunction)PyBTreeSet_height, METH_NOARGS,
     "Return the height of the B-tree."},
    {"order", (PyCFunction)PyBTreeSet_order, METH_NOARGS,
     "Return the order (branching factor) of the B-tree."},
    {NULL}};

static PyObject *PyBTreeSet_iter(PyBTreeSetObject *self)
{
    if (PyType_Ready(&PyBTreeSetIterType) < 0)
        return NULL;

    PyBTreeSetIterObject *it = PyObject_New(PyBTreeSetIterObject,
                                            &PyBTreeSetIterType);
    if (!it)
        return NULL;

    Py_INCREF(self);
    it->set = self;
    it->snap_size = self->tree->size;
    it->pos = btree_first(self->tree);
    return (PyObject *)it;
}

static PyObject *PyBTreeSet_repr(PyBTreeSetObject *self)
{
    if (self->tree->size == 0)
        return PyUnicode_FromString("BTreeSet()");

    PyObject *parts = PyList_New(0);
    if (!parts)
        return NULL;

    BTreePos pos = btree_first(self->tree);
    while (pos.node)
    {
        PyObject *r = PyObject_Repr(pos.node->keys[pos.idx]);
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
        pos = btree_next(self->tree, pos);
    }

    PyObject *sep = PyUnicode_FromString(", ");
    PyObject *joined = PyUnicode_Join(sep, parts);
    Py_DECREF(sep);
    Py_DECREF(parts);
    if (!joined)
        return NULL;

    PyObject *result = PyUnicode_FromFormat("BTreeSet({%U})", joined);
    Py_DECREF(joined);
    return result;
}

static PyObject *PyBTreeSet_richcmp(PyObject *self, PyObject *other, int op)
{
    if (op != Py_EQ && op != Py_NE)
        Py_RETURN_NOTIMPLEMENTED;

    if (!PyObject_TypeCheck(other, Py_TYPE(self)))
        Py_RETURN_NOTIMPLEMENTED;

    PyBTreeSetObject *a = (PyBTreeSetObject *)self;
    PyBTreeSetObject *b = (PyBTreeSetObject *)other;

    if (a->tree->size != b->tree->size)
    {
        if (op == Py_EQ)
            Py_RETURN_FALSE;
        Py_RETURN_TRUE;
    }

    BTreePos pa = btree_first(a->tree);
    BTreePos pb = btree_first(b->tree);

    while (pa.node)
    {
        int eq = PyObject_RichCompareBool(pa.node->keys[pa.idx],
                                          pb.node->keys[pb.idx], Py_EQ);
        if (eq < 0)
            return NULL;
        if (!eq)
        {
            if (op == Py_EQ)
                Py_RETURN_FALSE;
            Py_RETURN_TRUE;
        }
        pa = btree_next(a->tree, pa);
        pb = btree_next(b->tree, pb);
    }

    if (op == Py_EQ)
        Py_RETURN_TRUE;
    Py_RETURN_FALSE;
}

PyTypeObject PyBTreeSetType = {
    PyVarObject_HEAD_INIT(NULL, 0)
        .tp_name = "juniper._native.BTreeSet",
    .tp_basicsize = sizeof(PyBTreeSetObject),
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    .tp_new = PyBTreeSet_new,
    .tp_dealloc = (destructor)PyBTreeSet_dealloc,
    .tp_methods = PyBTreeSet_methods,
    .tp_as_sequence = &PyBTreeSet_as_sequence,
    .tp_iter = (getiterfunc)PyBTreeSet_iter,
    .tp_repr = (reprfunc)PyBTreeSet_repr,
    .tp_richcompare = PyBTreeSet_richcmp,
    .tp_doc = "Sorted set based on a B-tree.",
};
