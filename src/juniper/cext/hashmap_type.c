#include <Python.h>
#include "hashmap.h"

typedef struct {
    PyObject_HEAD
    HashMap *map;
} PyHashMapObject;

static PyObject *PyHashMap_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    PyHashMapObject *self;
    size_t capacity = 16;  // дефолт

    if (!PyArg_ParseTuple(args, "|n", &capacity))
        return NULL;

    self = (PyHashMapObject *)type->tp_alloc(type, 0);
    if (!self) return NULL;

    self->map = hashmap_create(capacity);
    if (!self->map) {
        Py_DECREF(self);
        return PyErr_NoMemory();
    }

    return (PyObject *)self;
}

static void PyHashMap_dealloc(PyHashMapObject *self)
{
    if (self->map)
        hashmap_free(self->map);

    Py_TYPE(self)->tp_free((PyObject *)self);
}

static PyObject *PyHashMap_put(PyHashMapObject *self, PyObject *args)
{
    PyObject *key, *value;

    if (!PyArg_ParseTuple(args, "OO", &key, &value))
        return NULL;

    hashmap_put(self->map, key, value);
    Py_RETURN_NONE;
}

static PyObject *PyHashMap_get(PyHashMapObject *self, PyObject *args)
{
    PyObject *key;

    if (!PyArg_ParseTuple(args, "O", &key))
        return NULL;

    return hashmap_get(self->map, key);
}

static PyObject *PyHashMap_remove(PyHashMapObject *self, PyObject *args)
{
    PyObject *key;

    if (!PyArg_ParseTuple(args, "O", &key))
        return NULL;

    int result = hashmap_remove(self->map, key);
    return PyLong_FromLong(result);
}

static PyObject *PyHashMap_getitem(PyHashMapObject *self, PyObject *key)
{
    return hashmap_get(self->map, key);
}

static int PyHashMap_setitem(PyHashMapObject *self, PyObject *key, PyObject *value)
{
    if (!value) {
        hashmap_remove(self->map, key);
        return 0;
    }

    hashmap_put(self->map, key, value);
    return 0;
}

static PyMethodDef PyHashMap_methods[] = {
    {"put", (PyCFunction)PyHashMap_put, METH_VARARGS, "Insert key/value"},
    {"get", (PyCFunction)PyHashMap_get, METH_VARARGS, "Get value"},
    {"remove", (PyCFunction)PyHashMap_remove, METH_VARARGS, "Remove key"},
    {NULL}
};

static PyMappingMethods PyHashMap_mapping = {
    .mp_length = NULL,
    .mp_subscript = (binaryfunc)PyHashMap_getitem,
    .mp_ass_subscript = (objobjargproc)PyHashMap_setitem,
};

PyTypeObject PyHashMapType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "juniper._native.HashMap",
    .tp_basicsize = sizeof(PyHashMapObject),
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    .tp_new = PyHashMap_new,
    .tp_dealloc = (destructor)PyHashMap_dealloc,
    .tp_methods = PyHashMap_methods,
    .tp_as_mapping = &PyHashMap_mapping,
};