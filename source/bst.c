#include "bst.h"
#include <stdlib.h>

static BSTNode *node_create(PyObject *key)
{
    BSTNode *n = malloc(sizeof(BSTNode));
    if (!n) return NULL;
    Py_INCREF(key);
    n->key = key;
    n->left = NULL;
    n->right = NULL;
    n->parent = NULL;
    return n;
}

static void node_free_recursive(BSTNode *node)
{
    if (!node) return;
    node_free_recursive(node->left);
    node_free_recursive(node->right);
    Py_DECREF(node->key);
    free(node);
}

static BSTNode *find_node(BST *tree, PyObject *key)
{
    BSTNode *x = tree->root;
    while (x) {
        int cmp = PyObject_RichCompareBool(key, x->key, Py_LT);
        if (cmp < 0) return (BSTNode *)(intptr_t)-1;
        if (cmp) { x = x->left; continue; }
        int eq = PyObject_RichCompareBool(key, x->key, Py_EQ);
        if (eq < 0) return (BSTNode *)(intptr_t)-1;
        if (eq) return x;
        x = x->right;
    }
    return NULL;
}

BST *bst_create(void)
{
    BST *tree = malloc(sizeof(BST));
    if (!tree) return NULL;
    tree->root = NULL;
    tree->size = 0;
    return tree;
}

void bst_free(BST *tree)
{
    if (!tree) return;
    node_free_recursive(tree->root);
    free(tree);
}

int bst_contains(BST *tree, PyObject *key)
{
    BSTNode *n = find_node(tree, key);
    if (n == (BSTNode *)(intptr_t)-1) return -1;
    return n ? 1 : 0;
}

BSTNode *bst_minimum(BST *tree, BSTNode *node)
{
    (void)tree;
    while (node && node->left)
        node = node->left;
    return node;
}

BSTNode *bst_successor(BST *tree, BSTNode *node)
{
    (void)tree;
    if (node->right)
        return bst_minimum(tree, node->right);
    BSTNode *y = node->parent;
    while (y && node == y->right) {
        node = y;
        y = y->parent;
    }
    return y;
}

static int node_height(BSTNode *node)
{
    if (!node) return 0;
    int lh = node_height(node->left);
    int rh = node_height(node->right);
    return 1 + (lh > rh ? lh : rh);
}

int bst_height(BST *tree)
{
    return node_height(tree->root);
}

int bst_insert(BST *tree, PyObject *key)
{
    BSTNode *y = NULL;
    BSTNode *x = tree->root;

    while (x) {
        y = x;
        int cmp = PyObject_RichCompareBool(key, x->key, Py_LT);
        if (cmp < 0) return -1;
        if (cmp) { x = x->left; continue; }
        int eq = PyObject_RichCompareBool(key, x->key, Py_EQ);
        if (eq < 0) return -1;
        if (eq) return 0;
        x = x->right;
    }

    BSTNode *z = node_create(key);
    if (!z) { PyErr_NoMemory(); return -1; }

    z->parent = y;
    if (!y) {
        tree->root = z;
    } else {
        int cmp = PyObject_RichCompareBool(key, y->key, Py_LT);
        if (cmp < 0) { Py_DECREF(z->key); free(z); return -1; }
        if (cmp) y->left = z;
        else     y->right = z;
    }

    tree->size++;
    return 1;
}

static void transplant(BST *tree, BSTNode *u, BSTNode *v)
{
    if (!u->parent)
        tree->root = v;
    else if (u == u->parent->left)
        u->parent->left = v;
    else
        u->parent->right = v;
    if (v) v->parent = u->parent;
}

int bst_remove(BST *tree, PyObject *key)
{
    BSTNode *z = find_node(tree, key);
    if (z == (BSTNode *)(intptr_t)-1) return -1;
    if (!z) return 0;

    if (!z->left) {
        transplant(tree, z, z->right);
    } else if (!z->right) {
        transplant(tree, z, z->left);
    } else {
        BSTNode *y = bst_minimum(tree, z->right);
        if (y->parent != z) {
            transplant(tree, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        transplant(tree, z, y);
        y->left = z->left;
        y->left->parent = y;
    }

    Py_DECREF(z->key);
    free(z);
    tree->size--;
    return 1;
}