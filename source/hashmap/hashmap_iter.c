#include "hashmap_internal.h"
#include "bucket_list.h"
#include "bucket_tree.h"
#include "hashmap_iter.h"
#include <string.h>

static void push_left(hm_iter *it, rb_node *n) {
    while (n) {
        it->stack[it->sp++] = n;
        n = n->l;
    }
}

static rb_node* next_tree(hm_iter *it) {
    if (it->sp == 0) return NULL;

    rb_node *n = it->stack[--it->sp];
    push_left(it, n->r);
    return n;
}

void hm_iter_init(hm_iter *it, hashmap *hm) {
    memset(it, 0, sizeof(hm_iter));

    it->hm = hm;
    it->bucket_index = 0;
    it->lnode = NULL;
    it->sp = 0;
}

void* hm_iter_next(hm_iter *it) {
    while (it->bucket_index < it->hm->cap) {
        bucket *b = it->hm->buckets[it->bucket_index];

        if (!b) {
            it->bucket_index++;
            it->bucket_started = 0;
            continue;
        }

        if (!it->bucket_started) {
            if (b->k == LIST) {
                bucket_list *l = (bucket_list*)b->impl;
                it->lnode = l->head;
            } else {
                push_left(it, ((bucket_tree*)b->impl)->root);
            }

            it->bucket_started = 1;
        }

        if (b->k == LIST) {
            if (it->lnode) {
                list_node *cur = it->lnode;
                it->lnode = cur->next;
                return cur->key;
            }
        } else {
            rb_node *n = next_tree(it);
            if (n) {
                return n->key;
            }
        }

        it->bucket_index++;
        it->bucket_started = 0;
        it->lnode = NULL;
        it->sp = 0;
    }

    return NULL;
}
