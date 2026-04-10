#ifndef RBTREE_H
#define RBTREE_H

#include <Python.h>
#include <stddef.h>

typedef enum { RB_BLACK, RB_RED } RBColor;

typedef struct RBNode
{
    PyObject *key;
    RBColor color;
    struct RBNode *left;
    struct RBNode *right;
    struct RBNode *parent;
} RBNode;

typedef struct
{
    RBNode *root;
    RBNode *nil;
    size_t size;
} RBTree;

RBTree *rbtree_create(void);

int rbtree_insert(RBTree *tree, PyObject *key);

int rbtree_remove(RBTree *tree, PyObject *key);

int rbtree_contains(RBTree *tree, PyObject *key);

RBNode *rbtree_minimum(RBTree *tree, RBNode *node);

RBNode *rbtree_successor(RBTree *tree, RBNode *node);

void rbtree_free(RBTree *tree);

#endif
