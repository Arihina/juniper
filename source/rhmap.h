#ifndef RHMAP_H
#define RHMAP_H

#include <Python.h>
#include <stddef.h>

/*
 * Robin Hood hash map — open addressing with linear probing.
 *
 * Each slot stores key, value, cached hash, and PSL (Probe Sequence Length).
 * On insert, a "rich" entry (low PSL) gives its slot to a "poor" entry
 * (high PSL), keeping maximum PSL small and lookups fast.
 *
 * Load factor is kept below 0.75; table size is always a power of two.
 */

#define RHMAP_EMPTY 0
#define RHMAP_USED 1

typedef struct
{
    PyObject *key;
    PyObject *value;
    size_t hash;
    size_t psl;
    int state;
} RHSlot;

typedef struct
{
    RHSlot *slots;
    size_t capacity;
    size_t size;
} RHMap;

RHMap *rhmap_create(size_t capacity);
void rhmap_free(RHMap *map);

int rhmap_put(RHMap *map, PyObject *key, PyObject *value);

PyObject *rhmap_get(RHMap *map, PyObject *key);

int rhmap_remove(RHMap *map, PyObject *key);

int rhmap_contains(RHMap *map, PyObject *key);

void rhmap_clear(RHMap *map);

#endif
