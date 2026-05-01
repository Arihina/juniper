#pragma once
#include <stddef.h>

typedef struct hm_node
{
    size_t hash;
    void *key;
    void *val;

    struct hm_node *prev;
    struct hm_node *next;

    struct hm_node *parent;
    struct hm_node *left;
    struct hm_node *right;
    int red;
} hm_node;

typedef enum
{
    BK_LIST,
    BK_TREE
} bucket_kind;

typedef struct
{
    bucket_kind kind;
    size_t size;
    hm_node *head;
    hm_node *root;
} bucket;

bucket *bucket_new(void);
void bucket_free(bucket *b);

int bucket_insert(bucket *b, size_t h, void *k, void *v,
                  int (*cmp)(void *, void *), void **old_val);
int bucket_find(bucket *b, size_t h, void *k,
                int (*cmp)(void *, void *), void **out);
int bucket_delete(bucket *b, size_t h, void *k,
                  int (*cmp)(void *, void *),
                  void **out_key, void **out_val);

void bucket_treeify(bucket *b, int (*cmp)(void *, void *));
void bucket_untreeify(bucket *b);