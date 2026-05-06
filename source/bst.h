#ifndef BST_H
#define BST_H

#include <Python.h>
#include <stddef.h>

typedef struct BSTNode
{
    PyObject *key;
    struct BSTNode *left;
    struct BSTNode *right;
    struct BSTNode *parent;
} BSTNode;

typedef struct
{
    BSTNode *root;
    size_t size;
} BST;

BST *bst_create(void);

void bst_free(BST *tree);
int bst_insert(BST *tree, PyObject *key);
int bst_remove(BST *tree, PyObject *key);
int bst_contains(BST *tree, PyObject *key);

BSTNode *bst_minimum(BST *tree, BSTNode *node);
BSTNode *bst_successor(BST *tree, BSTNode *node);

int bst_height(BST *tree);

#endif