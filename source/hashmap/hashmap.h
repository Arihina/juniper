#pragma once
#include <stddef.h>

typedef size_t (*hash_fn)(void *);
typedef int (*cmp_fn)(void *, void *);

typedef struct hashmap hashmap;

extern void *const HM_NEW_KEY;
extern void *const HM_ERROR;

hashmap *hm_new(size_t cap, hash_fn, cmp_fn);
void hm_free(hashmap *);
void hm_set_err_flag(hashmap *, int *flag);

void *hm_set(hashmap *, void *key, void *val);
int hm_get_ex(hashmap *, void *key, void **out);
int hm_del_ex(hashmap *, void *key, void **out_key, void **out_val);
size_t hm_len(hashmap *);
unsigned long hm_version(hashmap *);