#ifndef SLIST_H
#define SLIST_H

#include <stddef.h>

typedef struct SListNode {
    int value;
    struct SListNode* next;
} SListNode;

typedef struct {
    SListNode* head;
    size_t size;
} SList;

SList* slist_create();
void slist_push_front(SList* list, int value);
int slist_pop_front(SList* list);
void slist_free(SList* list);

#endif
