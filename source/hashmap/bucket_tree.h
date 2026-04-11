#pragma once
#include <stddef.h>

typedef struct rb_node {
    size_t hash;
    void *key;
    void *val;
    struct rb_node *l,*r;
} rb_node;

typedef struct {
    rb_node *root;
    size_t size;
} bucket_tree;

bucket_tree *bt_new();
void bt_insert(bucket_tree*,size_t,void*,void*,int(*cmp)());
void *bt_find(bucket_tree*,size_t,void*,int(*cmp)());
void bt_delete(bucket_tree*,size_t,void*,int(*cmp)());
