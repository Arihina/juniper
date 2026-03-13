#include <Python.h>
#include "slist.h"
#include "hashmap.h"

void hashmap_capsule_destructor(PyObject *capsule)
{

    HashMap *map = PyCapsule_GetPointer(capsule, "HashMap");

    if (map)
        hashmap_free(map);
}

static PyObject *py_create(PyObject *self, PyObject *args)
{
    SList *list = slist_create();
    return PyCapsule_New(list, "SList", NULL);
}

static PyObject *py_push_front(PyObject *self, PyObject *args)
{
    PyObject *capsule;
    int value;

    if (!PyArg_ParseTuple(args, "Oi", &capsule, &value))
        return NULL;

    SList *list = PyCapsule_GetPointer(capsule, "SList");
    slist_push_front(list, value);

    Py_RETURN_NONE;
}

static PyObject *py_pop_front(PyObject *self, PyObject *args)
{
    PyObject *capsule;

    if (!PyArg_ParseTuple(args, "O", &capsule))
        return NULL;

    SList *list = PyCapsule_GetPointer(capsule, "SList");
    int value = slist_pop_front(list);

    return PyLong_FromLong(value);
}

static PyObject *py_hashmap_create(PyObject *self, PyObject *args)
{

    HashMap *map = hashmap_create(8);

    return PyCapsule_New(map, "HashMap", hashmap_capsule_destructor);
}

static PyObject *py_hashmap_put(PyObject *self, PyObject *args)
{

    PyObject *capsule;
    PyObject *key;
    PyObject *value;

    if (!PyArg_ParseTuple(args, "OOO", &capsule, &key, &value))
        return NULL;

    HashMap *map = PyCapsule_GetPointer(capsule, "HashMap");

    hashmap_put(map, key, value);

    Py_RETURN_NONE;
}

static PyObject *py_hashmap_get(PyObject *self, PyObject *args)
{

    PyObject *capsule;
    PyObject *key;

    if (!PyArg_ParseTuple(args, "OO", &capsule, &key))
        return NULL;

    HashMap *map = PyCapsule_GetPointer(capsule, "HashMap");

    return hashmap_get(map, key);
}

static PyObject *py_hashmap_remove(PyObject *self, PyObject *args)
{

    PyObject *capsule;
    PyObject *key;

    if (!PyArg_ParseTuple(args, "OO", &capsule, &key))
        return NULL;

    HashMap *map = PyCapsule_GetPointer(capsule, "HashMap");

    hashmap_remove(map, key);

    Py_RETURN_NONE;
}

static PyMethodDef methods[] = {
    {"create", py_create, METH_NOARGS, "Create list"},
    {"push_front", py_push_front, METH_VARARGS, "Push value"},
    {"pop_front", py_pop_front, METH_VARARGS, "Pop value"},
    {"hashmap_create", py_hashmap_create, METH_NOARGS, "Create hashmap"},
    {"hashmap_put", py_hashmap_put, METH_VARARGS, "Insert"},
    {"hashmap_get", py_hashmap_get, METH_VARARGS, "Get"},
    {"hashmap_remove", py_hashmap_remove, METH_VARARGS, "Remove"},
    {NULL, NULL, 0, NULL}};

static struct PyModuleDef module = {
    PyModuleDef_HEAD_INIT,
    "_native",
    "Native C structures",
    -1,
    methods};

PyMODINIT_FUNC PyInit__native(void)
{
    return PyModule_Create(&module);
}
