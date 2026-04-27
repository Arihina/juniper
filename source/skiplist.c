#include "skiplist.h"
#include <stdlib.h>
#include <string.h>

static int random_level(void)
{
    int lvl = 1;
    while ((rand() & 0xFFFF) < (int)(SKIPLIST_P * 0xFFFF) &&
           lvl < SKIPLIST_MAX_LEVEL)
        lvl++;
    return lvl;
}

static SkipNode *node_create(int level, PyObject *key)
{
    SkipNode *n = malloc(sizeof(SkipNode));
    if (!n) return NULL;

    n->forward = calloc(level, sizeof(SkipNode *));
    if (!n->forward) { free(n); return NULL; }

    n->level = level;
    if (key) {
        Py_INCREF(key);
        n->key = key;
    } else {
        n->key = NULL;
    }
    return n;
}

static void node_free(SkipNode *n)
{
    if (!n) return;
    if (n->key) Py_DECREF(n->key);
    free(n->forward);
    free(n);
}

SkipList *skiplist_create(void)
{
    SkipList *list = malloc(sizeof(SkipList));
    if (!list) return NULL;

    list->header = node_create(SKIPLIST_MAX_LEVEL, NULL);
    if (!list->header) { free(list); return NULL; }

    list->level = 1;
    list->size  = 0;
    return list;
}

void skiplist_free(SkipList *list)
{
    if (!list) return;
    SkipNode *cur = list->header->forward[0];
    while (cur) {
        SkipNode *next = cur->forward[0];
        node_free(cur);
        cur = next;
    }
    node_free(list->header);
    free(list);
}

int skiplist_contains(SkipList *list, PyObject *key)
{
    SkipNode *x = list->header;

    for (int i = list->level - 1; i >= 0; i--) {
        while (x->forward[i]) {
            int cmp = PyObject_RichCompareBool(x->forward[i]->key, key, Py_LT);
            if (cmp < 0) return -1;
            if (!cmp) break;
            x = x->forward[i];
        }
    }

    x = x->forward[0];
    if (x) {
        int eq = PyObject_RichCompareBool(x->key, key, Py_EQ);
        if (eq < 0) return -1;
        if (eq) return 1;
    }
    return 0;
}

int skiplist_insert(SkipList *list, PyObject *key)
{
    SkipNode *update[SKIPLIST_MAX_LEVEL];
    SkipNode *x = list->header;

    for (int i = list->level - 1; i >= 0; i--) {
        while (x->forward[i]) {
            int cmp = PyObject_RichCompareBool(x->forward[i]->key, key, Py_LT);
            if (cmp < 0) return -1;
            if (!cmp) break;
            x = x->forward[i];
        }
        update[i] = x;
    }

    x = x->forward[0];

    if (x) {
        int eq = PyObject_RichCompareBool(x->key, key, Py_EQ);
        if (eq < 0) return -1;
        if (eq) return 0;
    }

    int lvl = random_level();

    if (lvl > list->level) {
        for (int i = list->level; i < lvl; i++)
            update[i] = list->header;
        list->level = lvl;
    }

    SkipNode *new_node = node_create(lvl, key);
    if (!new_node) { PyErr_NoMemory(); return -1; }

    for (int i = 0; i < lvl; i++) {
        new_node->forward[i] = update[i]->forward[i];
        update[i]->forward[i] = new_node;
    }

    list->size++;
    return 1;
}

int skiplist_remove(SkipList *list, PyObject *key)
{
    SkipNode *update[SKIPLIST_MAX_LEVEL];
    SkipNode *x = list->header;

    for (int i = list->level - 1; i >= 0; i--) {
        while (x->forward[i]) {
            int cmp = PyObject_RichCompareBool(x->forward[i]->key, key, Py_LT);
            if (cmp < 0) return -1;
            if (!cmp) break;
            x = x->forward[i];
        }
        update[i] = x;
    }

    x = x->forward[0];
    if (!x) return 0;

    int eq = PyObject_RichCompareBool(x->key, key, Py_EQ);
    if (eq < 0) return -1;
    if (!eq) return 0;

    for (int i = 0; i < list->level; i++) {
        if (update[i]->forward[i] != x)
            break;
        update[i]->forward[i] = x->forward[i];
    }

    node_free(x);

    while (list->level > 1 && !list->header->forward[list->level - 1])
        list->level--;

    list->size--;
    return 1;
}

SkipNode *skiplist_first(SkipList *list)
{
    return list->header->forward[0];
}

SkipNode *skiplist_next(SkipNode *node)
{
    return node ? node->forward[0] : NULL;
}

PyObject *skiplist_peek_min(SkipList *list)
{
    SkipNode *first = list->header->forward[0];
    if (!first) return NULL;
    Py_INCREF(first->key);
    return first->key;
}

int skiplist_height(SkipList *list)
{
    return list->level;
}
