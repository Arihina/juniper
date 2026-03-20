#include "hashmap.h"
#include <stdlib.h>

static size_t hash_key(PyObject *key)
{
    Py_hash_t h = PyObject_Hash(key);
    if (h == -1 && PyErr_Occurred())
        return (size_t)-1;
    return (size_t)h;
}

HashMap *hashmap_create(size_t capacity)
{
    HashMap *map = malloc(sizeof(HashMap));
    if (!map)
        return NULL;

    map->capacity = capacity;
    map->size = 0;

    map->buckets = calloc(capacity, sizeof(HashNode *));
    if (!map->buckets) {
        free(map);
        return NULL;
    }

    return map;
}

static HashNode *node_create(PyObject *key, PyObject *value)
{
    HashNode *node = malloc(sizeof(HashNode));
    if (!node)
        return NULL;

    Py_INCREF(key);
    Py_INCREF(value);

    node->key = key;
    node->value = value;
    node->next = NULL;

    return node;
}

void hashmap_put(HashMap *map, PyObject *key, PyObject *value)
{
    size_t hash = hash_key(key);
    if (hash == (size_t)-1)
        return;

    size_t index = hash % map->capacity;

    HashNode *node = map->buckets[index];

    while (node)
    {
        int cmp = PyObject_RichCompareBool(node->key, key, Py_EQ);
        if (cmp < 0)
            return;

        if (cmp == 1)
        {
            Py_DECREF(node->value);
            Py_INCREF(value);
            node->value = value;
            return;
        }

        node = node->next;
    }

    HashNode *new_node = node_create(key, value);
    if (!new_node)
        return;

    new_node->next = map->buckets[index];
    map->buckets[index] = new_node;

    map->size++;
}

PyObject *hashmap_get(HashMap *map, PyObject *key)
{
    size_t hash = hash_key(key);
    if (hash == (size_t)-1)
        return NULL;

    size_t index = hash % map->capacity;

    HashNode *node = map->buckets[index];

    while (node)
    {
        int cmp = PyObject_RichCompareBool(node->key, key, Py_EQ);
        if (cmp < 0)
            return NULL;

        if (cmp == 1)
        {
            Py_INCREF(node->value);
            return node->value;
        }

        node = node->next;
    }

    Py_RETURN_NONE;
}

int hashmap_remove(HashMap *map, PyObject *key)
{
    size_t hash = hash_key(key);
    if (hash == (size_t)-1)
        return -1;

    size_t index = hash % map->capacity;

    HashNode *node = map->buckets[index];
    HashNode *prev = NULL;

    while (node)
    {
        int cmp = PyObject_RichCompareBool(node->key, key, Py_EQ);
        if (cmp < 0)
            return -1;

        if (cmp == 1)
        {
            if (prev)
                prev->next = node->next;
            else
                map->buckets[index] = node->next;

            Py_DECREF(node->key);
            Py_DECREF(node->value);
            free(node);

            map->size--;
            return 0;
        }

        prev = node;
        node = node->next;
    }

    return -1;
}

void hashmap_free(HashMap *map)
{
    for (size_t i = 0; i < map->capacity; i++)
    {
        HashNode *node = map->buckets[i];

        while (node)
        {
            HashNode *next = node->next;

            Py_DECREF(node->key);
            Py_DECREF(node->value);
            free(node);

            node = next;
        }
    }

    free(map->buckets);
    free(map);
}