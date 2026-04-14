#include <Python.h>
#include "bthashmap.h"

typedef struct {
    PyObject_HEAD
    BTHashMap *map;
} PyBTHashMapObject;

typedef enum { BHI_KEYS, BHI_VALUES, BHI_ITEMS } BHIterKind;

typedef struct {
    PyObject_HEAD
    PyBTHashMapObject *owner;
    size_t bucket_idx;
    BTMPos pos;
    size_t snap_size;
    BHIterKind kind;
} PyBTHashMapIterObject;

static PyTypeObject PyBTHashMapIterType;

static void advance_to_next_bucket(PyBTHashMapIterObject *it)
{
    BTHashMap *map = it->owner->map;
    while (it->bucket_idx < map->nbuckets) {
        BTreeMap *bt = map->buckets[it->bucket_idx];
        if (bt && bt->size > 0) {
            it->pos = btm_first(bt);
            if (it->pos.node) return;
        }
        it->bucket_idx++;
    }
    it->pos.node = NULL;
    it->pos.idx  = 0;
}

static void PyBTHashMapIter_dealloc(PyBTHashMapIterObject *it)
{
    Py_XDECREF(it->owner);
    Py_TYPE(it)->tp_free((PyObject *)it);
}

static PyObject *PyBTHashMapIter_next(PyBTHashMapIterObject *it)
{
    if (!it->owner || !it->owner->map)
        return NULL;

    BTHashMap *map = it->owner->map;

    if (map->size != it->snap_size) {
        PyErr_SetString(PyExc_RuntimeError,
                        "BTHashMap changed size during iteration");
        return NULL;
    }

    if (!it->pos.node)
        return NULL;

    BTMNode *node = it->pos.node;
    int idx = it->pos.idx;
    PyObject *result = NULL;

    switch (it->kind) {
    case BHI_KEYS:
        result = node->keys[idx];
        Py_INCREF(result);
        break;
    case BHI_VALUES:
        result = node->values[idx];
        Py_INCREF(result);
        break;
    case BHI_ITEMS: {
        PyObject *tuple = PyTuple_New(2);
        if (!tuple) return NULL;
        Py_INCREF(node->keys[idx]);
        Py_INCREF(node->values[idx]);
        PyTuple_SET_ITEM(tuple, 0, node->keys[idx]);
        PyTuple_SET_ITEM(tuple, 1, node->values[idx]);
        result = tuple;
        break;
    }
    }

    /* Advance position */
    BTreeMap *bt = map->buckets[it->bucket_idx];
    it->pos = btm_next(bt, it->pos);
    if (!it->pos.node) {
        it->bucket_idx++;
        advance_to_next_bucket(it);
    }

    return result;
}

static PyObject *PyBTHashMapIter_iter(PyObject *self)
{
    Py_INCREF(self);
    return self;
}

static PyTypeObject PyBTHashMapIterType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name      = "juniper._native.BTHashMapIterator",
    .tp_basicsize = sizeof(PyBTHashMapIterObject),
    .tp_flags     = Py_TPFLAGS_DEFAULT,
    .tp_dealloc   = (destructor)PyBTHashMapIter_dealloc,
    .tp_iter      = PyBTHashMapIter_iter,
    .tp_iternext  = (iternextfunc)PyBTHashMapIter_next,
};

static PyObject *make_iter(PyBTHashMapObject *self, BHIterKind kind)
{
    if (PyType_Ready(&PyBTHashMapIterType) < 0)
        return NULL;

    PyBTHashMapIterObject *it = PyObject_New(PyBTHashMapIterObject,
                                             &PyBTHashMapIterType);
    if (!it) return NULL;

    Py_INCREF(self);
    it->owner      = self;
    it->snap_size  = self->map->size;
    it->kind       = kind;
    it->bucket_idx = 0;
    it->pos.node   = NULL;
    it->pos.idx    = 0;

    advance_to_next_bucket(it);
    return (PyObject *)it;
}

static PyObject *PyBTHashMap_new(PyTypeObject *type,
                                 PyObject *args, PyObject *kwds)
{
    PyBTHashMapObject *self;
    PyObject *arg = NULL;
    int nbuckets = BTHM_DEFAULT_NBUCKETS;
    int bt_order = BTHM_BUCKET_ORDER;

    static char *kwlist[] = {"", "nbuckets", "bt_order", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "|Oii", kwlist,
                                     &arg, &nbuckets, &bt_order))
        return NULL;

    self = (PyBTHashMapObject *)type->tp_alloc(type, 0);
    if (!self) return NULL;

    self->map = bthm_create((size_t)nbuckets, bt_order);
    if (!self->map) {
        Py_DECREF(self);
        return PyErr_NoMemory();
    }

    if (arg && PyDict_Check(arg)) {
        PyObject *k, *v;
        Py_ssize_t pos = 0;
        while (PyDict_Next(arg, &pos, &k, &v)) {
            if (bthm_put(self->map, k, v) < 0) {
                Py_DECREF(self); return NULL;
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
                Py_DECREF(item); Py_DECREF(iter);
                Py_DECREF(self); return NULL;
            }
            Py_DECREF(item);
            if (bthm_put(self->map, k, v) < 0) {
                Py_DECREF(iter); Py_DECREF(self); return NULL;
            }
        }
        Py_DECREF(iter);
        if (PyErr_Occurred()) { Py_DECREF(self); return NULL; }
    }

    if (kwds && PyDict_Size(kwds) > 0) {
        PyObject *k, *v;
        Py_ssize_t pos = 0;
        while (PyDict_Next(kwds, &pos, &k, &v)) {
            const char *ks = PyUnicode_AsUTF8(k);
            if (ks && (strcmp(ks, "nbuckets") == 0 ||
                       strcmp(ks, "bt_order") == 0))
                continue;
            if (bthm_put(self->map, k, v) < 0) {
                Py_DECREF(self); return NULL;
            }
        }
    }

    return (PyObject *)self;
}

static void PyBTHashMap_dealloc(PyBTHashMapObject *self)
{
    if (self->map)
        bthm_free(self->map);
    Py_TYPE(self)->tp_free((PyObject *)self);
}

static Py_ssize_t PyBTHashMap_len(PyBTHashMapObject *self)
{
    return (Py_ssize_t)self->map->size;
}

static PyObject *PyBTHashMap_subscript(PyBTHashMapObject *self, PyObject *key)
{
    PyObject *val = bthm_get(self->map, key);
    if (!val) {
        if (!PyErr_Occurred())
            PyErr_SetObject(PyExc_KeyError, key);
        return NULL;
    }
    return val;
}

static int PyBTHashMap_ass_subscript(PyBTHashMapObject *self,
                                     PyObject *key, PyObject *value)
{
    if (!value) {
        int rc = bthm_remove(self->map, key);
        if (rc < 0) return -1;
        if (rc == 0) {
            PyErr_SetObject(PyExc_KeyError, key);
            return -1;
        }
        return 0;
    }
    int rc = bthm_put(self->map, key, value);
    if (rc < 0) return -1;
    return 0;
}

static PyMappingMethods PyBTHashMap_as_mapping = {
    .mp_length        = (lenfunc)PyBTHashMap_len,
    .mp_subscript     = (binaryfunc)PyBTHashMap_subscript,
    .mp_ass_subscript = (objobjargproc)PyBTHashMap_ass_subscript,
};

static int PyBTHashMap_sq_contains(PyBTHashMapObject *self, PyObject *key)
{
    return bthm_contains(self->map, key);
}

static PySequenceMethods PyBTHashMap_as_sequence = {
    .sq_contains = (objobjproc)PyBTHashMap_sq_contains,
};

static PyObject *PyBTHashMap_put(PyBTHashMapObject *self, PyObject *args)
{
    PyObject *key, *value;
    if (!PyArg_ParseTuple(args, "OO", &key, &value)) return NULL;
    int rc = bthm_put(self->map, key, value);
    if (rc < 0) return NULL;
    Py_RETURN_NONE;
}

static PyObject *PyBTHashMap_get_method(PyBTHashMapObject *self, PyObject *args)
{
    PyObject *key, *defval = Py_None;
    if (!PyArg_ParseTuple(args, "O|O", &key, &defval)) return NULL;
    PyObject *val = bthm_get(self->map, key);
    if (!val) {
        if (PyErr_Occurred()) return NULL;
        Py_INCREF(defval);
        return defval;
    }
    return val;
}

static PyObject *PyBTHashMap_remove(PyBTHashMapObject *self, PyObject *args)
{
    PyObject *key;
    if (!PyArg_ParseTuple(args, "O", &key)) return NULL;
    int rc = bthm_remove(self->map, key);
    if (rc < 0) return NULL;
    if (rc == 0) {
        PyErr_SetObject(PyExc_KeyError, key);
        return NULL;
    }
    Py_RETURN_NONE;
}

static PyObject *PyBTHashMap_contains_method(PyBTHashMapObject *self,
                                             PyObject *args)
{
    PyObject *key;
    if (!PyArg_ParseTuple(args, "O", &key)) return NULL;
    int rc = bthm_contains(self->map, key);
    if (rc < 0) return NULL;
    return PyBool_FromLong(rc);
}

static PyObject *PyBTHashMap_clear(PyBTHashMapObject *self,
                                   PyObject *Py_UNUSED(ignored))
{
    bthm_clear(self->map);
    Py_RETURN_NONE;
}

static PyObject *PyBTHashMap_keys(PyBTHashMapObject *self,
                                  PyObject *Py_UNUSED(ignored))
{
    return make_iter(self, BHI_KEYS);
}

static PyObject *PyBTHashMap_values(PyBTHashMapObject *self,
                                    PyObject *Py_UNUSED(ignored))
{
    return make_iter(self, BHI_VALUES);
}

static PyObject *PyBTHashMap_items(PyBTHashMapObject *self,
                                   PyObject *Py_UNUSED(ignored))
{
    return make_iter(self, BHI_ITEMS);
}

static PyObject *PyBTHashMap_pop(PyBTHashMapObject *self, PyObject *args)
{
    PyObject *key, *defval = NULL;
    if (!PyArg_ParseTuple(args, "O|O", &key, &defval)) return NULL;

    PyObject *val = bthm_get(self->map, key);
    if (val) {
        bthm_remove(self->map, key);
        return val;
    }
    if (PyErr_Occurred()) return NULL;
    if (defval) { Py_INCREF(defval); return defval; }
    PyErr_SetObject(PyExc_KeyError, key);
    return NULL;
}

static PyMethodDef PyBTHashMap_methods[] = {
    {"put",      (PyCFunction)PyBTHashMap_put,      METH_VARARGS,
     "Insert or update key/value pair."},
    {"get",      (PyCFunction)PyBTHashMap_get_method, METH_VARARGS,
     "Get value by key with optional default."},
    {"remove",   (PyCFunction)PyBTHashMap_remove,   METH_VARARGS,
     "Remove key; raise KeyError if missing."},
    {"contains", (PyCFunction)PyBTHashMap_contains_method, METH_VARARGS,
     "Return True if key exists."},
    {"clear",    (PyCFunction)PyBTHashMap_clear,    METH_NOARGS,
     "Remove all entries."},
    {"keys",     (PyCFunction)PyBTHashMap_keys,     METH_NOARGS,
     "Return an iterator over keys."},
    {"values",   (PyCFunction)PyBTHashMap_values,   METH_NOARGS,
     "Return an iterator over values."},
    {"items",    (PyCFunction)PyBTHashMap_items,     METH_NOARGS,
     "Return an iterator over (key, value) pairs."},
    {"pop",      (PyCFunction)PyBTHashMap_pop,      METH_VARARGS,
     "Remove key and return value; return default if given, else KeyError."},
    {NULL}
};

static PyObject *PyBTHashMap_iter(PyBTHashMapObject *self)
{
    return make_iter(self, BHI_KEYS);
}

static PyObject *PyBTHashMap_repr(PyBTHashMapObject *self)
{
    if (self->map->size == 0)
        return PyUnicode_FromString("BTHashMap({})");

    PyObject *parts = PyList_New(0);
    if (!parts) return NULL;

    for (size_t bi = 0; bi < self->map->nbuckets; bi++) {
        BTreeMap *bt = self->map->buckets[bi];
        if (!bt) continue;

        BTMPos pos = btm_first(bt);
        while (pos.node) {
            PyObject *kr = PyObject_Repr(pos.node->keys[pos.idx]);
            if (!kr) { Py_DECREF(parts); return NULL; }
            PyObject *vr = PyObject_Repr(pos.node->values[pos.idx]);
            if (!vr) { Py_DECREF(kr); Py_DECREF(parts); return NULL; }

            PyObject *pair = PyUnicode_FromFormat("%U: %U", kr, vr);
            Py_DECREF(kr); Py_DECREF(vr);
            if (!pair) { Py_DECREF(parts); return NULL; }

            if (PyList_Append(parts, pair) < 0) {
                Py_DECREF(pair); Py_DECREF(parts); return NULL;
            }
            Py_DECREF(pair);
            pos = btm_next(bt, pos);
        }
    }

    PyObject *sep = PyUnicode_FromString(", ");
    PyObject *joined = PyUnicode_Join(sep, parts);
    Py_DECREF(sep); Py_DECREF(parts);
    if (!joined) return NULL;

    PyObject *result = PyUnicode_FromFormat("BTHashMap({%U})", joined);
    Py_DECREF(joined);
    return result;
}

static PyObject *PyBTHashMap_richcmp(PyObject *self, PyObject *other, int op)
{
    if (op != Py_EQ && op != Py_NE)
        Py_RETURN_NOTIMPLEMENTED;
    if (!PyObject_TypeCheck(other, Py_TYPE(self)))
        Py_RETURN_NOTIMPLEMENTED;

    PyBTHashMapObject *a = (PyBTHashMapObject *)self;
    PyBTHashMapObject *b = (PyBTHashMapObject *)other;

    if (a->map->size != b->map->size) {
        if (op == Py_EQ) Py_RETURN_FALSE;
        Py_RETURN_TRUE;
    }

    for (size_t bi = 0; bi < a->map->nbuckets; bi++) {
        BTreeMap *bt = a->map->buckets[bi];
        if (!bt) continue;
        BTMPos pos = btm_first(bt);
        while (pos.node) {
            PyObject *k = pos.node->keys[pos.idx];
            PyObject *va = pos.node->values[pos.idx];
            PyObject *vb = bthm_get(b->map, k);
            if (!vb) {
                if (PyErr_Occurred()) return NULL;
                if (op == Py_EQ) Py_RETURN_FALSE;
                Py_RETURN_TRUE;
            }
            int eq = PyObject_RichCompareBool(va, vb, Py_EQ);
            Py_DECREF(vb);
            if (eq < 0) return NULL;
            if (!eq) {
                if (op == Py_EQ) Py_RETURN_FALSE;
                Py_RETURN_TRUE;
            }
            pos = btm_next(bt, pos);
        }
    }

    if (op == Py_EQ) Py_RETURN_TRUE;
    Py_RETURN_FALSE;
}

PyTypeObject PyBTHashMapType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name        = "juniper._native.BTHashMap",
    .tp_basicsize   = sizeof(PyBTHashMapObject),
    .tp_flags       = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    .tp_new         = PyBTHashMap_new,
    .tp_dealloc     = (destructor)PyBTHashMap_dealloc,
    .tp_methods     = PyBTHashMap_methods,
    .tp_as_mapping  = &PyBTHashMap_as_mapping,
    .tp_as_sequence = &PyBTHashMap_as_sequence,
    .tp_iter        = (getiterfunc)PyBTHashMap_iter,
    .tp_repr        = (reprfunc)PyBTHashMap_repr,
    .tp_richcompare = PyBTHashMap_richcmp,
    .tp_doc         = "Hash map with B-tree buckets, implemented in C.",
};