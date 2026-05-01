#pragma once
#include "hashmap.h"
#include "bucket.h"

typedef struct
{
    hashmap *hm;
    size_t bucket_index;
    hm_node *cur;
    unsigned long version;
} hm_iter;

void hm_iter_init(hm_iter *it, hashmap *hm);

int hm_iter_next(hm_iter *it, void **out_key, void **out_val);