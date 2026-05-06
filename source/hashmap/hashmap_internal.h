#pragma once
#include "hashmap.h"
#include "bucket.h"

struct hashmap
{
    size_t size;
    size_t cap;
    hash_fn hash;
    cmp_fn cmp;
    bucket **buckets;
    unsigned long version;
    int *err_flag;
};