#include <Python.h>
#include "slist_type.h"
#include "dlist_type.h"
#include "hashmap_type.h"
#include "rhmap_type.h"
#include "bthashmap_type.h"
#include "swisstable_type.h"
#include "rbset_type.h"
#include "bset_type.h"
#include "avlset_type.h"
#include "bstset_type.h"
#include "skiplist_type.h"


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
 
    if (PyType_Ready(&PyDListType) < 0)
        return NULL;
 
    Py_INCREF(&PyDListType);
    if (PyModule_AddObject(m, "DList", (PyObject *)&PyDListType) < 0) {
        Py_DECREF(&PyDListType);
        Py_DECREF(m);
        return NULL;
    }
 
    if (PyType_Ready(&HashMapType) < 0)
        return NULL;

    if (PyType_Ready(&PyHMIterType) < 0)
        return NULL;

    Py_INCREF(&HashMapType);
    if (PyModule_AddObject(m, "HashMap", (PyObject *)&HashMapType) < 0) {
        Py_DECREF(&HashMapType);
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
 
    if (PyType_Ready(&PyRHMapType) < 0)
        return NULL;
 
    Py_INCREF(&PyRHMapType);
    if (PyModule_AddObject(m, "RHMap", (PyObject *)&PyRHMapType) < 0) {
        Py_DECREF(&PyRHMapType);
        Py_DECREF(m);
        return NULL;
    }

    if (PyType_Ready(&PyAVLSetType) < 0)
        return NULL;
 
    Py_INCREF(&PyAVLSetType);
    if (PyModule_AddObject(m, "AVLSet", (PyObject *)&PyAVLSetType) < 0) {
        Py_DECREF(&PyAVLSetType);
        Py_DECREF(m);
        return NULL;
    }
    
    if (PyType_Ready(&PyBTreeSetType) < 0)
        return NULL;
 
    Py_INCREF(&PyBTreeSetType);
    if (PyModule_AddObject(m, "BTreeSet", (PyObject *)&PyBTreeSetType) < 0) {
        Py_DECREF(&PyBTreeSetType);
        Py_DECREF(m);
        return NULL;
    }

    if (PyType_Ready(&PyBTHashMapType) < 0)
        return NULL;
 
    Py_INCREF(&PyBTHashMapType);
    if (PyModule_AddObject(m, "BTHashMap", (PyObject *)&PyBTHashMapType) < 0) {
        Py_DECREF(&PyBTHashMapType);
        Py_DECREF(m);
        return NULL;
    }

    if (PyType_Ready(&PySwissTableType) < 0)
        return NULL;
 
    Py_INCREF(&PySwissTableType);
    if (PyModule_AddObject(m, "SwissTable", (PyObject *)&PySwissTableType) < 0) {
        Py_DECREF(&PySwissTableType);
        Py_DECREF(m);
        return NULL;
    }

    if (PyType_Ready(&PyBSTSetType) < 0)
        return NULL;
 
    Py_INCREF(&PyBSTSetType);
    if (PyModule_AddObject(m, "BSTSet", (PyObject *)&PyBSTSetType) < 0) {
        Py_DECREF(&PyBSTSetType);
        Py_DECREF(m);
        return NULL;
    }
 
    if (PyType_Ready(&PySkipListSetType) < 0)
        return NULL;
 
    Py_INCREF(&PySkipListSetType);
    if (PyModule_AddObject(m, "SkipListSet", (PyObject *)&PySkipListSetType) < 0) {
        Py_DECREF(&PySkipListSetType);
        Py_DECREF(m);
        return NULL;
    }

    return m;
}