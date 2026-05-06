#include <Python.h>
#include "swisstable.h"

typedef struct
{
    PyObject_HEAD SwissTable *map;
} PySwissTableObject;

typedef enum
{
    SWI_KEYS,
    SWI_VALUES,
    SWI_ITEMS
} SWIterKind;

typedef struct
{
    PyObject_HEAD PySwissTableObject *owner;
    size_t pos;
    size_t snap_size;
    SWIterKind kind;
} PySwissTableIterObject;

static PyTypeObject PySwissTableIterType;

static void PySwissTableIter_dealloc(PySwissTableIterObject *it)
{
    Py_XDECREF(it->owner);
    Py_TYPE(it)->tp_free((PyObject *)it);
}

static PyObject *PySwissTableIter_next(PySwissTableIterObject *it)
{
    if (!it->owner || !it->owner->map)
        return NULL;

    SwissTable *map = it->owner->map;

    if (map->size != it->snap_size)
    {
        PyErr_SetString(PyExc_RuntimeError,
                        "SwissTable changed size during iteration");
        return NULL;
    }

    while (it->pos < map->capacity)
    {
        uint8_t c = map->ctrl[it->pos];
        size_t idx = it->pos++;
        if (c != SW_EMPTY && c != SW_DELETED)
        {
            SWSlot *s = &map->slots[idx];
            switch (it->kind)
            {
            case SWI_KEYS:
                Py_INCREF(s->key);
                return s->key;
            case SWI_VALUES:
                Py_INCREF(s->value);
                return s->value;
            case SWI_ITEMS:
            {
                PyObject *t = PyTuple_New(2);
                if (!t)
                    return NULL;
                Py_INCREF(s->key);
                Py_INCREF(s->value);
                PyTuple_SET_ITEM(t, 0, s->key);
                PyTuple_SET_ITEM(t, 1, s->value);
                return t;
            }
            }
        }
    }
    return NULL;
}

static PyObject *PySwissTableIter_iter(PyObject *self)
{
    Py_INCREF(self);
    return self;
}

static PyTypeObject PySwissTableIterType = {
    PyVarObject_HEAD_INIT(NULL, 0)
        .tp_name = "juniper._native.SwissTableIterator",
    .tp_basicsize = sizeof(PySwissTableIterObject),
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_dealloc = (destructor)PySwissTableIter_dealloc,
    .tp_iter = PySwissTableIter_iter,
    .tp_iternext = (iternextfunc)PySwissTableIter_next,
};

static PyObject *make_iter(PySwissTableObject *self, SWIterKind kind)
{
    if (PyType_Ready(&PySwissTableIterType) < 0)
        return NULL;
    PySwissTableIterObject *it = PyObject_New(PySwissTableIterObject,
                                              &PySwissTableIterType);
    if (!it)
        return NULL;
    Py_INCREF(self);
    it->owner = self;
    it->pos = 0;
    it->snap_size = self->map->size;
    it->kind = kind;
    return (PyObject *)it;
}

static PyObject *PySwissTable_new(PyTypeObject *type,
                                  PyObject *args, PyObject *kwds)
{
    PySwissTableObject *self;
    PyObject *arg = NULL;
    size_t capacity = 16;

    if (!PyArg_ParseTuple(args, "|O", &arg))
        return NULL;

    if (arg && PyLong_Check(arg))
    {
        capacity = (size_t)PyLong_AsSize_t(arg);
        if (capacity == (size_t)-1 && PyErr_Occurred())
            return NULL;
        arg = NULL;
    }

    self = (PySwissTableObject *)type->tp_alloc(type, 0);
    if (!self)
        return NULL;

    self->map = sw_create(capacity);
    if (!self->map)
    {
        Py_DECREF(self);
        return PyErr_NoMemory();
    }

    if (arg && PyDict_Check(arg))
    {
        PyObject *k, *v;
        Py_ssize_t pos = 0;
        while (PyDict_Next(arg, &pos, &k, &v))
        {
            if (sw_put(self->map, k, v) < 0)
            {
                Py_DECREF(self);
                return NULL;
            }
        }
    }
    else if (arg)
    {
        PyObject *iter = PyObject_GetIter(arg);
        if (!iter)
        {
            Py_DECREF(self);
            return NULL;
        }
        PyObject *item;
        while ((item = PyIter_Next(iter)))
        {
            PyObject *k, *v;
            if (!PyArg_ParseTuple(item, "OO", &k, &v))
            {
                Py_DECREF(item);
                Py_DECREF(iter);
                Py_DECREF(self);
                return NULL;
            }
            Py_DECREF(item);
            if (sw_put(self->map, k, v) < 0)
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

    if (kwds && PyDict_Size(kwds) > 0)
    {
        PyObject *k, *v;
        Py_ssize_t pos = 0;
        while (PyDict_Next(kwds, &pos, &k, &v))
        {
            if (sw_put(self->map, k, v) < 0)
            {
                Py_DECREF(self);
                return NULL;
            }
        }
    }

    return (PyObject *)self;
}

static void PySwissTable_dealloc(PySwissTableObject *self)
{
    if (self->map)
        sw_free(self->map);
    Py_TYPE(self)->tp_free((PyObject *)self);
}

static Py_ssize_t PySwissTable_len(PySwissTableObject *self)
{
    return (Py_ssize_t)self->map->size;
}

static PyObject *PySwissTable_subscript(PySwissTableObject *self, PyObject *key)
{
    PyObject *val = sw_get(self->map, key);
    if (!val)
    {
        if (!PyErr_Occurred())
            PyErr_SetObject(PyExc_KeyError, key);
        return NULL;
    }
    return val;
}

static int PySwissTable_ass_subscript(PySwissTableObject *self,
                                      PyObject *key, PyObject *value)
{
    if (!value)
    {
        int rc = sw_remove(self->map, key);
        if (rc < 0)
            return -1;
        if (rc == 0)
        {
            PyErr_SetObject(PyExc_KeyError, key);
            return -1;
        }
        return 0;
    }
    int rc = sw_put(self->map, key, value);
    if (rc < 0)
        return -1;
    return 0;
}

static PyMappingMethods PySwissTable_as_mapping = {
    .mp_length = (lenfunc)PySwissTable_len,
    .mp_subscript = (binaryfunc)PySwissTable_subscript,
    .mp_ass_subscript = (objobjargproc)PySwissTable_ass_subscript,
};

static int PySwissTable_sq_contains(PySwissTableObject *self, PyObject *key)
{
    return sw_contains(self->map, key);
}

static PySequenceMethods PySwissTable_as_sequence = {
    .sq_contains = (objobjproc)PySwissTable_sq_contains,
};

static PyObject *PySwissTable_put(PySwissTableObject *self, PyObject *args)
{
    PyObject *key, *value;
    if (!PyArg_ParseTuple(args, "OO", &key, &value))
        return NULL;
    int rc = sw_put(self->map, key, value);
    if (rc < 0)
        return NULL;
    Py_RETURN_NONE;
}

static PyObject *PySwissTable_get_method(PySwissTableObject *self, PyObject *args)
{
    PyObject *key, *defval = Py_None;
    if (!PyArg_ParseTuple(args, "O|O", &key, &defval))
        return NULL;
    PyObject *val = sw_get(self->map, key);
    if (!val)
    {
        if (PyErr_Occurred())
            return NULL;
        Py_INCREF(defval);
        return defval;
    }
    return val;
}

static PyObject *PySwissTable_remove(PySwissTableObject *self, PyObject *args)
{
    PyObject *key;
    if (!PyArg_ParseTuple(args, "O", &key))
        return NULL;
    int rc = sw_remove(self->map, key);
    if (rc < 0)
        return NULL;
    if (rc == 0)
    {
        PyErr_SetObject(PyExc_KeyError, key);
        return NULL;
    }
    Py_RETURN_NONE;
}

static PyObject *PySwissTable_contains_method(PySwissTableObject *self, PyObject *args)
{
    PyObject *key;
    if (!PyArg_ParseTuple(args, "O", &key))
        return NULL;
    int rc = sw_contains(self->map, key);
    if (rc < 0)
        return NULL;
    return PyBool_FromLong(rc);
}

static PyObject *PySwissTable_clear(PySwissTableObject *self,
                                    PyObject *Py_UNUSED(ignored))
{
    sw_clear(self->map);
    Py_RETURN_NONE;
}

static PyObject *PySwissTable_keys(PySwissTableObject *self,
                                   PyObject *Py_UNUSED(ignored))
{
    return make_iter(self, SWI_KEYS);
}

static PyObject *PySwissTable_values(PySwissTableObject *self,
                                     PyObject *Py_UNUSED(ignored))
{
    return make_iter(self, SWI_VALUES);
}

static PyObject *PySwissTable_items(PySwissTableObject *self,
                                    PyObject *Py_UNUSED(ignored))
{
    return make_iter(self, SWI_ITEMS);
}

static PyObject *PySwissTable_pop(PySwissTableObject *self, PyObject *args)
{
    PyObject *key, *defval = NULL;
    if (!PyArg_ParseTuple(args, "O|O", &key, &defval))
        return NULL;
    PyObject *val = sw_get(self->map, key);
    if (val)
    {
        sw_remove(self->map, key);
        return val;
    }
    if (PyErr_Occurred())
        return NULL;
    if (defval)
    {
        Py_INCREF(defval);
        return defval;
    }
    PyErr_SetObject(PyExc_KeyError, key);
    return NULL;
}

static PyMethodDef PySwissTable_methods[] = {
    {"put", (PyCFunction)PySwissTable_put, METH_VARARGS,
     "Insert or update key/value pair."},
    {"get", (PyCFunction)PySwissTable_get_method, METH_VARARGS,
     "Get value by key with optional default."},
    {"remove", (PyCFunction)PySwissTable_remove, METH_VARARGS,
     "Remove key; raise KeyError if missing."},
    {"contains", (PyCFunction)PySwissTable_contains_method, METH_VARARGS,
     "Return True if key exists."},
    {"clear", (PyCFunction)PySwissTable_clear, METH_NOARGS,
     "Remove all entries."},
    {"keys", (PyCFunction)PySwissTable_keys, METH_NOARGS,
     "Return an iterator over keys."},
    {"values", (PyCFunction)PySwissTable_values, METH_NOARGS,
     "Return an iterator over values."},
    {"items", (PyCFunction)PySwissTable_items, METH_NOARGS,
     "Return an iterator over (key, value) pairs."},
    {"pop", (PyCFunction)PySwissTable_pop, METH_VARARGS,
     "Remove key and return value; return default if given, else KeyError."},
    {NULL}};

static PyObject *PySwissTable_iter(PySwissTableObject *self)
{
    return make_iter(self, SWI_KEYS);
}

static PyObject *PySwissTable_repr(PySwissTableObject *self)
{
    if (self->map->size == 0)
        return PyUnicode_FromString("SwissTable({})");

    PyObject *parts = PyList_New(0);
    if (!parts)
        return NULL;

    for (size_t i = 0; i < self->map->capacity; i++)
    {
        if (self->map->ctrl[i] == SW_EMPTY ||
            self->map->ctrl[i] == SW_DELETED)
            continue;

        SWSlot *s = &self->map->slots[i];
        PyObject *kr = PyObject_Repr(s->key);
        if (!kr)
        {
            Py_DECREF(parts);
            return NULL;
        }
        PyObject *vr = PyObject_Repr(s->value);
        if (!vr)
        {
            Py_DECREF(kr);
            Py_DECREF(parts);
            return NULL;
        }

        PyObject *pair = PyUnicode_FromFormat("%U: %U", kr, vr);
        Py_DECREF(kr);
        Py_DECREF(vr);
        if (!pair)
        {
            Py_DECREF(parts);
            return NULL;
        }
        if (PyList_Append(parts, pair) < 0)
        {
            Py_DECREF(pair);
            Py_DECREF(parts);
            return NULL;
        }
        Py_DECREF(pair);
    }

    PyObject *sep = PyUnicode_FromString(", ");
    PyObject *joined = PyUnicode_Join(sep, parts);
    Py_DECREF(sep);
    Py_DECREF(parts);
    if (!joined)
        return NULL;

    PyObject *result = PyUnicode_FromFormat("SwissTable({%U})", joined);
    Py_DECREF(joined);
    return result;
}

static PyObject *PySwissTable_richcmp(PyObject *self, PyObject *other, int op)
{
    if (op != Py_EQ && op != Py_NE)
        Py_RETURN_NOTIMPLEMENTED;
    if (!PyObject_TypeCheck(other, Py_TYPE(self)))
        Py_RETURN_NOTIMPLEMENTED;

    PySwissTableObject *a = (PySwissTableObject *)self;
    PySwissTableObject *b = (PySwissTableObject *)other;

    if (a->map->size != b->map->size)
    {
        if (op == Py_EQ)
            Py_RETURN_FALSE;
        Py_RETURN_TRUE;
    }

    for (size_t i = 0; i < a->map->capacity; i++)
    {
        if (a->map->ctrl[i] == SW_EMPTY || a->map->ctrl[i] == SW_DELETED)
            continue;
        SWSlot *sa = &a->map->slots[i];
        PyObject *vb = sw_get(b->map, sa->key);
        if (!vb)
        {
            if (PyErr_Occurred())
                return NULL;
            if (op == Py_EQ)
                Py_RETURN_FALSE;
            Py_RETURN_TRUE;
        }
        int eq = PyObject_RichCompareBool(sa->value, vb, Py_EQ);
        Py_DECREF(vb);
        if (eq < 0)
            return NULL;
        if (!eq)
        {
            if (op == Py_EQ)
                Py_RETURN_FALSE;
            Py_RETURN_TRUE;
        }
    }

    if (op == Py_EQ)
        Py_RETURN_TRUE;
    Py_RETURN_FALSE;
}

PyTypeObject PySwissTableType = {
    PyVarObject_HEAD_INIT(NULL, 0)
        .tp_name = "juniper._native.SwissTable",
    .tp_basicsize = sizeof(PySwissTableObject),
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    .tp_new = PySwissTable_new,
    .tp_dealloc = (destructor)PySwissTable_dealloc,
    .tp_methods = PySwissTable_methods,
    .tp_as_mapping = &PySwissTable_as_mapping,
    .tp_as_sequence = &PySwissTable_as_sequence,
    .tp_iter = (getiterfunc)PySwissTable_iter,
    .tp_repr = (reprfunc)PySwissTable_repr,
    .tp_richcompare = PySwissTable_richcmp,
    .tp_doc = "Swiss Table hash map (open addressing with control bytes).",
};