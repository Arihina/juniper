#ifndef DLIST_H
#define DLIST_H

#include <Python.h>
#include <stddef.h>

typedef struct DListNode {
    PyObject *value;
    struct DListNode *prev;
    struct DListNode *next;
} DListNode;

typedef struct {
    DListNode *head;
    DListNode *tail;
    size_t size;
} DList;

DList *dlist_create(void);
void dlist_free(DList *list);

int dlist_push_front(DList *list, PyObject *value);
int dlist_push_back(DList *list, PyObject *value);
int dlist_insert(DList *list, size_t index, PyObject *value);

PyObject *dlist_pop_front(DList *list);
PyObject *dlist_pop_back(DList *list);
int dlist_remove(DList *list, PyObject *value);

PyObject *dlist_get(DList *list, size_t index);

int dlist_contains(DList *list, PyObject *value);
Py_ssize_t dlist_index(DList *list, PyObject *value);

void dlist_clear(DList *list);
void dlist_reverse(DList *list);
void dlist_move_to_front(DList *list, DListNode *node);

static inline int dlist_is_empty(DList *list) { return list->size == 0; }
static inline size_t dlist_len(DList *list) { return list->size; }

#endif