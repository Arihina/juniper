#include <Python.h>
#include "avltree.h"

typedef struct
{
    PyObject_HEAD AVLTree *tree;
} PyAVLSetObject;

typedef struct
{
    PyObject_HEAD PyAVLSetObject *set;
    AVLNode *next;
    size_t snap_size;
} PyAVLSetIterObject;

static PyTypeObject PyAVLSetIterType;

static void PyAVLSetIter_dealloc(PyAVLSetIterObject *it)
{
    Py_XDECREF(it->set);
    Py_TYPE(it)->tp_free((PyObject *)it);
}

static PyObject *PyAVLSetIter_next(PyAVLSetIterObject *it)
{
    if (!it->set || !it->set->tree)
        return NULL;

    AVLTree *tree = it->set->tree;

    if (tree->size != it->snap_size)
    {
        PyErr_SetString(PyExc_RuntimeError,
                        "AVLSet changed size during iteration");
        return NULL;
    }

    if (it->next == tree->nil)
        return NULL;

    PyObject *key = it->next->key;
    Py_INCREF(key);

    it->next = avltree_successor(tree, it->next);
    return key;
}

static PyObject *PyAVLSetIter_iter(PyObject *self)
{
    Py_INCREF(self);
    return self;
}

static PyTypeObject PyAVLSetIterType = {
    PyVarObject_HEAD_INIT(NULL, 0)
        .tp_name = "juniper._native.AVLSetIterator",
    .tp_basicsize = sizeof(PyAVLSetIterObject),
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_dealloc = (destructor)PyAVLSetIter_dealloc,
    .tp_iter = PyAVLSetIter_iter,
    .tp_iternext = (iternextfunc)PyAVLSetIter_next,
};

static PyObject *PyAVLSet_new(PyTypeObject *type,
                              PyObject *args, PyObject *kwds)
{
    PyAVLSetObject *self;
    PyObject *iterable = NULL;

    if (!PyArg_ParseTuple(args, "|O", &iterable))
        return NULL;

    self = (PyAVLSetObject *)type->tp_alloc(type, 0);
    if (!self)
        return NULL;

    self->tree = avltree_create();
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
            int rc = avltree_insert(self->tree, item);
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

static void PyAVLSet_dealloc(PyAVLSetObject *self)
{
    if (self->tree)
        avltree_free(self->tree);
    Py_TYPE(self)->tp_free((PyObject *)self);
}

static Py_ssize_t PyAVLSet_len(PyAVLSetObject *self)
{
    return (Py_ssize_t)self->tree->size;
}

static int PyAVLSet_contains(PyAVLSetObject *self, PyObject *key)
{
    return avltree_contains(self->tree, key);
}

static PyObject *PyAVLSet_add(PyAVLSetObject *self, PyObject *args)
{
    PyObject *key;
    if (!PyArg_ParseTuple(args, "O", &key))
        return NULL;
    int rc = avltree_insert(self->tree, key);
    if (rc < 0)
        return NULL;
    Py_RETURN_NONE;
}

static PyObject *PyAVLSet_discard(PyAVLSetObject *self, PyObject *args)
{
    PyObject *key;
    if (!PyArg_ParseTuple(args, "O", &key))
        return NULL;
    int rc = avltree_remove(self->tree, key);
    if (rc < 0)
        return NULL;
    Py_RETURN_NONE;
}

static PyObject *PyAVLSet_remove(PyAVLSetObject *self, PyObject *args)
{
    PyObject *key;
    if (!PyArg_ParseTuple(args, "O", &key))
        return NULL;
    int rc = avltree_remove(self->tree, key);
    if (rc < 0)
        return NULL;
    if (rc == 0)
    {
        PyErr_SetObject(PyExc_KeyError, key);
        return NULL;
    }
    Py_RETURN_NONE;
}

static PyObject *PyAVLSet_contains_method(PyAVLSetObject *self, PyObject *args)
{
    PyObject *key;
    if (!PyArg_ParseTuple(args, "O", &key))
        return NULL;
    int rc = avltree_contains(self->tree, key);
    if (rc < 0)
        return NULL;
    return PyBool_FromLong(rc);
}

static PyObject *PyAVLSet_clear(PyAVLSetObject *self,
                                PyObject *Py_UNUSED(ignored))
{
    if (self->tree)
    {
        avltree_free(self->tree);
        self->tree = avltree_create();
        if (!self->tree)
            return PyErr_NoMemory();
    }
    Py_RETURN_NONE;
}

static PyObject *PyAVLSet_pop(PyAVLSetObject *self,
                              PyObject *Py_UNUSED(ignored))
{
    if (self->tree->size == 0)
    {
        PyErr_SetString(PyExc_KeyError, "pop from an empty set");
        return NULL;
    }
    AVLNode *min = avltree_minimum(self->tree, self->tree->root);
    PyObject *key = min->key;
    Py_INCREF(key);

    int rc = avltree_remove(self->tree, key);
    if (rc < 0)
    {
        Py_DECREF(key);
        return NULL;
    }
    return key;
}

static PyObject *PyAVLSet_height(PyAVLSetObject *self,
                                 PyObject *Py_UNUSED(ignored))
{
    if (self->tree->root == self->tree->nil)
        return PyLong_FromLong(0);
    return PyLong_FromLong(self->tree->root->height);
}

static PyMethodDef PyAVLSet_methods[] = {
    {"add", (PyCFunction)PyAVLSet_add, METH_VARARGS,
     "Add an element to the set."},
    {"discard", (PyCFunction)PyAVLSet_discard, METH_VARARGS,
     "Remove an element if present (no error if missing)."},
    {"remove", (PyCFunction)PyAVLSet_remove, METH_VARARGS,
     "Remove an element; raise KeyError if missing."},
    {"contains", (PyCFunction)PyAVLSet_contains_method, METH_VARARGS,
     "Return True if element is in the set."},
    {"clear", (PyCFunction)PyAVLSet_clear, METH_NOARGS,
     "Remove all elements."},
    {"pop", (PyCFunction)PyAVLSet_pop, METH_NOARGS,
     "Remove and return the smallest element."},
    {"height", (PyCFunction)PyAVLSet_height, METH_NOARGS,
     "Return the height of the AVL tree."},
    {NULL}};

static PyObject *PyAVLSet_iter(PyAVLSetObject *self)
{
    if (PyType_Ready(&PyAVLSetIterType) < 0)
        return NULL;

    PyAVLSetIterObject *it = PyObject_New(PyAVLSetIterObject,
                                          &PyAVLSetIterType);
    if (!it)
        return NULL;

    Py_INCREF(self);
    it->set = self;
    it->snap_size = self->tree->size;

    if (self->tree->size > 0)
        it->next = avltree_minimum(self->tree, self->tree->root);
    else
        it->next = self->tree->nil;

    return (PyObject *)it;
}

static PyObject *PyAVLSet_repr(PyAVLSetObject *self)
{
    if (self->tree->size == 0)
        return PyUnicode_FromString("AVLSet()");

    PyObject *parts = PyList_New(0);
    if (!parts)
        return NULL;

    AVLNode *node = avltree_minimum(self->tree, self->tree->root);
    while (node != self->tree->nil)
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
        node = avltree_successor(self->tree, node);
    }

    PyObject *sep = PyUnicode_FromString(", ");
    PyObject *joined = PyUnicode_Join(sep, parts);
    Py_DECREF(sep);
    Py_DECREF(parts);
    if (!joined)
        return NULL;

    PyObject *result = PyUnicode_FromFormat("AVLSet({%U})", joined);
    Py_DECREF(joined);
    return result;
}

static PyObject *PyAVLSet_richcmp(PyObject *self, PyObject *other, int op)
{
    if (op != Py_EQ && op != Py_NE)
        Py_RETURN_NOTIMPLEMENTED;

    if (!PyObject_TypeCheck(other, Py_TYPE(self)))
        Py_RETURN_NOTIMPLEMENTED;

    PyAVLSetObject *a = (PyAVLSetObject *)self;
    PyAVLSetObject *b = (PyAVLSetObject *)other;

    if (a->tree->size != b->tree->size)
    {
        if (op == Py_EQ)
            Py_RETURN_FALSE;
        Py_RETURN_TRUE;
    }

    AVLNode *na = avltree_minimum(a->tree, a->tree->root);
    AVLNode *nb = avltree_minimum(b->tree, b->tree->root);

    while (na != a->tree->nil)
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
        na = avltree_successor(a->tree, na);
        nb = avltree_successor(b->tree, nb);
    }

    if (op == Py_EQ)
        Py_RETURN_TRUE;
    Py_RETURN_FALSE;
}

static PySequenceMethods PyAVLSet_as_sequence = {
    .sq_length = (lenfunc)PyAVLSet_len,
    .sq_contains = (objobjproc)PyAVLSet_contains,
};

PyTypeObject PyAVLSetType = {
    PyVarObject_HEAD_INIT(NULL, 0)
        .tp_name = "juniper._native.AVLSet",
    .tp_basicsize = sizeof(PyAVLSetObject),
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    .tp_new = PyAVLSet_new,
    .tp_dealloc = (destructor)PyAVLSet_dealloc,
    .tp_methods = PyAVLSet_methods,
    .tp_as_sequence = &PyAVLSet_as_sequence,
    .tp_iter = (getiterfunc)PyAVLSet_iter,
    .tp_repr = (reprfunc)PyAVLSet_repr,
    .tp_richcompare = PyAVLSet_richcmp,
    .tp_doc = "Sorted set based on an AVL tree.",
};
