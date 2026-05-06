#include <Python.h>
#include "bst.h"

typedef struct
{
    PyObject_HEAD BST *tree;
} PyBSTSetObject;

typedef struct
{
    PyObject_HEAD PyBSTSetObject *set;
    BSTNode *next;
    size_t snap_size;
} PyBSTSetIterObject;

static PyTypeObject PyBSTSetIterType;

static void PyBSTSetIter_dealloc(PyBSTSetIterObject *it)
{
    Py_XDECREF(it->set);
    Py_TYPE(it)->tp_free((PyObject *)it);
}

static PyObject *PyBSTSetIter_next(PyBSTSetIterObject *it)
{
    if (!it->set || !it->set->tree)
        return NULL;
    BST *tree = it->set->tree;

    if (tree->size != it->snap_size)
    {
        PyErr_SetString(PyExc_RuntimeError,
                        "BSTSet changed size during iteration");
        return NULL;
    }
    if (!it->next)
        return NULL;

    PyObject *key = it->next->key;
    Py_INCREF(key);
    it->next = bst_successor(tree, it->next);
    return key;
}

static PyObject *PyBSTSetIter_iter(PyObject *self)
{
    Py_INCREF(self);
    return self;
}

static PyTypeObject PyBSTSetIterType = {
    PyVarObject_HEAD_INIT(NULL, 0)
        .tp_name = "juniper._native.BSTSetIterator",
    .tp_basicsize = sizeof(PyBSTSetIterObject),
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_dealloc = (destructor)PyBSTSetIter_dealloc,
    .tp_iter = PyBSTSetIter_iter,
    .tp_iternext = (iternextfunc)PyBSTSetIter_next,
};

static PyObject *PyBSTSet_new(PyTypeObject *type,
                              PyObject *args, PyObject *kwds)
{
    PyBSTSetObject *self;
    PyObject *iterable = NULL;
    if (!PyArg_ParseTuple(args, "|O", &iterable))
        return NULL;

    self = (PyBSTSetObject *)type->tp_alloc(type, 0);
    if (!self)
        return NULL;

    self->tree = bst_create();
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
            int rc = bst_insert(self->tree, item);
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

static void PyBSTSet_dealloc(PyBSTSetObject *self)
{
    if (self->tree)
        bst_free(self->tree);
    Py_TYPE(self)->tp_free((PyObject *)self);
}

static Py_ssize_t PyBSTSet_len(PyBSTSetObject *self)
{
    return (Py_ssize_t)self->tree->size;
}

static int PyBSTSet_contains(PyBSTSetObject *self, PyObject *key)
{
    return bst_contains(self->tree, key);
}

static PySequenceMethods PyBSTSet_as_sequence = {
    .sq_length = (lenfunc)PyBSTSet_len,
    .sq_contains = (objobjproc)PyBSTSet_contains,
};

static PyObject *PyBSTSet_add(PyBSTSetObject *self, PyObject *args)
{
    PyObject *key;
    if (!PyArg_ParseTuple(args, "O", &key))
        return NULL;
    int rc = bst_insert(self->tree, key);
    if (rc < 0)
        return NULL;
    Py_RETURN_NONE;
}

static PyObject *PyBSTSet_discard(PyBSTSetObject *self, PyObject *args)
{
    PyObject *key;
    if (!PyArg_ParseTuple(args, "O", &key))
        return NULL;
    int rc = bst_remove(self->tree, key);
    if (rc < 0)
        return NULL;
    Py_RETURN_NONE;
}

static PyObject *PyBSTSet_remove(PyBSTSetObject *self, PyObject *args)
{
    PyObject *key;
    if (!PyArg_ParseTuple(args, "O", &key))
        return NULL;
    int rc = bst_remove(self->tree, key);
    if (rc < 0)
        return NULL;
    if (rc == 0)
    {
        PyErr_SetObject(PyExc_KeyError, key);
        return NULL;
    }
    Py_RETURN_NONE;
}

static PyObject *PyBSTSet_contains_method(PyBSTSetObject *self, PyObject *args)
{
    PyObject *key;
    if (!PyArg_ParseTuple(args, "O", &key))
        return NULL;
    int rc = bst_contains(self->tree, key);
    if (rc < 0)
        return NULL;
    return PyBool_FromLong(rc);
}

static PyObject *PyBSTSet_clear(PyBSTSetObject *self,
                                PyObject *Py_UNUSED(ignored))
{
    if (self->tree)
    {
        bst_free(self->tree);
        self->tree = bst_create();
        if (!self->tree)
            return PyErr_NoMemory();
    }
    Py_RETURN_NONE;
}

static PyObject *PyBSTSet_pop(PyBSTSetObject *self,
                              PyObject *Py_UNUSED(ignored))
{
    if (self->tree->size == 0)
    {
        PyErr_SetString(PyExc_KeyError, "pop from an empty set");
        return NULL;
    }
    BSTNode *min = bst_minimum(self->tree, self->tree->root);
    PyObject *key = min->key;
    Py_INCREF(key);
    int rc = bst_remove(self->tree, key);
    if (rc < 0)
    {
        Py_DECREF(key);
        return NULL;
    }
    return key;
}

static PyObject *PyBSTSet_height(PyBSTSetObject *self,
                                 PyObject *Py_UNUSED(ignored))
{
    return PyLong_FromLong(bst_height(self->tree));
}

static PyMethodDef PyBSTSet_methods[] = {
    {"add", (PyCFunction)PyBSTSet_add, METH_VARARGS,
     "Add an element to the set."},
    {"discard", (PyCFunction)PyBSTSet_discard, METH_VARARGS,
     "Remove an element if present (no error if missing)."},
    {"remove", (PyCFunction)PyBSTSet_remove, METH_VARARGS,
     "Remove an element; raise KeyError if missing."},
    {"contains", (PyCFunction)PyBSTSet_contains_method, METH_VARARGS,
     "Return True if element is in the set."},
    {"clear", (PyCFunction)PyBSTSet_clear, METH_NOARGS,
     "Remove all elements."},
    {"pop", (PyCFunction)PyBSTSet_pop, METH_NOARGS,
     "Remove and return the smallest element."},
    {"height", (PyCFunction)PyBSTSet_height, METH_NOARGS,
     "Return the height of the tree."},
    {NULL}};

static PyObject *PyBSTSet_iter(PyBSTSetObject *self)
{
    if (PyType_Ready(&PyBSTSetIterType) < 0)
        return NULL;
    PyBSTSetIterObject *it = PyObject_New(PyBSTSetIterObject, &PyBSTSetIterType);
    if (!it)
        return NULL;
    Py_INCREF(self);
    it->set = self;
    it->snap_size = self->tree->size;
    it->next = self->tree->size > 0
                   ? bst_minimum(self->tree, self->tree->root)
                   : NULL;
    return (PyObject *)it;
}

static PyObject *PyBSTSet_repr(PyBSTSetObject *self)
{
    if (self->tree->size == 0)
        return PyUnicode_FromString("BSTSet()");

    PyObject *parts = PyList_New(0);
    if (!parts)
        return NULL;

    BSTNode *node = bst_minimum(self->tree, self->tree->root);
    while (node)
    {
        PyObject *r = PyObject_Repr(node->key);
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
        node = bst_successor(self->tree, node);
    }

    PyObject *sep = PyUnicode_FromString(", ");
    PyObject *joined = PyUnicode_Join(sep, parts);
    Py_DECREF(sep);
    Py_DECREF(parts);
    if (!joined)
        return NULL;

    PyObject *result = PyUnicode_FromFormat("BSTSet({%U})", joined);
    Py_DECREF(joined);
    return result;
}

static PyObject *PyBSTSet_richcmp(PyObject *self, PyObject *other, int op)
{
    if (op != Py_EQ && op != Py_NE)
        Py_RETURN_NOTIMPLEMENTED;
    if (!PyObject_TypeCheck(other, Py_TYPE(self)))
        Py_RETURN_NOTIMPLEMENTED;

    PyBSTSetObject *a = (PyBSTSetObject *)self;
    PyBSTSetObject *b = (PyBSTSetObject *)other;

    if (a->tree->size != b->tree->size)
    {
        if (op == Py_EQ)
            Py_RETURN_FALSE;
        Py_RETURN_TRUE;
    }

    BSTNode *na = bst_minimum(a->tree, a->tree->root);
    BSTNode *nb = bst_minimum(b->tree, b->tree->root);
    while (na)
    {
        int eq = PyObject_RichCompareBool(na->key, nb->key, Py_EQ);
        if (eq < 0)
            return NULL;
        if (!eq)
        {
            if (op == Py_EQ)
                Py_RETURN_FALSE;
            Py_RETURN_TRUE;
        }
        na = bst_successor(a->tree, na);
        nb = bst_successor(b->tree, nb);
    }
    if (op == Py_EQ)
        Py_RETURN_TRUE;
    Py_RETURN_FALSE;
}

PyTypeObject PyBSTSetType = {
    PyVarObject_HEAD_INIT(NULL, 0)
        .tp_name = "juniper._native.BSTSet",
    .tp_basicsize = sizeof(PyBSTSetObject),
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    .tp_new = PyBSTSet_new,
    .tp_dealloc = (destructor)PyBSTSet_dealloc,
    .tp_methods = PyBSTSet_methods,
    .tp_as_sequence = &PyBSTSet_as_sequence,
    .tp_iter = (getiterfunc)PyBSTSet_iter,
    .tp_repr = (reprfunc)PyBSTSet_repr,
    .tp_richcompare = PyBSTSet_richcmp,
    .tp_doc = "Sorted set based on an unbalanced binary search tree.",
};