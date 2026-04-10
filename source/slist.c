#include "slist.h"
#include <stdlib.h>


SList *slist_create(void)
{
    SList *list = malloc(sizeof(SList));
    if (!list) return NULL;
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    return list;
}

void slist_clear(SList *list)
{
    SListNode *cur = list->head;
    while (cur) {
        SListNode *next = cur->next;
        Py_DECREF(cur->value);
        free(cur);
        cur = next;
    }
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

void slist_free(SList *list)
{
    if (!list) return;
    slist_clear(list);
    free(list);
}


static SListNode *node_create(PyObject *value)
{
    SListNode *node = malloc(sizeof(SListNode));
    if (!node) return NULL;
    Py_INCREF(value);
    node->value = value;
    node->next  = NULL;
    return node;
}


int slist_push_front(SList *list, PyObject *value)
{
    SListNode *node = node_create(value);
    if (!node) return -1;
    node->next = list->head;
    list->head = node;
    if (!list->tail)
        list->tail = node;
    list->size++;
    return 0;
}

int slist_push_back(SList *list, PyObject *value)
{
    SListNode *node = node_create(value);
    if (!node) return -1;
    if (list->tail)
        list->tail->next = node;
    else
        list->head = node;
    list->tail = node;
    list->size++;
    return 0;
}

int slist_insert(SList *list, size_t index, PyObject *value)
{
    if (index == 0)
        return slist_push_front(list, value);
    if (index >= list->size)
        return slist_push_back(list, value);

    SListNode *node = node_create(value);
    if (!node) return -1;

    SListNode *prev = list->head;
    for (size_t i = 0; i < index - 1; i++)
        prev = prev->next;

    node->next = prev->next;
    prev->next = node;
    list->size++;
    return 0;
}

PyObject *slist_pop_front(SList *list)
{
    if (!list->head) return NULL;

    SListNode *node = list->head;
    PyObject *value = node->value;

    list->head = node->next;
    if (!list->head)
        list->tail = NULL;

    free(node);
    list->size--;
    return value;
}

PyObject *slist_pop_back(SList *list)
{
    if (!list->head) return NULL;

    if (list->head == list->tail)
        return slist_pop_front(list);

    SListNode *prev = list->head;
    while (prev->next != list->tail)
        prev = prev->next;

    SListNode *node = list->tail;
    PyObject *value = node->value;

    prev->next = NULL;
    list->tail = prev;
    free(node);
    list->size--;
    return value;
}

int slist_remove(SList *list, PyObject *value)
{
    SListNode *prev = NULL;
    SListNode *cur  = list->head;

    while (cur) {
        int cmp = PyObject_RichCompareBool(cur->value, value, Py_EQ);
        if (cmp < 0) return -1;
        if (cmp) {
            if (prev)
                prev->next = cur->next;
            else
                list->head = cur->next;

            if (cur == list->tail)
                list->tail = prev;

            Py_DECREF(cur->value);
            free(cur);
            list->size--;
            return 1;
        }
        prev = cur;
        cur = cur->next;
    }
    return 0;
}

PyObject *slist_get(SList *list, size_t index)
{
    if (index >= list->size) return NULL;
    SListNode *cur = list->head;
    for (size_t i = 0; i < index; i++)
        cur = cur->next;
    Py_INCREF(cur->value);
    return cur->value;
}

int slist_contains(SList *list, PyObject *value)
{
    SListNode *cur = list->head;
    while (cur) {
        int cmp = PyObject_RichCompareBool(cur->value, value, Py_EQ);
        if (cmp < 0) return -1;
        if (cmp) return 1;
        cur = cur->next;
    }
    return 0;
}

Py_ssize_t slist_index(SList *list, PyObject *value)
{
    SListNode *cur = list->head;
    Py_ssize_t idx = 0;
    while (cur) {
        int cmp = PyObject_RichCompareBool(cur->value, value, Py_EQ);
        if (cmp < 0) return -2;
        if (cmp) return idx;
        cur = cur->next;
        idx++;
    }
    return -1;
}

void slist_reverse(SList *list)
{
    if (list->size <= 1) return;
    list->tail = list->head;

    SListNode *prev = NULL;
    SListNode *cur  = list->head;
    while (cur) {
        SListNode *next = cur->next;
        cur->next = prev;
        prev = cur;
        cur = next;
    }
    list->head = prev;
}
