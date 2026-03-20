#include "slist.h"
#include <stdlib.h>

SList* slist_create() {
    SList* list = malloc(sizeof(SList));
    if (!list) return NULL;

    list->head = NULL;
    list->size = 0;
    return list;
}

void slist_push_front(SList* list, int value) {
    SListNode* node = malloc(sizeof(SListNode));
    if (!node) return;

    node->value = value;
    node->next = list->head;
    list->head = node;
    list->size++;
}

int slist_pop_front(SList* list) {
    if (!list->head) return -1;

    SListNode* temp = list->head;
    int value = temp->value;
    list->head = temp->next;
    free(temp);
    list->size--;
    return value;
}

void slist_free(SList* list) {
    SListNode* current = list->head;
    while (current) {
        SListNode* next = current->next;
        free(current);
        current = next;
    }
    free(list);
}
