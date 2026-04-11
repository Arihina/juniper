#include "bucket_list.h"
#include <stdlib.h>

bucket_list *bl_new() { return calloc(1,sizeof(bucket_list)); }

int bl_insert(bucket_list *b, size_t h, void *k, void *v, int (*cmp)()) {
    for (list_node *n = b->head; n; n = n->next) {
        if (n->hash == h && cmp(n->key, k) == 0) {
            n->val = v;
            return 0;
        }
    }

    list_node *n = calloc(1, sizeof(list_node));
    n->hash = h;
    n->key = k;
    n->val = v;
    n->next = b->head;

    b->head = n;
    b->size++;

    return 1;
}

void *bl_find(bucket_list *b,size_t h,void *k,int(*cmp)()){
    for(list_node*n=b->head;n;n=n->next)
        if(n->hash==h && cmp(n->key,k)==0) return n->val;
    return NULL;
}

int bl_delete(bucket_list *b, size_t h, void *k, int (*cmp)()) {
    list_node **pp = &b->head;

    while (*pp) {
        list_node *n = *pp;

        if (n->hash == h && cmp(n->key, k) == 0) {
            *pp = n->next;
            free(n);
            b->size--;
            return 1;
        }

        pp = &n->next;
    }

    return 0;
}
