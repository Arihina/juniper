#include <Python.h>
#include <string.h>
#include "hashmap/hashmap.h"
#include "hashmap/hashmap_iter.h"

typedef struct {
    PyObject_HEAD
    hashmap *hm;
} PyHashMap;

typedef enum {
    HM_ITER_KEYS,
    HM_ITER_VALUES,
    HM_ITER_ITEMS
} hm_iter_mode;

typedef struct {
    PyObject_HEAD
    hm_iter it;
    hashmap *hm;
    hm_iter_mode mode;
} PyHMIter;

static size_t py_hash(void *o) {
    return PyObject_Hash((PyObject*)o);
}

static int py_cmp(void *a, void *b) {
    return PyObject_RichCompareBool(a, b, Py_EQ) == 1 ? 0 : 1;
}

static PyObject* hm_new_py(PyTypeObject *t, PyObject *a, PyObject *k) {
    PyHashMap *self = (PyHashMap*)t->tp_alloc(t, 0);
    if (!self) return NULL;

    self->hm = hm_new(16, py_hash, py_cmp);
    return (PyObject*)self;
}

static Py_ssize_t hm_len_py(PyHashMap *self) {
    return hm_len(self->hm);
}

static PyObject* hm_get_py(PyHashMap *self, PyObject *key) {
    void *v = hm_get(self->hm, key);
    if (!v) {
        PyErr_SetObject(PyExc_KeyError, key);
        return NULL;
    }

    Py_INCREF((PyObject*)v);
    return (PyObject*)v;
}

static int hm_ass_subscript(PyHashMap *self, PyObject *key, PyObject *val) {
    if (val == NULL) {
        void *old = hm_get(self->hm, key);

        if (!old) {
            PyErr_SetObject(PyExc_KeyError, key);
            return -1;
        }
        Py_DECREF((PyObject*)old);

        hm_del(self->hm, key);
        return 0;
    }

    void *old = hm_get(self->hm, key);

    if (old) {
        Py_DECREF((PyObject*)old);
        Py_INCREF(val);

        hm_set(self->hm, key, val);
    } else {
        Py_INCREF(key);
        Py_INCREF(val);

        hm_set(self->hm, key, val);
    }

    return 0;
}

static PyObject* PyHMIter_iter(PyObject *self) {
    Py_INCREF(self);
    return self;
}

static PyObject* PyHMIter_next(PyHMIter *self) {
    void *k = hm_iter_next(&self->it);

    if (!k) {
        PyErr_SetNone(PyExc_StopIteration);
        return NULL;
    }

    void *v = hm_get(self->hm, k);
    if (!v) {
        PyErr_SetString(PyExc_RuntimeError, "HashMap corrupted");
        return NULL;
    }

    PyObject *key = (PyObject*)k;
    PyObject *val = (PyObject*)v;

    if (self->mode == HM_ITER_KEYS) {
        Py_INCREF(key);
        return key;
    }

    if (self->mode == HM_ITER_VALUES) {
        Py_INCREF(val);
        return val;
    }

    Py_INCREF(key);
    Py_INCREF(val);

    return PyTuple_Pack(2, key, val);
}

PyTypeObject PyHMIterType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "juniper.HashMapIterator",
    .tp_basicsize = sizeof(PyHMIter),
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_iter = PyHMIter_iter,
    .tp_iternext = (iternextfunc)PyHMIter_next,
};

static PyObject* hm_make_iter(PyHashMap *self, hm_iter_mode mode) {
    PyHMIter *it = PyObject_New(PyHMIter, &PyHMIterType);
    if (!it) return NULL;

    memset(&it->it, 0, sizeof(hm_iter));
    hm_iter_init(&it->it, self->hm);

    it->hm = self->hm;
    it->mode = mode;

    return (PyObject*)it;
}

static PyObject* hm_iter_py(PyHashMap *self) {
    return hm_make_iter(self, HM_ITER_KEYS);
}

static PyObject* hm_keys(PyHashMap *self, PyObject *Py_UNUSED(ignored)) {
    return hm_make_iter(self, HM_ITER_KEYS);
}

static PyObject* hm_values(PyHashMap *self, PyObject *Py_UNUSED(ignored)) {
    return hm_make_iter(self, HM_ITER_VALUES);
}

static PyObject* hm_items(PyHashMap *self, PyObject *Py_UNUSED(ignored)) {
    return hm_make_iter(self, HM_ITER_ITEMS);
}

static void hm_dealloc(PyHashMap *self) {
    hm_iter it;
    hm_iter_init(&it, self->hm);

    void *k;
    while ((k = hm_iter_next(&it))) {
        void *v = hm_get(self->hm, k);

        Py_DECREF((PyObject*)k);
        if (v) {
            Py_DECREF((PyObject*)v);
        }
    }
    
    hm_free(self->hm);

    Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyMethodDef hashmap_methods[] = {
    {"keys", (PyCFunction)hm_keys, METH_NOARGS, "Return keys iterator"},
    {"values", (PyCFunction)hm_values, METH_NOARGS, "Return values iterator"},
    {"items", (PyCFunction)hm_items, METH_NOARGS, "Return items iterator"},
    {NULL}
};

static PyMappingMethods map = {
    (lenfunc)hm_len_py,
    (binaryfunc)hm_get_py,
    (objobjargproc)hm_ass_subscript
};

PyTypeObject HashMapType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "juniper.HashMap",
    .tp_basicsize = sizeof(PyHashMap),
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    .tp_new = hm_new_py,
    .tp_dealloc = (destructor)hm_dealloc,
    .tp_as_mapping = &map,
    .tp_iter = (getiterfunc)hm_iter_py,
    .tp_methods = hashmap_methods
};
