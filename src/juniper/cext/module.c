#include <Python.h>
#include "slist_type.h"
#include "hashmap_type.h"
#include "rbset_type.h"

static PyModuleDef juniper_module = {
    PyModuleDef_HEAD_INIT,
    "_native",
    "Juniper native C extension",
    -1,
    NULL,
};

PyMODINIT_FUNC PyInit__native(void)
{
    PyObject *m;
 
    m = PyModule_Create(&juniper_module);
    if (!m)
        return NULL;
 
    if (PyType_Ready(&PySListType) < 0)
        return NULL;
 
    Py_INCREF(&PySListType);
    if (PyModule_AddObject(m, "SList", (PyObject *)&PySListType) < 0) {
        Py_DECREF(&PySListType);
        Py_DECREF(m);
        return NULL;
    }
 
    if (PyType_Ready(&PyHashMapType) < 0)
        return NULL;
 
    Py_INCREF(&PyHashMapType);
    if (PyModule_AddObject(m, "HashMap", (PyObject *)&PyHashMapType) < 0) {
        Py_DECREF(&PyHashMapType);
        Py_DECREF(m);
        return NULL;
    }
 
    if (PyType_Ready(&PyRBSetType) < 0)
        return NULL;
 
    Py_INCREF(&PyRBSetType);
    if (PyModule_AddObject(m, "RBSet", (PyObject *)&PyRBSetType) < 0) {
        Py_DECREF(&PyRBSetType);
        Py_DECREF(m);
        return NULL;
    }
 
    return m;
}