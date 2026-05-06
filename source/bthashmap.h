#ifndef BTHASHMAP_H
#define BTHASHMAP_H

#include <Python.h>
#include <stddef.h>
#include "btreemap.h"

/*
 * Hash map where each bucket is a B-tree map.
 *
 * Collisions are resolved by inserting into the bucket's B-tree,
 * giving O(log k) worst-case per-bucket (k = keys in bucket)
 * instead of O(k) for chaining with linked lists.
 *
 * The hash table resizes (grow/shrink) to keep the average
 * bucket size bounded.
 */

#define BTHM_DEFAULT_NBUCKETS 16
#define BTHM_BUCKET_ORDER 8

typedef struct
{
    BTreeMap **buckets;
    size_t nbuckets;
    size_t size;
    int bt_order;
} BTHashMap;

BTHashMap *bthm_create(size_t nbuckets, int bt_order);

void bthm_free(BTHashMap *map);
int bthm_put(BTHashMap *map, PyObject *key, PyObject *value);

PyObject *bthm_get(BTHashMap *map, PyObject *key);

int bthm_remove(BTHashMap *map, PyObject *key);
int bthm_contains(BTHashMap *map, PyObject *key);
void bthm_clear(BTHashMap *map);

#endif