#pragma once
#include <stddef.h>

typedef size_t (*hash_fn)(void *);
typedef int (*cmp_fn)(void *, void *);

typedef struct hashmap hashmap;

hashmap *hm_new(size_t cap, hash_fn, cmp_fn);
void hm_set(hashmap *, void *key, void *val);
void *hm_get(hashmap *, void *key);
void hm_del(hashmap *, void *key);
size_t hm_len(hashmap *);
void hm_free(hashmap *);
