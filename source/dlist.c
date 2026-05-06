#include "dlist.h"
#include <stdlib.h>

DList *dlist_create(void)
{
    DList *list = malloc(sizeof(DList));
    if (!list)
        return NULL;
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    return list;
}

void dlist_clear(DList *list)
{
    DListNode *cur = list->head;
    while (cur)
    {
        DListNode *next = cur->next;
        Py_DECREF(cur->value);
        free(cur);
        cur = next;
    }
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

void dlist_free(DList *list)
{
    if (!list)
        return;
    dlist_clear(list);
    free(list);
}

static DListNode *node_create(PyObject *value)
{
    DListNode *node = malloc(sizeof(DListNode));
    if (!node)
        return NULL;
    Py_INCREF(value);
    node->value = value;
    node->prev = NULL;
    node->next = NULL;
    return node;
}

static void node_unlink(DList *list, DListNode *node)
{
    if (node->prev)
        node->prev->next = node->next;
    else
        list->head = node->next;

    if (node->next)
        node->next->prev = node->prev;
    else
        list->tail = node->prev;

    Py_DECREF(node->value);
    free(node);
    list->size--;
}

int dlist_push_front(DList *list, PyObject *value)
{
    DListNode *node = node_create(value);
    if (!node)
        return -1;

    node->next = list->head;
    if (list->head)
        list->head->prev = node;
    else
        list->tail = node;
    list->head = node;
    list->size++;
    return 0;
}

int dlist_push_back(DList *list, PyObject *value)
{
    DListNode *node = node_create(value);
    if (!node)
        return -1;

    node->prev = list->tail;
    if (list->tail)
        list->tail->next = node;
    else
        list->head = node;
    list->tail = node;
    list->size++;
    return 0;
}

int dlist_insert(DList *list, size_t index, PyObject *value)
{
    if (index == 0)
        return dlist_push_front(list, value);
    if (index >= list->size)
        return dlist_push_back(list, value);

    DListNode *node = node_create(value);
    if (!node)
        return -1;

    DListNode *at;
    if (index <= list->size / 2)
    {
        at = list->head;
        for (size_t i = 0; i < index; i++)
            at = at->next;
    }
    else
    {
        at = list->tail;
        for (size_t i = list->size - 1; i > index; i--)
            at = at->prev;
    }
    node->next = at;
    node->prev = at->prev;
    at->prev->next = node;
    at->prev = node;
    list->size++;
    return 0;
}

PyObject *dlist_pop_front(DList *list)
{
    if (!list->head)
        return NULL;

    DListNode *node = list->head;
    PyObject *value = node->value;

    list->head = node->next;
    if (list->head)
        list->head->prev = NULL;
    else
        list->tail = NULL;

    free(node);
    list->size--;
    return value;
}

PyObject *dlist_pop_back(DList *list)
{
    if (!list->tail)
        return NULL;

    DListNode *node = list->tail;
    PyObject *value = node->value;

    list->tail = node->prev;
    if (list->tail)
        list->tail->next = NULL;
    else
        list->head = NULL;

    free(node);
    list->size--;
    return value;
}

int dlist_remove(DList *list, PyObject *value)
{
    DListNode *cur = list->head;
    while (cur)
    {
        int cmp = PyObject_RichCompareBool(cur->value, value, Py_EQ);
        if (cmp < 0)
            return -1;
        if (cmp)
        {
            node_unlink(list, cur);
            return 1;
        }
        cur = cur->next;
    }
    return 0;
}

static DListNode *dlist_node_at(DList *list, size_t index)
{
    DListNode *cur;
    if (index <= list->size / 2)
    {
        cur = list->head;
        for (size_t i = 0; i < index; i++)
            cur = cur->next;
    }
    else
    {
        cur = list->tail;
        for (size_t i = list->size - 1; i > index; i--)
            cur = cur->prev;
    }
    return cur;
}

PyObject *dlist_get(DList *list, size_t index)
{
    if (index >= list->size)
        return NULL;
    DListNode *node = dlist_node_at(list, index);
    Py_INCREF(node->value);
    return node->value;
}

int dlist_contains(DList *list, PyObject *value)
{
    DListNode *cur = list->head;
    while (cur)
    {
        int cmp = PyObject_RichCompareBool(cur->value, value, Py_EQ);
        if (cmp < 0)
            return -1;
        if (cmp)
            return 1;
        cur = cur->next;
    }
    return 0;
}

Py_ssize_t dlist_index(DList *list, PyObject *value)
{
    DListNode *cur = list->head;
    Py_ssize_t idx = 0;
    while (cur)
    {
        int cmp = PyObject_RichCompareBool(cur->value, value, Py_EQ);
        if (cmp < 0)
            return -2;
        if (cmp)
            return idx;
        cur = cur->next;
        idx++;
    }
    return -1;
}

void dlist_reverse(DList *list)
{
    if (list->size <= 1)
        return;

    DListNode *cur = list->head;
    while (cur)
    {
        DListNode *tmp = cur->next;
        cur->next = cur->prev;
        cur->prev = tmp;
        cur = tmp;
    }

    DListNode *tmp = list->head;
    list->head = list->tail;
    list->tail = tmp;
}

void dlist_move_to_front(DList *list, DListNode *node)
{
    if (!node || node == list->head || list->size <= 1)
        return;

    if (node->prev)
        node->prev->next = node->next;
    if (node->next)
        node->next->prev = node->prev;
    else
        list->tail = node->prev;

    node->prev = NULL;
    node->next = list->head;
    list->head->prev = node;
    list->head = node;
}

void dlist_move_to_back(DList *list, DListNode *node)
{
    if (!node || node == list->tail || list->size <= 1)
        return;

    if (node->prev)
        node->prev->next = node->next;
    else
        list->head = node->next;
    if (node->next)
        node->next->prev = node->prev;

    node->next = NULL;
    node->prev = list->tail;
    list->tail->next = node;
    list->tail = node;
}

DListNode *dlist_push_front_node(DList *list, PyObject *value)
{
    DListNode *node = node_create(value);
    if (!node)
        return NULL;

    node->next = list->head;
    if (list->head)
        list->head->prev = node;
    else
        list->tail = node;
    list->head = node;
    list->size++;
    return node;
}

DListNode *dlist_push_back_node(DList *list, PyObject *value)
{
    DListNode *node = node_create(value);
    if (!node)
        return NULL;

    node->prev = list->tail;
    if (list->tail)
        list->tail->next = node;
    else
        list->head = node;
    list->tail = node;
    list->size++;
    return node;
}

void dlist_unlink_keep(DList *list, DListNode *node)
{
    if (node->prev)
        node->prev->next = node->next;
    else
        list->head = node->next;

    if (node->next)
        node->next->prev = node->prev;
    else
        list->tail = node->prev;

    node->prev = NULL;
    node->next = NULL;
    list->size--;
}