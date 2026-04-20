#include "swisstable.h"
#include <stdlib.h>
#include <string.h>

static uint32_t group_match(const uint8_t *ctrl, size_t offset, uint8_t h2)
{
    uint32_t mask = 0;
    const uint8_t *g = ctrl + offset;
    for (int i = 0; i < SW_GROUP_SIZE; i++) {
        if (g[i] == h2)
            mask |= (1u << i);
    }
    return mask;
}

static uint32_t group_match_empty(const uint8_t *ctrl, size_t offset)
{
    uint32_t mask = 0;
    const uint8_t *g = ctrl + offset;
    for (int i = 0; i < SW_GROUP_SIZE; i++) {
        if (g[i] == SW_EMPTY)
            mask |= (1u << i);
    }
    return mask;
}

static uint32_t group_match_empty_or_deleted(const uint8_t *ctrl, size_t offset)
{
    uint32_t mask = 0;
    const uint8_t *g = ctrl + offset;
    for (int i = 0; i < SW_GROUP_SIZE; i++) {
        if (g[i] >= SW_DELETED)
            mask |= (1u << i);
    }
    return mask;
}

static int ctz32(uint32_t x)
{
    if (x == 0) return 32;
    int n = 0;
    while ((x & 1) == 0) { x >>= 1; n++; }
    return n;
}

static size_t compute_hash(PyObject *key)
{
    Py_hash_t h = PyObject_Hash(key);
    if (h == -1 && PyErr_Occurred())
        return (size_t)-1;
    return (size_t)h;
}

#define NUM_GROUPS(cap) ((cap) / SW_GROUP_SIZE)

static size_t capacity_to_growth(size_t cap)
{
    return cap - cap / 8;
}

static size_t normalize_cap(size_t n)
{
    if (n < SW_GROUP_SIZE) n = SW_GROUP_SIZE;
    return (n + SW_GROUP_SIZE - 1) & ~(size_t)(SW_GROUP_SIZE - 1);
}

SwissTable *sw_create(size_t capacity)
{
    SwissTable *map = malloc(sizeof(SwissTable));
    if (!map) return NULL;

    map->capacity = normalize_cap(capacity);
    map->size     = 0;

    /* Allocate ctrl with GROUP_SIZE extra bytes for group overflow reads */
    map->ctrl = malloc(map->capacity + SW_GROUP_SIZE);
    if (!map->ctrl) { free(map); return NULL; }
    memset(map->ctrl, SW_EMPTY, map->capacity + SW_GROUP_SIZE);

    map->slots = calloc(map->capacity, sizeof(SWSlot));
    if (!map->slots) { free(map->ctrl); free(map); return NULL; }

    map->growth_left = capacity_to_growth(map->capacity);
    return map;
}

void sw_clear(SwissTable *map)
{
    for (size_t i = 0; i < map->capacity; i++) {
        if (map->ctrl[i] != SW_EMPTY && map->ctrl[i] != SW_DELETED) {
            Py_DECREF(map->slots[i].key);
            Py_DECREF(map->slots[i].value);
        }
    }
    memset(map->ctrl, SW_EMPTY, map->capacity + SW_GROUP_SIZE);
    memset(map->slots, 0, map->capacity * sizeof(SWSlot));
    map->size = 0;
    map->growth_left = capacity_to_growth(map->capacity);
}

void sw_free(SwissTable *map)
{
    if (!map) return;
    for (size_t i = 0; i < map->capacity; i++) {
        if (map->ctrl[i] != SW_EMPTY && map->ctrl[i] != SW_DELETED) {
            Py_DECREF(map->slots[i].key);
            Py_DECREF(map->slots[i].value);
        }
    }
    free(map->ctrl);
    free(map->slots);
    free(map);
}

static int sw_resize(SwissTable *map, size_t new_cap)
{
    new_cap = normalize_cap(new_cap);

    uint8_t *old_ctrl  = map->ctrl;
    SWSlot  *old_slots = map->slots;
    size_t   old_cap   = map->capacity;

    map->ctrl = malloc(new_cap + SW_GROUP_SIZE);
    if (!map->ctrl) { map->ctrl = old_ctrl; return -1; }
    memset(map->ctrl, SW_EMPTY, new_cap + SW_GROUP_SIZE);

    map->slots = calloc(new_cap, sizeof(SWSlot));
    if (!map->slots) {
        free(map->ctrl);
        map->ctrl  = old_ctrl;
        map->slots = old_slots;
        return -1;
    }

    map->capacity    = new_cap;
    map->size        = 0;
    map->growth_left = capacity_to_growth(new_cap);

    for (size_t i = 0; i < old_cap; i++) {
        if (old_ctrl[i] != SW_EMPTY && old_ctrl[i] != SW_DELETED) {
            size_t h  = old_slots[i].hash;
            uint8_t h2 = SW_H2(h);
            size_t num_groups = NUM_GROUPS(map->capacity);
            size_t gi = SW_H1(h) % num_groups;

            for (size_t probe = 0; ; probe++) {
                size_t group_off = gi * SW_GROUP_SIZE;
                uint32_t empty = group_match_empty(map->ctrl, group_off);
                if (empty) {
                    int pos = ctz32(empty);
                    size_t slot_idx = group_off + pos;
                    map->ctrl[slot_idx] = h2;

                    if (slot_idx < SW_GROUP_SIZE)
                        map->ctrl[map->capacity + slot_idx] = h2;
                    map->slots[slot_idx].key   = old_slots[i].key;
                    map->slots[slot_idx].value  = old_slots[i].value;
                    map->slots[slot_idx].hash   = h;
                    map->size++;
                    map->growth_left--;
                    break;
                }
                gi = (gi + probe + 1) % num_groups;
            }
        }
    }

    free(old_ctrl);
    free(old_slots);
    return 0;
}

static size_t sw_find(SwissTable *map, PyObject *key, size_t h)
{
    uint8_t h2 = SW_H2(h);
    size_t num_groups = NUM_GROUPS(map->capacity);
    size_t gi = SW_H1(h) % num_groups;

    for (size_t probe = 0; ; probe++) {
        size_t group_off = gi * SW_GROUP_SIZE;

        uint32_t match = group_match(map->ctrl, group_off, h2);
        while (match) {
            int pos = ctz32(match);
            size_t slot_idx = group_off + pos;
            SWSlot *s = &map->slots[slot_idx];
            if (s->hash == h) {
                int eq = PyObject_RichCompareBool(s->key, key, Py_EQ);
                if (eq < 0) return (size_t)-2;
                if (eq) return slot_idx;
            }
            match &= match - 1;
        }

        uint32_t empty = group_match_empty(map->ctrl, group_off);
        if (empty)
            return (size_t)-1;

        gi = (gi + probe + 1) % num_groups;
    }
}

int sw_put(SwissTable *map, PyObject *key, PyObject *value)
{
    size_t h = compute_hash(key);
    if (h == (size_t)-1) return -1;

    size_t existing = sw_find(map, key, h);
    if (existing == (size_t)-2) return -1;
    if (existing != (size_t)-1) {
        Py_INCREF(value);
        Py_DECREF(map->slots[existing].value);
        map->slots[existing].value = value;
        return 1;
    }

    if (map->growth_left == 0) {
        if (sw_resize(map, map->capacity * 2) < 0) {
            PyErr_NoMemory();
            return -1;
        }
    }

    uint8_t h2 = SW_H2(h);
    size_t num_groups = NUM_GROUPS(map->capacity);
    size_t gi = SW_H1(h) % num_groups;

    for (size_t probe = 0; ; probe++) {
        size_t group_off = gi * SW_GROUP_SIZE;
        uint32_t avail = group_match_empty_or_deleted(map->ctrl, group_off);
        if (avail) {
            int pos = ctz32(avail);
            size_t slot_idx = group_off + pos;

            int was_empty = (map->ctrl[slot_idx] == SW_EMPTY);

            Py_INCREF(key);
            Py_INCREF(value);
            map->ctrl[slot_idx] = h2;
            if (slot_idx < SW_GROUP_SIZE)
                map->ctrl[map->capacity + slot_idx] = h2;
            map->slots[slot_idx].key   = key;
            map->slots[slot_idx].value = value;
            map->slots[slot_idx].hash  = h;
            map->size++;

            if (was_empty)
                map->growth_left--;

            return 0;
        }
        gi = (gi + probe + 1) % num_groups;
    }
}

PyObject *sw_get(SwissTable *map, PyObject *key)
{
    size_t h = compute_hash(key);
    if (h == (size_t)-1) return NULL;

    size_t idx = sw_find(map, key, h);
    if (idx == (size_t)-2) return NULL;
    if (idx == (size_t)-1) return NULL;

    Py_INCREF(map->slots[idx].value);
    return map->slots[idx].value;
}

int sw_contains(SwissTable *map, PyObject *key)
{
    size_t h = compute_hash(key);
    if (h == (size_t)-1) return -1;

    size_t idx = sw_find(map, key, h);
    if (idx == (size_t)-2) return -1;
    return (idx != (size_t)-1) ? 1 : 0;
}

int sw_remove(SwissTable *map, PyObject *key)
{
    size_t h = compute_hash(key);
    if (h == (size_t)-1) return -1;

    size_t idx = sw_find(map, key, h);
    if (idx == (size_t)-2) return -1;
    if (idx == (size_t)-1) return 0;

    Py_DECREF(map->slots[idx].key);
    Py_DECREF(map->slots[idx].value);
    map->slots[idx].key   = NULL;
    map->slots[idx].value = NULL;

    map->ctrl[idx] = SW_DELETED;
    if (idx < SW_GROUP_SIZE)
        map->ctrl[map->capacity + idx] = SW_DELETED;

    map->size--;

    size_t half = map->capacity / 2;
    if (half >= SW_GROUP_SIZE &&
        map->size < map->capacity / 4)
    {
        sw_resize(map, half);
    }

    return 1;
}