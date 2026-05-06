#include "bthashmap.h"
#include <stdlib.h>

static const double BTHM_MAX_LOAD = 4.0;
static const double BTHM_MIN_LOAD = 0.5;
static const size_t BTHM_MIN_BUCKETS = 8;

static size_t next_pow2(size_t n)
{
    if (n < 8)
        return 8;
    n--;
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;
#if SIZEOF_SIZE_T >= 8
    n |= n >> 32;
#endif
    return n + 1;
}

static size_t compute_hash(PyObject *key)
{
    Py_hash_t h = PyObject_Hash(key);
    if (h == -1 && PyErr_Occurred())
        return (size_t)-1;
    size_t u = (size_t)h;
    u ^= u >> 16;
    return u;
}

#define BUCKET_IDX(map, h) ((h) & ((map)->nbuckets - 1))

BTHashMap *bthm_create(size_t nbuckets, int bt_order)
{
    if (bt_order < 3)
        bt_order = BTHM_BUCKET_ORDER;

    BTHashMap *map = malloc(sizeof(BTHashMap));
    if (!map)
        return NULL;

    map->nbuckets = next_pow2(nbuckets);
    map->size = 0;
    map->bt_order = bt_order;

    map->buckets = calloc(map->nbuckets, sizeof(BTreeMap *));
    if (!map->buckets)
    {
        free(map);
        return NULL;
    }

    return map;
}

void bthm_clear(BTHashMap *map)
{
    for (size_t i = 0; i < map->nbuckets; i++)
    {
        if (map->buckets[i])
        {
            btm_free(map->buckets[i]);
            map->buckets[i] = NULL;
        }
    }
    map->size = 0;
}

void bthm_free(BTHashMap *map)
{
    if (!map)
        return;
    bthm_clear(map);
    free(map->buckets);
    free(map);
}

static int bthm_resize(BTHashMap *map, size_t new_nbuckets)
{
    BTreeMap **old_buckets = map->buckets;
    size_t old_nb = map->nbuckets;

    map->buckets = calloc(new_nbuckets, sizeof(BTreeMap *));
    if (!map->buckets)
    {
        map->buckets = old_buckets;
        return -1;
    }
    map->nbuckets = new_nbuckets;
    map->size = 0;

    for (size_t i = 0; i < old_nb; i++)
    {
        BTreeMap *bt = old_buckets[i];
        if (!bt)
            continue;

        BTMPos pos = btm_first(bt);
        while (pos.node)
        {
            PyObject *k = pos.node->keys[pos.idx];
            PyObject *v = pos.node->values[pos.idx];
            bthm_put(map, k, v);
            pos = btm_next(bt, pos);
        }
        btm_free(bt);
    }

    free(old_buckets);
    return 0;
}

int bthm_put(BTHashMap *map, PyObject *key, PyObject *value)
{
    if (map->size > (size_t)(map->nbuckets * BTHM_MAX_LOAD))
    {
        bthm_resize(map, map->nbuckets * 2);
    }

    size_t h = compute_hash(key);
    if (h == (size_t)-1)
        return -1;

    size_t idx = BUCKET_IDX(map, h);

    if (!map->buckets[idx])
    {
        map->buckets[idx] = btm_create(map->bt_order);
        if (!map->buckets[idx])
        {
            PyErr_NoMemory();
            return -1;
        }
    }

    int rc = btm_put(map->buckets[idx], key, value);
    if (rc == 0)
        map->size++;
    return rc;
}

PyObject *bthm_get(BTHashMap *map, PyObject *key)
{
    size_t h = compute_hash(key);
    if (h == (size_t)-1)
        return NULL;

    size_t idx = BUCKET_IDX(map, h);
    if (!map->buckets[idx])
        return NULL;

    return btm_get(map->buckets[idx], key);
}

int bthm_contains(BTHashMap *map, PyObject *key)
{
    size_t h = compute_hash(key);
    if (h == (size_t)-1)
        return -1;

    size_t idx = BUCKET_IDX(map, h);
    if (!map->buckets[idx])
        return 0;

    return btm_contains(map->buckets[idx], key);
}

int bthm_remove(BTHashMap *map, PyObject *key)
{
    size_t h = compute_hash(key);
    if (h == (size_t)-1)
        return -1;

    size_t idx = BUCKET_IDX(map, h);
    if (!map->buckets[idx])
        return 0;

    int rc = btm_remove(map->buckets[idx], key);
    if (rc > 0)
    {
        map->size--;

        if (map->buckets[idx]->size == 0)
        {
            btm_free(map->buckets[idx]);
            map->buckets[idx] = NULL;
        }

        size_t half = map->nbuckets / 2;
        if (half >= BTHM_MIN_BUCKETS &&
            map->size < (size_t)(map->nbuckets * BTHM_MIN_LOAD))
        {
            bthm_resize(map, half);
        }
    }
    return rc;
}