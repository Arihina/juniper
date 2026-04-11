#include <Python.h>
#include "rhmap.h"

typedef struct {
    PyObject_HEAD
    RHMap *map;
} PyRHMapObject;

typedef enum { ITER_KEYS, ITER_VALUES, ITER_ITEMS } IterKind;

typedef struct {
    PyObject_HEAD
    PyRHMapObject *owner;
    size_t pos;
    size_t snap_size;
    IterKind kind;
} PyRHMapIterObject;

static PyTypeObject PyRHMapIterType;

static void PyRHMapIter_dealloc(PyRHMapIterObject *it)
{
    Py_XDECREF(it->owner);
    Py_TYPE(it)->tp_free((PyObject *)it);
}

static PyObject *PyRHMapIter_next(PyRHMapIterObject *it)
{
    if (!it->owner || !it->owner->map)
        return NULL;

    RHMap *map = it->owner->map;

    if (map->size != it->snap_size) {
        PyErr_SetString(PyExc_RuntimeError,
                        "RHMap changed size during iteration");
        return NULL;
    }

    while (it->pos < map->capacity) {
        RHSlot *s = &map->slots[it->pos];
        it->pos++;
        if (s->state == RHMAP_USED) {
            switch (it->kind) {
            case ITER_KEYS:
                Py_INCREF(s->key);
                return s->key;
            case ITER_VALUES:
                Py_INCREF(s->value);
                return s->value;
            case ITER_ITEMS: {
                PyObject *tuple = PyTuple_New(2);
                if (!tuple) return NULL;
                Py_INCREF(s->key);
                Py_INCREF(s->value);
                PyTuple_SET_ITEM(tuple, 0, s->key);
                PyTuple_SET_ITEM(tuple, 1, s->value);
                return tuple;
            }
            }
        }
    }
    return NULL;
}

static PyObject *PyRHMapIter_iter(PyObject *self)
{
    Py_INCREF(self);
    return self;
}

static PyTypeObject PyRHMapIterType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name      = "juniper._native.RHMapIterator",
    .tp_basicsize = sizeof(PyRHMapIterObject),
    .tp_flags     = Py_TPFLAGS_DEFAULT,
    .tp_dealloc   = (destructor)PyRHMapIter_dealloc,
    .tp_iter      = PyRHMapIter_iter,
    .tp_iternext  = (iternextfunc)PyRHMapIter_next,
};

static PyObject *make_iter(PyRHMapObject *self, IterKind kind)
{
    if (PyType_Ready(&PyRHMapIterType) < 0)
        return NULL;

    PyRHMapIterObject *it = PyObject_New(PyRHMapIterObject,
                                         &PyRHMapIterType);
    if (!it) return NULL;

    Py_INCREF(self);
    it->owner     = self;
    it->pos       = 0;
    it->snap_size = self->map->size;
    it->kind      = kind;
    return (PyObject *)it;
}

static PyObject *PyRHMap_new(PyTypeObject *type,
                             PyObject *args, PyObject *kwds)
{
    PyRHMapObject *self;
    PyObject *arg = NULL;
    size_t capacity = 16;

    if (!PyArg_ParseTuple(args, "|O", &arg))
        return NULL;

    if (arg && PyLong_Check(arg)) {
        capacity = (size_t)PyLong_AsSize_t(arg);
        if (capacity == (size_t)-1 && PyErr_Occurred())
            return NULL;
        arg = NULL;
    }

    self = (PyRHMapObject *)type->tp_alloc(type, 0);
    if (!self) return NULL;

    self->map = rhmap_create(capacity);
    if (!self->map) {
        Py_DECREF(self);
        return PyErr_NoMemory();
    }

    if (arg && PyDict_Check(arg)) {
        PyObject *k, *v;
        Py_ssize_t pos = 0;
        while (PyDict_Next(arg, &pos, &k, &v)) {
            if (rhmap_put(self->map, k, v) < 0) {
                Py_DECREF(self);
                return NULL;
            }
        }
    }
    else if (arg) {
        PyObject *iter = PyObject_GetIter(arg);
        if (!iter) { Py_DECREF(self); return NULL; }
        PyObject *item;
        while ((item = PyIter_Next(iter))) {
            PyObject *k, *v;
            if (!PyArg_ParseTuple(item, "OO", &k, &v)) {
                Py_DECREF(item);
                Py_DECREF(iter);
                Py_DECREF(self);
                return NULL;
            }
            Py_DECREF(item);
            if (rhmap_put(self->map, k, v) < 0) {
                Py_DECREF(iter);
                Py_DECREF(self);
                return NULL;
            }
        }
        Py_DECREF(iter);
        if (PyErr_Occurred()) { Py_DECREF(self); return NULL; }
    }

    if (kwds && PyDict_Size(kwds) > 0) {
        PyObject *k, *v;
        Py_ssize_t pos = 0;
        while (PyDict_Next(kwds, &pos, &k, &v)) {
            if (rhmap_put(self->map, k, v) < 0) {
                Py_DECREF(self);
                return NULL;
            }
        }
    }

    return (PyObject *)self;
}

static void PyRHMap_dealloc(PyRHMapObject *self)
{
    if (self->map)
        rhmap_free(self->map);
    Py_TYPE(self)->tp_free((PyObject *)self);
}

static Py_ssize_t PyRHMap_len(PyRHMapObject *self)
{
    return (Py_ssize_t)self->map->size;
}

static PyObject *PyRHMap_subscript(PyRHMapObject *self, PyObject *key)
{
    PyObject *val = rhmap_get(self->map, key);
    if (!val) {
        if (!PyErr_Occurred())
            PyErr_SetObject(PyExc_KeyError, key);
        return NULL;
    }
    return val;
}

static int PyRHMap_ass_subscript(PyRHMapObject *self,
                                 PyObject *key, PyObject *value)
{
    if (!value) {
        /* del m[key] */
        int rc = rhmap_remove(self->map, key);
        if (rc < 0) return -1;
        if (rc == 0) {
            PyErr_SetObject(PyExc_KeyError, key);
            return -1;
        }
        return 0;
    }
    int rc = rhmap_put(self->map, key, value);
    if (rc < 0) return -1;
    return 0;
}

static PyMappingMethods PyRHMap_as_mapping = {
    .mp_length        = (lenfunc)PyRHMap_len,
    .mp_subscript     = (binaryfunc)PyRHMap_subscript,
    .mp_ass_subscript = (objobjargproc)PyRHMap_ass_subscript,
};

static int PyRHMap_sq_contains(PyRHMapObject *self, PyObject *key)
{
    return rhmap_contains(self->map, key);
}

static PySequenceMethods PyRHMap_as_sequence = {
    .sq_contains = (objobjproc)PyRHMap_sq_contains,
};

static PyObject *PyRHMap_put(PyRHMapObject *self, PyObject *args)
{
    PyObject *key, *value;
    if (!PyArg_ParseTuple(args, "OO", &key, &value)) return NULL;
    int rc = rhmap_put(self->map, key, value);
    if (rc < 0) return NULL;
    Py_RETURN_NONE;
}

static PyObject *PyRHMap_get_method(PyRHMapObject *self, PyObject *args)
{
    PyObject *key, *defval = Py_None;
    if (!PyArg_ParseTuple(args, "O|O", &key, &defval)) return NULL;
    PyObject *val = rhmap_get(self->map, key);
    if (!val) {
        if (PyErr_Occurred()) return NULL;
        Py_INCREF(defval);
        return defval;
    }
    return val;
}

static PyObject *PyRHMap_remove(PyRHMapObject *self, PyObject *args)
{
    PyObject *key;
    if (!PyArg_ParseTuple(args, "O", &key)) return NULL;
    int rc = rhmap_remove(self->map, key);
    if (rc < 0) return NULL;
    if (rc == 0) {
        PyErr_SetObject(PyExc_KeyError, key);
        return NULL;
    }
    Py_RETURN_NONE;
}

static PyObject *PyRHMap_contains_method(PyRHMapObject *self, PyObject *args)
{
    PyObject *key;
    if (!PyArg_ParseTuple(args, "O", &key)) return NULL;
    int rc = rhmap_contains(self->map, key);
    if (rc < 0) return NULL;
    return PyBool_FromLong(rc);
}

static PyObject *PyRHMap_clear(PyRHMapObject *self,
                               PyObject *Py_UNUSED(ignored))
{
    rhmap_clear(self->map);
    Py_RETURN_NONE;
}

static PyObject *PyRHMap_keys(PyRHMapObject *self,
                              PyObject *Py_UNUSED(ignored))
{
    return make_iter(self, ITER_KEYS);
}

static PyObject *PyRHMap_values(PyRHMapObject *self,
                                PyObject *Py_UNUSED(ignored))
{
    return make_iter(self, ITER_VALUES);
}

static PyObject *PyRHMap_items(PyRHMapObject *self,
                               PyObject *Py_UNUSED(ignored))
{
    return make_iter(self, ITER_ITEMS);
}

static PyObject *PyRHMap_pop(PyRHMapObject *self, PyObject *args)
{
    PyObject *key, *defval = NULL;
    if (!PyArg_ParseTuple(args, "O|O", &key, &defval)) return NULL;

    PyObject *val = rhmap_get(self->map, key);
    if (val) {
        rhmap_remove(self->map, key);
        return val;
    }
    if (PyErr_Occurred()) return NULL;
    if (defval) {
        Py_INCREF(defval);
        return defval;
    }
    PyErr_SetObject(PyExc_KeyError, key);
    return NULL;
}

static PyMethodDef PyRHMap_methods[] = {
    {"put",      (PyCFunction)PyRHMap_put,      METH_VARARGS,
     "Insert or update key/value pair."},
    {"get",      (PyCFunction)PyRHMap_get_method, METH_VARARGS,
     "Get value by key with optional default."},
    {"remove",   (PyCFunction)PyRHMap_remove,   METH_VARARGS,
     "Remove key; raise KeyError if missing."},
    {"contains", (PyCFunction)PyRHMap_contains_method, METH_VARARGS,
     "Return True if key exists."},
    {"clear",    (PyCFunction)PyRHMap_clear,    METH_NOARGS,
     "Remove all entries."},
    {"keys",     (PyCFunction)PyRHMap_keys,     METH_NOARGS,
     "Return an iterator over keys."},
    {"values",   (PyCFunction)PyRHMap_values,   METH_NOARGS,
     "Return an iterator over values."},
    {"items",    (PyCFunction)PyRHMap_items,     METH_NOARGS,
     "Return an iterator over (key, value) pairs."},
    {"pop",      (PyCFunction)PyRHMap_pop,      METH_VARARGS,
     "Remove key and return value; return default if given, else KeyError."},
    {NULL}
};

static PyObject *PyRHMap_iter(PyRHMapObject *self)
{
    return make_iter(self, ITER_KEYS);
}

static PyObject *PyRHMap_repr(PyRHMapObject *self)
{
    if (self->map->size == 0)
        return PyUnicode_FromString("RHMap({})");

    PyObject *parts = PyList_New(0);
    if (!parts) return NULL;

    for (size_t i = 0; i < self->map->capacity; i++) {
        RHSlot *s = &self->map->slots[i];
        if (s->state != RHMAP_USED) continue;

        PyObject *kr = PyObject_Repr(s->key);
        if (!kr) { Py_DECREF(parts); return NULL; }
        PyObject *vr = PyObject_Repr(s->value);
        if (!vr) { Py_DECREF(kr); Py_DECREF(parts); return NULL; }

        PyObject *pair = PyUnicode_FromFormat("%U: %U", kr, vr);
        Py_DECREF(kr);
        Py_DECREF(vr);
        if (!pair) { Py_DECREF(parts); return NULL; }

        if (PyList_Append(parts, pair) < 0) {
            Py_DECREF(pair); Py_DECREF(parts); return NULL;
        }
        Py_DECREF(pair);
    }

    PyObject *sep = PyUnicode_FromString(", ");
    PyObject *joined = PyUnicode_Join(sep, parts);
    Py_DECREF(sep);
    Py_DECREF(parts);
    if (!joined) return NULL;

    PyObject *result = PyUnicode_FromFormat("RHMap({%U})", joined);
    Py_DECREF(joined);
    return result;
}

static PyObject *PyRHMap_richcmp(PyObject *self, PyObject *other, int op)
{
    if (op != Py_EQ && op != Py_NE)
        Py_RETURN_NOTIMPLEMENTED;

    if (!PyObject_TypeCheck(other, Py_TYPE(self)))
        Py_RETURN_NOTIMPLEMENTED;

    PyRHMapObject *a = (PyRHMapObject *)self;
    PyRHMapObject *b = (PyRHMapObject *)other;

    if (a->map->size != b->map->size) {
        if (op == Py_EQ) Py_RETURN_FALSE;
        Py_RETURN_TRUE;
    }

    for (size_t i = 0; i < a->map->capacity; i++) {
        RHSlot *sa = &a->map->slots[i];
        if (sa->state != RHMAP_USED) continue;

        PyObject *vb = rhmap_get(b->map, sa->key);
        if (!vb) {
            if (PyErr_Occurred()) return NULL;
            if (op == Py_EQ) Py_RETURN_FALSE;
            Py_RETURN_TRUE;
        }
        int eq = PyObject_RichCompareBool(sa->value, vb, Py_EQ);
        Py_DECREF(vb);
        if (eq < 0) return NULL;
        if (!eq) {
            if (op == Py_EQ) Py_RETURN_FALSE;
            Py_RETURN_TRUE;
        }
    }

    if (op == Py_EQ) Py_RETURN_TRUE;
    Py_RETURN_FALSE;
}

PyTypeObject PyRHMapType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name        = "juniper._native.RHMap",
    .tp_basicsize   = sizeof(PyRHMapObject),
    .tp_flags       = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    .tp_new         = PyRHMap_new,
    .tp_dealloc     = (destructor)PyRHMap_dealloc,
    .tp_methods     = PyRHMap_methods,
    .tp_as_mapping  = &PyRHMap_as_mapping,
    .tp_as_sequence = &PyRHMap_as_sequence,
    .tp_iter        = (getiterfunc)PyRHMap_iter,
    .tp_repr        = (reprfunc)PyRHMap_repr,
    .tp_richcompare = PyRHMap_richcmp,
    .tp_doc         = "Robin Hood hash map implemented in C.",
};
