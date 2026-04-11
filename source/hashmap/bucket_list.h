#pragma once
#include <stddef.h>

typedef struct list_node {
    size_t hash;
    void *key;
    void *val;
    struct list_node *next;
} list_node;

typedef struct {
    list_node *head;
    size_t size;
} bucket_list;

bucket_list *bl_new();
int bl_insert(bucket_list *b, size_t h, void *k, void *v, int (*cmp)());
void *bl_find(bucket_list *b, size_t h, void *k, int (*cmp)());
int bl_delete(bucket_list *b, size_t h, void *k, int (*cmp)());
