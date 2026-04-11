#include "rhmap.h"
#include <stdlib.h>
#include <string.h>

static const double RHMAP_MAX_LOAD = 0.75;

static size_t next_pow2(size_t n)
{
    if (n < 8) return 8;
    n--;
    n |= n >> 1;  n |= n >> 2;
    n |= n >> 4;  n |= n >> 8;
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

#define SLOT_INDEX(map, h) ((h) & ((map)->capacity - 1))


RHMap *rhmap_create(size_t capacity)
{
    RHMap *map = malloc(sizeof(RHMap));
    if (!map) return NULL;

    map->capacity = next_pow2(capacity);
    map->size     = 0;
    map->slots    = calloc(map->capacity, sizeof(RHSlot));
    if (!map->slots) {
        free(map);
        return NULL;
    }
    return map;
}

void rhmap_clear(RHMap *map)
{
    for (size_t i = 0; i < map->capacity; i++) {
        if (map->slots[i].state == RHMAP_USED) {
            Py_DECREF(map->slots[i].key);
            Py_DECREF(map->slots[i].value);
            map->slots[i].state = RHMAP_EMPTY;
        }
    }
    map->size = 0;
}

void rhmap_free(RHMap *map)
{
    if (!map) return;
    rhmap_clear(map);
    free(map->slots);
    free(map);
}

static int rhmap_resize(RHMap *map, size_t new_cap)
{
    RHSlot *old_slots = map->slots;
    size_t  old_cap   = map->capacity;

    map->slots    = calloc(new_cap, sizeof(RHSlot));
    if (!map->slots) {
        map->slots = old_slots;
        return -1;
    }
    map->capacity = new_cap;
    map->size     = 0;

    for (size_t i = 0; i < old_cap; i++) {
        if (old_slots[i].state == RHMAP_USED) {
            size_t idx = SLOT_INDEX(map, old_slots[i].hash);
            size_t psl = 0;

            RHSlot incoming = {
                .key   = old_slots[i].key,
                .value = old_slots[i].value,
                .hash  = old_slots[i].hash,
                .psl   = 0,
                .state = RHMAP_USED,
            };

            for (;;) {
                RHSlot *s = &map->slots[idx];
                if (s->state == RHMAP_EMPTY) {
                    incoming.psl = psl;
                    *s = incoming;
                    map->size++;
                    break;
                }

                if (psl > s->psl) {
                    incoming.psl = psl;
                    RHSlot tmp = *s;
                    *s = incoming;
                    incoming = tmp;
                    psl = incoming.psl;
                }
                psl++;
                idx = (idx + 1) & (map->capacity - 1);
            }
        }
    }

    free(old_slots);
    return 0;
}

int rhmap_put(RHMap *map, PyObject *key, PyObject *value)
{
    if ((map->size + 1) > (size_t)(map->capacity * RHMAP_MAX_LOAD)) {
        if (rhmap_resize(map, map->capacity * 2) < 0) {
            PyErr_NoMemory();
            return -1;
        }
    }

    size_t h = compute_hash(key);
    if (h == (size_t)-1) return -1;

    size_t idx = SLOT_INDEX(map, h);
    size_t psl = 0;

    RHSlot incoming = {
        .key   = key,
        .value = value,
        .hash  = h,
        .psl   = 0,
        .state = RHMAP_USED,
    };
    int did_incref = 0;

    for (;;) {
        RHSlot *s = &map->slots[idx];

        if (s->state == RHMAP_EMPTY) {
            if (!did_incref) {
                Py_INCREF(incoming.key);
                Py_INCREF(incoming.value);
            }
            incoming.psl = psl;
            *s = incoming;
            map->size++;
            return 0;
        }

        if (!did_incref && s->hash == h) {
            int eq = PyObject_RichCompareBool(s->key, key, Py_EQ);
            if (eq < 0) return -1;
            if (eq) {
                Py_INCREF(value);
                Py_DECREF(s->value);
                s->value = value;
                return 1;\
            }
        }

        if (psl > s->psl) {
            if (!did_incref) {
                Py_INCREF(incoming.key);
                Py_INCREF(incoming.value);
                did_incref = 1;
            }
            incoming.psl = psl;
            RHSlot tmp = *s;
            *s = incoming;
            incoming = tmp;
            psl = incoming.psl;
        }

        psl++;
        idx = (idx + 1) & (map->capacity - 1);
    }
}

PyObject *rhmap_get(RHMap *map, PyObject *key)
{
    size_t h = compute_hash(key);
    if (h == (size_t)-1) return NULL;

    size_t idx = SLOT_INDEX(map, h);
    size_t psl = 0;

    for (;;) {
        RHSlot *s = &map->slots[idx];

        if (s->state == RHMAP_EMPTY || psl > s->psl)
            return NULL;

        if (s->hash == h) {
            int eq = PyObject_RichCompareBool(s->key, key, Py_EQ);
            if (eq < 0) return NULL;
            if (eq) {
                Py_INCREF(s->value);
                return s->value;
            }
        }

        psl++;
        idx = (idx + 1) & (map->capacity - 1);
    }
}

int rhmap_contains(RHMap *map, PyObject *key)
{
    size_t h = compute_hash(key);
    if (h == (size_t)-1) return -1;

    size_t idx = SLOT_INDEX(map, h);
    size_t psl = 0;

    for (;;) {
        RHSlot *s = &map->slots[idx];

        if (s->state == RHMAP_EMPTY || psl > s->psl)
            return 0;

        if (s->hash == h) {
            int eq = PyObject_RichCompareBool(s->key, key, Py_EQ);
            if (eq < 0) return -1;
            if (eq) return 1;
        }

        psl++;
        idx = (idx + 1) & (map->capacity - 1);
    }
}

int rhmap_remove(RHMap *map, PyObject *key)
{
    size_t h = compute_hash(key);
    if (h == (size_t)-1) return -1;

    size_t idx = SLOT_INDEX(map, h);
    size_t psl = 0;

    for (;;) {
        RHSlot *s = &map->slots[idx];

        if (s->state == RHMAP_EMPTY || psl > s->psl)
            return 0;

        if (s->hash == h) {
            int eq = PyObject_RichCompareBool(s->key, key, Py_EQ);
            if (eq < 0) return -1;
            if (eq) break;
        }

        psl++;
        idx = (idx + 1) & (map->capacity - 1);
    }

    Py_DECREF(map->slots[idx].key);
    Py_DECREF(map->slots[idx].value);
    map->slots[idx].state = RHMAP_EMPTY;

    size_t prev = idx;
    idx = (idx + 1) & (map->capacity - 1);

    while (map->slots[idx].state == RHMAP_USED && map->slots[idx].psl > 0) {
        map->slots[prev] = map->slots[idx];
        map->slots[prev].psl--;
        map->slots[idx].state = RHMAP_EMPTY;
        prev = idx;
        idx = (idx + 1) & (map->capacity - 1);
    }

    map->size--;
    return 1;
}
