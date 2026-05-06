#ifndef SLIST_H
#define SLIST_H

#include <Python.h>
#include <stddef.h>

typedef struct SListNode
{
    PyObject *value;
    struct SListNode *next;
} SListNode;

typedef struct
{
    SListNode *head;
    SListNode *tail;
    size_t size;
} SList;

SList *slist_create(void);
void slist_free(SList *list);

int slist_push_front(SList *list, PyObject *value);
int slist_push_back(SList *list, PyObject *value);
int slist_insert(SList *list, size_t index, PyObject *value);

PyObject *slist_pop_front(SList *list);
PyObject *slist_pop_back(SList *list);
int slist_remove(SList *list, PyObject *value);

PyObject *slist_get(SList *list, size_t index);

int slist_contains(SList *list, PyObject *value);
Py_ssize_t slist_index(SList *list, PyObject *value);

void slist_clear(SList *list);
void slist_reverse(SList *list);

static inline int slist_is_empty(SList *list) { return list->size == 0; }
static inline size_t slist_len(SList *list) { return list->size; }

#endif