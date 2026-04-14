#ifndef BTREEMAP_H
#define BTREEMAP_H

#include <Python.h>
#include <stddef.h>

/*
 * B-tree map: each node stores parallel arrays of keys[] and values[].
 * Used as a bucket in BTreeHashMap.
 * Small default order (8) since each bucket is expected to be small.
 */

#define BTM_DEFAULT_ORDER 8

typedef struct BTMNode {
    PyObject         **keys;
    PyObject         **values;
    struct BTMNode   **children;
    int                n;
    int                leaf;
} BTMNode;

typedef struct {
    BTMNode *root;
    size_t   size;
    int      order;
} BTreeMap;

BTreeMap *btm_create(int order);

void btm_free(BTreeMap *map);
int btm_put(BTreeMap *map, PyObject *key, PyObject *value);

PyObject *btm_get(BTreeMap *map, PyObject *key);

int btm_remove(BTreeMap *map, PyObject *key);
int btm_contains(BTreeMap *map, PyObject *key);

typedef struct {
    BTMNode *node;
    int idx;
} BTMPos;

BTMPos btm_first(BTreeMap *map);
BTMPos btm_next(BTreeMap *map, BTMPos pos);

#endif