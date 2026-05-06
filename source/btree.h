#ifndef BTREE_H
#define BTREE_H

#include <Python.h>
#include <stddef.h>

/*
 * B-tree of order M (max M children per node, max M-1 keys).
 * Default M = 64 — good cache-line utilisation on modern CPUs.
 *
 * This is a plain B-tree (not B+), keys stored in every node.
 * Used as a sorted-set: keys only, no values.
 */

#ifndef BTREE_ORDER
#define BTREE_ORDER 64
#endif

#define BTREE_MIN_KEYS ((BTREE_ORDER + 1) / 2 - 1)
#define BTREE_MAX_KEYS (BTREE_ORDER - 1)

typedef struct BTreeNode
{
    PyObject **keys;
    struct BTreeNode **children;
    int n;
    int leaf;
} BTreeNode;

typedef struct
{
    BTreeNode *root;
    size_t size;
    int order;
} BTree;

typedef struct
{
    BTreeNode *node;
    int idx;
} BTreePos;

BTree *btree_create(int order);

void btree_free(BTree *tree);
int btree_insert(BTree *tree, PyObject *key);
int btree_remove(BTree *tree, PyObject *key);
int btree_contains(BTree *tree, PyObject *key);

BTreePos btree_first(BTree *tree);
BTreePos btree_next(BTree *tree, BTreePos pos);

int btree_height(BTree *tree);

#endif