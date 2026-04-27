#ifndef SKIPLIST_H
#define SKIPLIST_H

#include <Python.h>
#include <stddef.h>

#define SKIPLIST_MAX_LEVEL 32
#define SKIPLIST_P 0.25

typedef struct SkipNode {
    PyObject *key;
    int level;
    struct SkipNode **forward;
} SkipNode;

typedef struct {
    SkipNode *header;
    int level;
    size_t size;
} SkipList;

SkipList *skiplist_create(void);

void skiplist_free(SkipList *list);
int skiplist_insert(SkipList *list, PyObject *key);
int skiplist_remove(SkipList *list, PyObject *key);
int skiplist_contains(SkipList *list, PyObject *key);

SkipNode *skiplist_first(SkipList *list);
SkipNode *skiplist_next(SkipNode *node);

PyObject *skiplist_peek_min(SkipList *list);

int skiplist_height(SkipList *list);

static inline int skiplist_is_empty(SkipList *list) { return list->size == 0; }
static inline size_t skiplist_len(SkipList *list) { return list->size; }

#endif