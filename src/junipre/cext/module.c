#include <Python.h>

// SList
PyObject* py_create(PyObject*, PyObject*);
PyObject* py_push_front(PyObject*, PyObject*);
PyObject* py_pop_front(PyObject*, PyObject*);

// HashMap
PyObject* py_hashmap_create(PyObject*, PyObject*);
PyObject* py_hashmap_put(PyObject*, PyObject*);
PyObject* py_hashmap_get(PyObject*, PyObject*);
PyObject* py_hashmap_remove(PyObject*, PyObject*);

static PyMethodDef methods[] = {
    // SList
    {"create", py_create, METH_NOARGS, "Create list"},
    {"push_front", py_push_front, METH_VARARGS, "Push value"},
    {"pop_front", py_pop_front, METH_VARARGS, "Pop value"},

    // HashMap
    {"hashmap_create", py_hashmap_create, METH_NOARGS, "Create hashmap"},
    {"hashmap_put", py_hashmap_put, METH_VARARGS, "Insert"},
    {"hashmap_get", py_hashmap_get, METH_VARARGS, "Get"},
    {"hashmap_remove", py_hashmap_remove, METH_VARARGS, "Remove"},

    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef module = {
    PyModuleDef_HEAD_INIT,
    "_native",
    "Native C structures",
    -1,
    methods
};

PyMODINIT_FUNC PyInit__native(void)
{
    return PyModule_Create(&module);
}