#include <Python.h>
#include "slist.h"

static PyObject* py_create(PyObject* self, PyObject* args) {
    SList* list = slist_create();
    return PyCapsule_New(list, "SList", NULL);
}

static PyObject* py_push_front(PyObject* self, PyObject* args) {
    PyObject* capsule;
    int value;

    if (!PyArg_ParseTuple(args, "Oi", &capsule, &value))
        return NULL;

    SList* list = PyCapsule_GetPointer(capsule, "SList");
    slist_push_front(list, value);

    Py_RETURN_NONE;
}

static PyObject* py_pop_front(PyObject* self, PyObject* args) {
    PyObject* capsule;

    if (!PyArg_ParseTuple(args, "O", &capsule))
        return NULL;

    SList* list = PyCapsule_GetPointer(capsule, "SList");
    int value = slist_pop_front(list);

    return PyLong_FromLong(value);
}

static PyMethodDef methods[] = {
    {"create", py_create, METH_NOARGS, "Create list"},
    {"push_front", py_push_front, METH_VARARGS, "Push value"},
    {"pop_front", py_pop_front, METH_VARARGS, "Pop value"},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef module = {
    PyModuleDef_HEAD_INIT,
    "_native",
    "Native C structures",
    -1,
    methods
};

PyMODINIT_FUNC PyInit__native(void) {
    return PyModule_Create(&module);
}
