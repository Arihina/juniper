#include "hashmap_internal.h"
#include "bucket_list.h"
#include "bucket_tree.h"
#include <stdlib.h>

#define TREEIFY_THRESHOLD 8
#define UNTREEIFY_THRESHOLD 6

static bucket *get_bucket(hashmap *hm,size_t h){
    size_t i=h%hm->cap;
    if(!hm->buckets[i]){
        bucket*b=calloc(1,sizeof(bucket));
        b->k=LIST; b->impl=bl_new();
        hm->buckets[i]=b;
    }
    return hm->buckets[i];
}

hashmap *hm_new(size_t cap,hash_fn h,cmp_fn c){
    hashmap*hm=calloc(1,sizeof(hashmap));
    hm->cap=cap; hm->hash=h; hm->cmp=c;
    hm->buckets=calloc(cap,sizeof(bucket*));
    return hm;
}

void hm_set(hashmap *hm, void *k, void *v) {
    size_t h = hm->hash(k);
    bucket *b = get_bucket(hm, h);

    int inserted = 0;

    if (b->k == LIST) {
        bucket_list *l = (bucket_list*)b->impl;

        inserted = bl_insert(l, h, k, v, hm->cmp);

        if (l->size >= TREEIFY_THRESHOLD) {
            bucket_tree *t = bt_new();

            for (list_node *n = l->head; n; n = n->next) {
                bt_insert(t, n->hash, n->key, n->val, hm->cmp);
            }

            list_node *n = l->head;
            while (n) {
                list_node *next = n->next;
                free(n);
                n = next;
            }

            free(l);

            b->k = TREE;
            b->impl = t;
        }

    } else {
        bt_insert(b->impl, h, k, v, hm->cmp);
        inserted = 1;
    }

    if (inserted) {
        hm->size++;
    }
}

void *hm_get(hashmap *hm, void *k) {
    size_t h = hm->hash(k);
    size_t i = h % hm->cap;

    bucket *b = hm->buckets[i];
    if (!b) return NULL;

    if (b->k == LIST) {
        return bl_find(b->impl, h, k, hm->cmp);
    } else {
        return bt_find(b->impl, h, k, hm->cmp);
    }
}

void hm_free(hashmap *hm) {
    if (!hm) return;

    for (size_t i = 0; i < hm->cap; i++) {
        bucket *b = hm->buckets[i];
        if (!b) continue;

        if (b->k == LIST) {
            bucket_list *l = (bucket_list*)b->impl;

            list_node *n = l->head;
            while (n) {
                list_node *next = n->next;
                free(n);
                n = next;
            }

            free(l);
        } else {
            bucket_tree *t = (bucket_tree*)b->impl;

            rb_node *stack[64];
            int sp = 0;
            rb_node *cur = t->root;

            while (cur || sp) {
                while (cur) {
                    stack[sp++] = cur;
                    cur = cur->l;
                }

                cur = stack[--sp];
                rb_node *right = cur->r;

                free(cur);
                cur = right;
            }

            free(t);
        }

        free(b);
    }

    free(hm->buckets);
    free(hm);
}

void hm_del(hashmap *hm, void *k) {
    size_t h = hm->hash(k);
    size_t i = h % hm->cap;

    bucket *b = hm->buckets[i];
    if (!b) return;

    int deleted = 0;

    if (b->k == LIST) {
        deleted = bl_delete(b->impl, h, k, hm->cmp);
    } else {
        bt_delete(b->impl, h, k, hm->cmp);
        deleted = 1;
    }

    if (deleted) {
        hm->size--;
    }
}

size_t hm_len(hashmap *hm){ return hm->size; }
