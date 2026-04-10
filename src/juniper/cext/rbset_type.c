#include <Python.h>
#include "rbtree.h"

typedef struct {
    PyObject_HEAD
    RBTree *tree;
} PyRBSetObject;

typedef struct {
    PyObject_HEAD
    PyRBSetObject *set;
    RBNode *next;
    size_t si;
} PyRBSetIterObject;

static PyTypeObject PyRBSetIterType;

static void PyRBSetIter_dealloc(PyRBSetIterObject *it)
{
    Py_XDECREF(it->set);
    Py_TYPE(it)->tp_free((PyObject *)it);
}

static PyObject *PyRBSetIter_next(PyRBSetIterObject *it)
{
    if (!it->set || !it->set->tree)
        return NULL;

    RBTree *tree = it->set->tree;

    if (tree->size != it->si) {
        PyErr_SetString(PyExc_RuntimeError,
                        "Set changed size during iteration");
        return NULL;
    }

    if (it->next == tree->nil)
        return NULL;

    PyObject *key = it->next->key;
    Py_INCREF(key);

    it->next = rbtree_successor(tree, it->next);
    return key;
}

static PyObject *PyRBSetIter_iter(PyObject *self)
{
    Py_INCREF(self);
    return self;
}

static PyTypeObject PyRBSetIterType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name      = "juniper._native.RBSetIterator",
    .tp_basicsize = sizeof(PyRBSetIterObject),
    .tp_flags     = Py_TPFLAGS_DEFAULT,
    .tp_dealloc   = (destructor)PyRBSetIter_dealloc,
    .tp_iter      = PyRBSetIter_iter,
    .tp_iternext  = (iternextfunc)PyRBSetIter_next,
};

static PyObject *PyRBSet_new(PyTypeObject *type,
                             PyObject *args, PyObject *kwds)
{
    PyRBSetObject *self;
    PyObject *iterable = NULL;

    if (!PyArg_ParseTuple(args, "|O", &iterable))
        return NULL;

    self = (PyRBSetObject *)type->tp_alloc(type, 0);
    if (!self) return NULL;

    self->tree = rbtree_create();
    if (!self->tree) {
        Py_DECREF(self);
        return PyErr_NoMemory();
    }

    if (iterable) {
        PyObject *iter = PyObject_GetIter(iterable);
        if (!iter) {
            Py_DECREF(self);
            return NULL;
        }
        PyObject *item;
        while ((item = PyIter_Next(iter))) {
            int rc = rbtree_insert(self->tree, item);
            Py_DECREF(item);
            if (rc < 0) {
                Py_DECREF(iter);
                Py_DECREF(self);
                return NULL;
            }
        }
        Py_DECREF(iter);
        if (PyErr_Occurred()) {
            Py_DECREF(self);
            return NULL;
        }
    }

    return (PyObject *)self;
}

static void PyRBSet_dealloc(PyRBSetObject *self)
{
    if (self->tree)
        rbtree_free(self->tree);
    Py_TYPE(self)->tp_free((PyObject *)self);
}

static Py_ssize_t PyRBSet_len(PyRBSetObject *self)
{
    return (Py_ssize_t)self->tree->size;
}

static int PyRBSet_contains(PyRBSetObject *self, PyObject *key)
{
    return rbtree_contains(self->tree, key);
}

static PyObject *PyRBSet_add(PyRBSetObject *self, PyObject *args)
{
    PyObject *key;
    if (!PyArg_ParseTuple(args, "O", &key))
        return NULL;

    int rc = rbtree_insert(self->tree, key);
    if (rc < 0) return NULL;
    Py_RETURN_NONE;
}

static PyObject *PyRBSet_discard(PyRBSetObject *self, PyObject *args)
{
    PyObject *key;
    if (!PyArg_ParseTuple(args, "O", &key))
        return NULL;

    int rc = rbtree_remove(self->tree, key);
    if (rc < 0) return NULL;
    Py_RETURN_NONE;
}

static PyObject *PyRBSet_remove(PyRBSetObject *self, PyObject *args)
{
    PyObject *key;
    if (!PyArg_ParseTuple(args, "O", &key))
        return NULL;

    int rc = rbtree_remove(self->tree, key);
    if (rc < 0) return NULL;
    if (rc == 0) {
        PyErr_SetObject(PyExc_KeyError, key);
        return NULL;
    }
    Py_RETURN_NONE;
}

static PyObject *PyRBSet_contains_method(PyRBSetObject *self, PyObject *args)
{
    PyObject *key;
    if (!PyArg_ParseTuple(args, "O", &key))
        return NULL;

    int rc = rbtree_contains(self->tree, key);
    if (rc < 0) return NULL;
    return PyBool_FromLong(rc);
}

static PyObject *PyRBSet_clear(PyRBSetObject *self,
                               PyObject *Py_UNUSED(ignored))
{
    if (self->tree) {
        rbtree_free(self->tree);
        self->tree = rbtree_create();
        if (!self->tree)
            return PyErr_NoMemory();
    }
    Py_RETURN_NONE;
}

static PyObject *PyRBSet_pop(PyRBSetObject *self,
                             PyObject *Py_UNUSED(ignored))
{
    if (self->tree->size == 0) {
        PyErr_SetString(PyExc_KeyError, "pop from an empty set");
        return NULL;
    }
    RBNode *min = rbtree_minimum(self->tree, self->tree->root);
    PyObject *key = min->key;
    Py_INCREF(key);

    int rc = rbtree_remove(self->tree, key);
    if (rc < 0) {
        Py_DECREF(key);
        return NULL;
    }
    return key;
}

static PyObject *PyRBSet_iter(PyRBSetObject *self)
{
    if (PyType_Ready(&PyRBSetIterType) < 0)
        return NULL;

    PyRBSetIterObject *it = PyObject_New(PyRBSetIterObject,
                                         &PyRBSetIterType);
    if (!it) return NULL;

    Py_INCREF(self);
    it->set = self;
    it->si  = self->tree->size;

    if (self->tree->size > 0)
        it->next = rbtree_minimum(self->tree, self->tree->root);
    else
        it->next = self->tree->nil;

    return (PyObject *)it;
}


static PyObject *PyRBSet_repr(PyRBSetObject *self)
{
    if (self->tree->size == 0)
        return PyUnicode_FromString("RBSet()");

    PyObject *parts = PyList_New(0);
    if (!parts) return NULL;

    RBNode *node = rbtree_minimum(self->tree, self->tree->root);
    while (node != self->tree->nil) {
        PyObject *r = PyObject_Repr(node->key);
        if (!r) { Py_DECREF(parts); return NULL; }
        if (PyList_Append(parts, r) < 0) {
            Py_DECREF(r); Py_DECREF(parts); return NULL;
        }
        Py_DECREF(r);
        node = rbtree_successor(self->tree, node);
    }

    PyObject *sep = PyUnicode_FromString(", ");
    PyObject *joined = PyUnicode_Join(sep, parts);
    Py_DECREF(sep);
    Py_DECREF(parts);
    if (!joined) return NULL;

    PyObject *result = PyUnicode_FromFormat("RBSet({%U})", joined);
    Py_DECREF(joined);
    return result;
}

static PyObject *PyRBSet_richcmp(PyObject *self, PyObject *other, int op)
{
    if (op != Py_EQ && op != Py_NE)
        Py_RETURN_NOTIMPLEMENTED;

    if (!PyObject_TypeCheck(other, Py_TYPE(self)))
        Py_RETURN_NOTIMPLEMENTED;

    PyRBSetObject *a = (PyRBSetObject *)self;
    PyRBSetObject *b = (PyRBSetObject *)other;

    if (a->tree->size != b->tree->size) {
        if (op == Py_EQ) Py_RETURN_FALSE;
        Py_RETURN_TRUE;
    }

    RBNode *na = rbtree_minimum(a->tree, a->tree->root);
    RBNode *nb = rbtree_minimum(b->tree, b->tree->root);

    while (na != a->tree->nil) {
        int eq = PyObject_RichCompareBool(na->key, nb->key, Py_EQ);
        if (eq < 0) return NULL;
        if (!eq) {
            if (op == Py_EQ) Py_RETURN_FALSE;
            Py_RETURN_TRUE;
        }
        na = rbtree_successor(a->tree, na);
        nb = rbtree_successor(b->tree, nb);
    }

    if (op == Py_EQ) Py_RETURN_TRUE;
    Py_RETURN_FALSE;
}


static PyMethodDef PyRBSet_methods[] = {
    {"add",      (PyCFunction)PyRBSet_add,      METH_VARARGS,
     "Add an element to the set."},
    {"discard",  (PyCFunction)PyRBSet_discard,   METH_VARARGS,
     "Remove an element if present (no error if missing)."},
    {"remove",   (PyCFunction)PyRBSet_remove,    METH_VARARGS,
     "Remove an element; raise KeyError if missing."},
    {"contains", (PyCFunction)PyRBSet_contains_method, METH_VARARGS,
     "Return True if element is in the set."},
    {"clear",    (PyCFunction)PyRBSet_clear,     METH_NOARGS,
     "Remove all elements."},
    {"pop",      (PyCFunction)PyRBSet_pop,       METH_NOARGS,
     "Remove and return the smallest element."},
    {NULL}
};

static PySequenceMethods PyRBSet_as_sequence = {
    .sq_length   = (lenfunc)PyRBSet_len,
    .sq_contains = (objobjproc)PyRBSet_contains,
};

PyTypeObject PyRBSetType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name        = "juniper._native.RBSet",
    .tp_basicsize   = sizeof(PyRBSetObject),
    .tp_flags       = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    .tp_new         = PyRBSet_new,
    .tp_dealloc     = (destructor)PyRBSet_dealloc,
    .tp_methods     = PyRBSet_methods,
    .tp_as_sequence = &PyRBSet_as_sequence,
    .tp_iter        = (getiterfunc)PyRBSet_iter,
    .tp_repr        = (reprfunc)PyRBSet_repr,
    .tp_richcompare = PyRBSet_richcmp,
    .tp_doc         = "Sorted set based on a red-black tree.",
};
