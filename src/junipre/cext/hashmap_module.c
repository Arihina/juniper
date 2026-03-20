#include <Python.h>
#include "hashmap.h"

void hashmap_capsule_destructor(PyObject *capsule)
{
    HashMap *map = PyCapsule_GetPointer(capsule, "HashMap");
    if (map)
        hashmap_free(map);
}

PyObject *py_hashmap_create(PyObject *self, PyObject *args)
{
    HashMap *map = hashmap_create(8);
    if (!map)
        return PyErr_NoMemory();

    return PyCapsule_New(map, "HashMap", hashmap_capsule_destructor);
}

PyObject *py_hashmap_put(PyObject *self, PyObject *args)
{
    PyObject *capsule, *key, *value;

    if (!PyArg_ParseTuple(args, "OOO", &capsule, &key, &value))
        return NULL;

    HashMap *map = PyCapsule_GetPointer(capsule, "HashMap");
    if (!map)
        return NULL;

    hashmap_put(map, key, value);
    if (PyErr_Occurred())
        return NULL;

    Py_RETURN_NONE;
}

PyObject *py_hashmap_get(PyObject *self, PyObject *args)
{
    PyObject *capsule, *key;

    if (!PyArg_ParseTuple(args, "OO", &capsule, &key))
        return NULL;

    HashMap *map = PyCapsule_GetPointer(capsule, "HashMap");
    if (!map)
        return NULL;

    PyObject *result = hashmap_get(map, key);
    if (!result)
        return NULL;

    return result;
}

PyObject *py_hashmap_remove(PyObject *self, PyObject *args)
{
    PyObject *capsule, *key;

    if (!PyArg_ParseTuple(args, "OO", &capsule, &key))
        return NULL;

    HashMap *map = PyCapsule_GetPointer(capsule, "HashMap");
    if (!map)
        return NULL;

    int res = hashmap_remove(map, key);
    if (res < 0)
        Py_RETURN_NONE;

    Py_RETURN_NONE;
}