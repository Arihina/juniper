#pragma once
#include "hashmap.h"
#include "bucket_list.h"
#include "bucket_tree.h"

typedef struct {
    hashmap *hm;
    size_t bucket_index;

    list_node *lnode;

    rb_node *stack[64];
    int sp;

    int bucket_started;
} hm_iter;

void hm_iter_init(hm_iter *it, hashmap *hm);
void *hm_iter_next(hm_iter *it);