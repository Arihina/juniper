#include "hashmap.h"
#include <stdlib.h>

static size_t hash_key(PyObject *key)
{
    Py_hash_t h = PyObject_Hash(key);
    return (size_t)h;
}

HashMap *hashmap_create(size_t capacity)
{

    HashMap *map = malloc(sizeof(HashMap));

    map->capacity = capacity;
    map->size = 0;

    map->buckets = calloc(capacity, sizeof(HashNode *));

    return map;
}

static HashNode *node_create(PyObject *key, PyObject *value)
{

    HashNode *node = malloc(sizeof(HashNode));

    Py_INCREF(key);
    Py_INCREF(value);

    node->key = key;
    node->value = value;
    node->next = NULL;

    return node;
}

void hashmap_put(HashMap *map, PyObject *key, PyObject *value)
{

    size_t index = hash_key(key) % map->capacity;

    HashNode *node = map->buckets[index];

    while (node)
    {

        if (PyObject_RichCompareBool(node->key, key, Py_EQ))
        {

            Py_DECREF(node->value);
            Py_INCREF(value);
            node->value = value;

            return;
        }

        node = node->next;
    }

    HashNode *new_node = node_create(key, value);

    new_node->next = map->buckets[index];
    map->buckets[index] = new_node;

    map->size++;

    if ((double)map->size / map->capacity > 0.75)
        hashmap_resize(map, map->capacity * 2);
}

PyObject *hashmap_get(HashMap *map, PyObject *key)
{

    size_t index = hash_key(key) % map->capacity;

    HashNode *node = map->buckets[index];

    while (node)
    {

        if (PyObject_RichCompareBool(node->key, key, Py_EQ))
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

    size_t index = hash_key(key) % map->capacity;

    HashNode *node = map->buckets[index];
    HashNode *prev = NULL;

    while (node)
    {

        if (PyObject_RichCompareBool(node->key, key, Py_EQ))
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

void hashmap_resize(HashMap *map, size_t new_capacity)
{

    HashNode **new_buckets = calloc(new_capacity, sizeof(HashNode *));

    for (size_t i = 0; i < map->capacity; i++)
    {

        HashNode *node = map->buckets[i];

        while (node)
        {

            HashNode *next = node->next;

            size_t index = hash_key(node->key) % new_capacity;

            node->next = new_buckets[index];
            new_buckets[index] = node;

            node = next;
        }
    }

    free(map->buckets);

    map->buckets = new_buckets;
    map->capacity = new_capacity;
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
