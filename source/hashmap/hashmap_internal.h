#pragma once
#include "hashmap.h"
#include "bucket_list.h"
#include "bucket_tree.h"

typedef enum { LIST, TREE } bucket_kind;

typedef struct bucket {
    bucket_kind k;
    void *impl;
} bucket;

struct hashmap {
    size_t size;
    size_t cap;
    hash_fn hash;
    cmp_fn cmp;
    bucket **buckets;
};
