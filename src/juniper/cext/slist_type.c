#include <Python.h>
#include "slist.h"

typedef struct
{
    PyObject_HEAD SList *list;
} PySListObject;

static PyObject *PySList_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    PySListObject *self;
    self = (PySListObject *)type->tp_alloc(type, 0);
    if (!self)
        return NULL;

    self->list = slist_create();
    if (!self->list)
    {
        Py_DECREF(self);
        return PyErr_NoMemory();
    }

    return (PyObject *)self;
}

static void PySList_dealloc(PySListObject *self)
{
    if (self->list)
        slist_free(self->list);

    Py_TYPE(self)->tp_free((PyObject *)self);
}

static PyObject *PySList_push_front(PySListObject *self, PyObject *args)
{
    int value;
    if (!PyArg_ParseTuple(args, "i", &value))
        return NULL;

    slist_push_front(self->list, value);
    Py_RETURN_NONE;
}

static PyObject *PySList_pop_front(PySListObject *self, PyObject *Py_UNUSED(ignored))
{
    int value = slist_pop_front(self->list);
    return PyLong_FromLong(value);
}

static PyMethodDef PySList_methods[] = {
    {"push_front", (PyCFunction)PySList_push_front, METH_VARARGS, "Push value"},
    {"pop_front", (PyCFunction)PySList_pop_front, METH_NOARGS, "Pop value"},
    {NULL}};

PyTypeObject PySListType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "juniper._native.SList",
    .tp_basicsize = sizeof(PySListObject),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    .tp_new = PySList_new,
    .tp_dealloc = (destructor)PySList_dealloc,
    .tp_methods = PySList_methods,
};
