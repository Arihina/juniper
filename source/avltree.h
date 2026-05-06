#ifndef AVLTREE_H
#define AVLTREE_H

#include <Python.h>
#include <stddef.h>

typedef struct AVLNode
{
    PyObject *key;
    int height;
    struct AVLNode *left;
    struct AVLNode *right;
    struct AVLNode *parent;
} AVLNode;

typedef struct
{
    AVLNode *root;
    AVLNode *nil;
    size_t size;
} AVLTree;

AVLTree *avltree_create(void);

int avltree_insert(AVLTree *tree, PyObject *key);
int avltree_remove(AVLTree *tree, PyObject *key);
int avltree_contains(AVLTree *tree, PyObject *key);

AVLNode *avltree_minimum(AVLTree *tree, AVLNode *node);
AVLNode *avltree_successor(AVLTree *tree, AVLNode *node);

void avltree_free(AVLTree *tree);

#endif