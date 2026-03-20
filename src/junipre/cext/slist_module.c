#include <Python.h>
#include "slist.h"

// destructor
void slist_capsule_destructor(PyObject *capsule)
{
    SList *list = PyCapsule_GetPointer(capsule, "SList");
    if (list)
        slist_free(list);
}

PyObject *py_create(PyObject *self, PyObject *args)
{
    SList *list = slist_create();
    if (!list)
        return PyErr_NoMemory();

    return PyCapsule_New(list, "SList", slist_capsule_destructor);
}

PyObject *py_push_front(PyObject *self, PyObject *args)
{
    PyObject *capsule;
    int value;

    if (!PyArg_ParseTuple(args, "Oi", &capsule, &value))
        return NULL;

    SList *list = PyCapsule_GetPointer(capsule, "SList");
    if (!list)
        return NULL;

    slist_push_front(list, value);

    Py_RETURN_NONE;
}

PyObject *py_pop_front(PyObject *self, PyObject *args)
{
    PyObject *capsule;

    if (!PyArg_ParseTuple(args, "O", &capsule))
        return NULL;

    SList *list = PyCapsule_GetPointer(capsule, "SList");
    if (!list)
        return NULL;

    if (!list->head) {
        PyErr_SetString(PyExc_IndexError, "pop from empty list");
        return NULL;
    }

    int value = slist_pop_front(list);
    return PyLong_FromLong(value);
}