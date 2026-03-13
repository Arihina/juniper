#ifndef HASHMAP_H
#define HASHMAP_H

#include <Python.h>
#include <stddef.h>

typedef struct HashNode
{
    PyObject *key;
    PyObject *value;
    struct HashNode *next;
} HashNode;

typedef struct
{
    HashNode **buckets;
    size_t capacity;
    size_t size;
} HashMap;

HashMap *hashmap_create(size_t capacity);

void hashmap_put(HashMap *map, PyObject *key, PyObject *value);

PyObject *hashmap_get(HashMap *map, PyObject *key);

int hashmap_remove(HashMap *map, PyObject *key);

void hashmap_resize(HashMap *map, size_t new_capacity);

void hashmap_free(HashMap *map);

#endif