#ifndef SWISSTABLE_H
#define SWISSTABLE_H

#include <Python.h>
#include <stddef.h>
#include <stdint.h>

/*
 * Swiss Table — open-addressing hash map inspired by Abseil's flat_hash_map.
 *
 * Layout:
 *   - ctrl[]:   array of control bytes (1 byte per slot)
 *   - slots[]:  parallel array of (key, value, hash) per slot
 *
 * Control byte encoding:
 *   0b1111_1111  (0xFF) = EMPTY  — never been used
 *   0b1000_0000  (0x80) = DELETED (tombstone)
 *   0b0xxx_xxxx  = USED — stores H2 (top 7 bits of hash)
 *
 * Probing: triangular probing over groups of 16 slots.
 *   group_index(i) = (h1 + i*(i+1)/2) mod (capacity/16)
 *
 * Within a group, the 16 control bytes are scanned with
 * portable bitwise operations to find H2 matches or empties.
 *
 * Capacity is always a multiple of 16.
 */

#define SW_GROUP_SIZE 16
#define SW_EMPTY ((uint8_t)0xFF)
#define SW_DELETED ((uint8_t)0x80)

#define SW_H2(hash) ((uint8_t)((hash) >> 57))
#define SW_H1(hash) ((size_t)(hash))

typedef struct {
    PyObject *key;
    PyObject *value;
    size_t hash;
} SWSlot;

typedef struct {
    uint8_t *ctrl; 
    SWSlot *slots;
    size_t capacity;
    size_t size;
    size_t growth_left;
} SwissTable;

SwissTable *sw_create(size_t capacity);

void sw_free(SwissTable *map);
int sw_put(SwissTable *map, PyObject *key, PyObject *value);

PyObject *sw_get(SwissTable *map, PyObject *key);

int sw_remove(SwissTable *map, PyObject *key);
int sw_contains(SwissTable *map, PyObject *key);
void sw_clear(SwissTable *map);

#endif