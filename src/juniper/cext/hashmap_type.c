#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <string.h>
#include "hashmap/hashmap.h"
#include "hashmap/hashmap_iter.h"

typedef struct
{
    PyObject_HEAD hashmap *hm;
} PyHashMap;
typedef enum
{
    HM_ITER_KEYS,
    HM_ITER_VALUES,
    HM_ITER_ITEMS
} hm_iter_mode;
typedef struct
{
    PyObject_HEAD
        hm_iter it;
    PyHashMap *map;
    hm_iter_mode mode;
} PyHMIter;

static __thread int hm_callback_error = 0;

static size_t py_hash(void *o)
{
    Py_hash_t h = PyObject_Hash((PyObject *)o);
    if (h == -1 && PyErr_Occurred())
    {
        hm_callback_error = 1;
        return 0;
    }
    return (size_t)h;
}
static int py_cmp(void *a, void *b)
{
    int r = PyObject_RichCompareBool((PyObject *)a, (PyObject *)b, Py_EQ);
    if (r < 0)
    {
        hm_callback_error = 1;
        return -1;
    }
    return r == 1 ? 0 : 1;
}

#define BEGIN_CB() (hm_callback_error = 0)
#define CB_FAILED() (hm_callback_error)

static PyObject *hm_new_py(PyTypeObject *t, PyObject *a, PyObject *k)
{
    PyHashMap *self = (PyHashMap *)t->tp_alloc(t, 0);
    if (!self)
        return NULL;
    self->hm = hm_new(16, py_hash, py_cmp);
    if (!self->hm)
    {
        Py_DECREF(self);
        return PyErr_NoMemory();
    }
    hm_set_err_flag(self->hm, &hm_callback_error);
    return (PyObject *)self;
}

static int hm_traverse(PyHashMap *self, visitproc visit, void *arg)
{
    if (!self->hm)
        return 0;
    hm_iter it;
    hm_iter_init(&it, self->hm);
    void *k, *v;
    while (hm_iter_next(&it, &k, &v) == 1)
    {
        Py_VISIT((PyObject *)k);
        Py_VISIT((PyObject *)v);
    }
    return 0;
}
static int hm_clear(PyHashMap *self)
{
    if (!self->hm)
        return 0;
    hm_iter it;
    hm_iter_init(&it, self->hm);
    void *k, *v;
    while (hm_iter_next(&it, &k, &v) == 1)
    {
        Py_DECREF((PyObject *)k);
        Py_DECREF((PyObject *)v);
    }
    hm_free(self->hm);
    self->hm = NULL;
    return 0;
}
static void hm_dealloc(PyHashMap *self)
{
    PyObject_GC_UnTrack(self);
    hm_clear(self);
    Py_TYPE(self)->tp_free((PyObject *)self);
}

static Py_ssize_t hm_len_py(PyHashMap *self) { return (Py_ssize_t)hm_len(self->hm); }

static PyObject *hm_subscript(PyHashMap *self, PyObject *key)
{
    void *v = NULL;
    BEGIN_CB();
    int r = hm_get_ex(self->hm, key, &v);
    if (CB_FAILED())
        return NULL;
    if (r == 0)
    {
        PyErr_SetObject(PyExc_KeyError, key);
        return NULL;
    }
    Py_INCREF((PyObject *)v);
    return (PyObject *)v;
}

static int hm_ass_subscript(PyHashMap *self, PyObject *key, PyObject *val)
{
    if (val == NULL)
    {
        void *ok = NULL, *ov = NULL;
        BEGIN_CB();
        int r = hm_del_ex(self->hm, key, &ok, &ov);
        if (CB_FAILED())
            return -1;
        if (r == 0)
        {
            PyErr_SetObject(PyExc_KeyError, key);
            return -1;
        }
        Py_DECREF((PyObject *)ok);
        Py_DECREF((PyObject *)ov);
        return 0;
    }
    Py_INCREF(key);
    Py_INCREF(val);
    BEGIN_CB();
    void *res = hm_set(self->hm, key, val);
    if (CB_FAILED() || res == HM_ERROR)
    {
        Py_DECREF(key);
        Py_DECREF(val);
        if (!PyErr_Occurred())
            PyErr_NoMemory();
        return -1;
    }
    if (res == HM_NEW_KEY)
        return 0;
    Py_DECREF(key);
    Py_DECREF((PyObject *)res);
    return 0;
}

static int hm_contains(PyHashMap *self, PyObject *key)
{
    void *v = NULL;
    BEGIN_CB();
    int r = hm_get_ex(self->hm, key, &v);
    if (CB_FAILED())
        return -1;
    return r == 1 ? 1 : 0;
}

static PyObject *hm_get_method(PyHashMap *self, PyObject *args)
{
    PyObject *key;
    PyObject *def = Py_None;
    if (!PyArg_ParseTuple(args, "O|O:get", &key, &def))
        return NULL;

    void *v = NULL;
    BEGIN_CB();
    int r = hm_get_ex(self->hm, key, &v);
    if (CB_FAILED())
        return NULL;

    if (r == 1)
    {
        Py_INCREF((PyObject *)v);
        return (PyObject *)v;
    }
    Py_INCREF(def);
    return def;
}

static PyObject *hm_put(PyHashMap *self, PyObject *args)
{
    PyObject *key, *val;
    if (!PyArg_ParseTuple(args, "OO:put", &key, &val))
        return NULL;

    Py_INCREF(key);
    Py_INCREF(val);
    BEGIN_CB();
    void *res = hm_set(self->hm, key, val);
    if (CB_FAILED() || res == HM_ERROR)
    {
        Py_DECREF(key);
        Py_DECREF(val);
        if (!PyErr_Occurred())
            PyErr_NoMemory();
        return NULL;
    }
    if (res == HM_NEW_KEY)
        Py_RETURN_NONE;

    Py_DECREF(key);
    return (PyObject *)res;
}

static int hm_pop_internal(PyHashMap *self, PyObject *key, PyObject **out)
{
    void *ok = NULL, *ov = NULL;
    BEGIN_CB();
    int r = hm_del_ex(self->hm, key, &ok, &ov);
    if (CB_FAILED())
        return -1;
    if (r == 0)
    {
        *out = NULL;
        return 0;
    }
    Py_DECREF((PyObject *)ok);
    *out = (PyObject *)ov;
    return 0;
}

static PyObject *hm_pop(PyHashMap *self, PyObject *args)
{
    PyObject *key;
    PyObject *def = NULL;
    if (!PyArg_ParseTuple(args, "O|O:pop", &key, &def))
        return NULL;

    PyObject *old = NULL;
    if (hm_pop_internal(self, key, &old) < 0)
        return NULL;

    if (old)
        return old;
    if (def)
    {
        Py_INCREF(def);
        return def;
    }
    PyErr_SetObject(PyExc_KeyError, key);
    return NULL;
}

static PyObject *hm_remove(PyHashMap *self, PyObject *key)
{
    PyObject *old = NULL;
    if (hm_pop_internal(self, key, &old) < 0)
        return NULL;
    if (old)
        return old;
    Py_RETURN_NONE;
}

/* ---------- iterator ---------- */

static PyObject *PyHMIter_iter(PyObject *self)
{
    Py_INCREF(self);
    return self;
}
static PyObject *PyHMIter_next(PyHMIter *self)
{
    void *k = NULL, *v = NULL;
    int r = hm_iter_next(&self->it, &k, &v);
    if (r == -1)
    {
        PyErr_SetString(PyExc_RuntimeError, "HashMap changed size during iteration");
        return NULL;
    }
    if (r == 0)
        return NULL;
    PyObject *key = (PyObject *)k, *val = (PyObject *)v;
    if (self->mode == HM_ITER_KEYS)
    {
        Py_INCREF(key);
        return key;
    }
    if (self->mode == HM_ITER_VALUES)
    {
        Py_INCREF(val);
        return val;
    }
    PyObject *t = PyTuple_New(2);
    if (!t)
        return NULL;
    Py_INCREF(key);
    PyTuple_SET_ITEM(t, 0, key);
    Py_INCREF(val);
    PyTuple_SET_ITEM(t, 1, val);
    return t;
}
static int PyHMIter_traverse(PyHMIter *self, visitproc visit, void *arg)
{
    Py_VISIT(self->map);
    return 0;
}
static int PyHMIter_clear(PyHMIter *self)
{
    Py_CLEAR(self->map);
    return 0;
}
static void PyHMIter_dealloc(PyHMIter *self)
{
    PyObject_GC_UnTrack(self);
    PyHMIter_clear(self);
    Py_TYPE(self)->tp_free((PyObject *)self);
}

PyTypeObject PyHMIterType = {
    PyVarObject_HEAD_INIT(NULL, 0)
        .tp_name = "juniper.HashMapIterator",
    .tp_basicsize = sizeof(PyHMIter),
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HAVE_GC,
    .tp_dealloc = (destructor)PyHMIter_dealloc,
    .tp_traverse = (traverseproc)PyHMIter_traverse,
    .tp_clear = (inquiry)PyHMIter_clear,
    .tp_iter = PyHMIter_iter,
    .tp_iternext = (iternextfunc)PyHMIter_next,
};

static PyObject *hm_make_iter(PyHashMap *self, hm_iter_mode mode)
{
    PyHMIter *it = PyObject_GC_New(PyHMIter, &PyHMIterType);
    if (!it)
        return NULL;
    hm_iter_init(&it->it, self->hm);
    Py_INCREF(self);
    it->map = self;
    it->mode = mode;
    PyObject_GC_Track(it);
    return (PyObject *)it;
}
static PyObject *hm_iter_py(PyHashMap *self) { return hm_make_iter(self, HM_ITER_KEYS); }
static PyObject *hm_keys(PyHashMap *self, PyObject *u) { return hm_make_iter(self, HM_ITER_KEYS); }
static PyObject *hm_values(PyHashMap *self, PyObject *u) { return hm_make_iter(self, HM_ITER_VALUES); }
static PyObject *hm_items(PyHashMap *self, PyObject *u) { return hm_make_iter(self, HM_ITER_ITEMS); }

static PyMethodDef hashmap_methods[] = {
    {"keys", (PyCFunction)hm_keys, METH_NOARGS, "Return keys iterator"},
    {"values", (PyCFunction)hm_values, METH_NOARGS, "Return values iterator"},
    {"items", (PyCFunction)hm_items, METH_NOARGS, "Return items iterator"},
    {"get", (PyCFunction)hm_get_method, METH_VARARGS, "get(key, default=None)"},
    {"pop", (PyCFunction)hm_pop, METH_VARARGS, "pop(key[, default])"},
    {"put", (PyCFunction)hm_put, METH_VARARGS, "put(key, value) -> old value or None"},
    {"remove", (PyCFunction)hm_remove, METH_O, "remove(key) -> old value or None"},
    {NULL}};

static PyMappingMethods map_methods = {
    (lenfunc)hm_len_py,
    (binaryfunc)hm_subscript,
    (objobjargproc)hm_ass_subscript,
};

static PySequenceMethods seq_methods = {
    .sq_contains = (objobjproc)hm_contains,
};

PyTypeObject HashMapType = {
    PyVarObject_HEAD_INIT(NULL, 0)
        .tp_name = "juniper.HashMap",
    .tp_basicsize = sizeof(PyHashMap),
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE | Py_TPFLAGS_HAVE_GC,
    .tp_new = hm_new_py,
    .tp_dealloc = (destructor)hm_dealloc,
    .tp_traverse = (traverseproc)hm_traverse,
    .tp_clear = (inquiry)hm_clear,
    .tp_as_mapping = &map_methods,
    .tp_as_sequence = &seq_methods,
    .tp_iter = (getiterfunc)hm_iter_py,
    .tp_methods = hashmap_methods,
};