#include "avltree.h"
#include <stdlib.h>

static int height(AVLTree *tree, AVLNode *n)
{
    return (n == tree->nil) ? 0 : n->height;
}

static int balance_factor(AVLTree *tree, AVLNode *n)
{
    if (n == tree->nil) return 0;
    return height(tree, n->left) - height(tree, n->right);
}

static void update_height(AVLTree *tree, AVLNode *n)
{
    int lh = height(tree, n->left);
    int rh = height(tree, n->right);
    n->height = 1 + (lh > rh ? lh : rh);
}

static AVLNode *node_create(AVLTree *tree, PyObject *key)
{
    AVLNode *n = malloc(sizeof(AVLNode));
    if (!n) return NULL;

    Py_INCREF(key);
    n->key    = key;
    n->height = 1;
    n->left   = tree->nil;
    n->right  = tree->nil;
    n->parent = tree->nil;
    return n;
}

static void node_free_recursive(AVLTree *tree, AVLNode *node)
{
    if (node == tree->nil) return;
    node_free_recursive(tree, node->left);
    node_free_recursive(tree, node->right);
    Py_DECREF(node->key);
    free(node);
}

static AVLNode *rotate_right(AVLTree *tree, AVLNode *y)
{
    AVLNode *x = y->left;
    AVLNode *T = x->right;

    x->right = y;
    y->left  = T;

    x->parent = y->parent;
    y->parent = x;
    if (T != tree->nil)
        T->parent = y;

    if (x->parent == tree->nil)
        tree->root = x;
    else if (x->parent->left == y)
        x->parent->left = x;
    else
        x->parent->right = x;

    update_height(tree, y);
    update_height(tree, x);
    return x;
}

static AVLNode *rotate_left(AVLTree *tree, AVLNode *x)
{
    AVLNode *y = x->right;
    AVLNode *T = y->left;

    y->left  = x;
    x->right = T;

    y->parent = x->parent;
    x->parent = y;
    if (T != tree->nil)
        T->parent = x;

    if (y->parent == tree->nil)
        tree->root = y;
    else if (y->parent->left == x)
        y->parent->left = y;
    else
        y->parent->right = y;

    update_height(tree, x);
    update_height(tree, y);
    return y;
}

static AVLNode *rebalance(AVLTree *tree, AVLNode *n)
{
    update_height(tree, n);
    int bf = balance_factor(tree, n);

    if (bf > 1) {
        if (balance_factor(tree, n->left) < 0)
            rotate_left(tree, n->left);
        return rotate_right(tree, n);
    }

    if (bf < -1) {
        if (balance_factor(tree, n->right) > 0)
            rotate_right(tree, n->right);
        return rotate_left(tree, n);
    }

    return n;
}

static void rebalance_up(AVLTree *tree, AVLNode *n)
{
    while (n != tree->nil) {
        AVLNode *p = n->parent;
        rebalance(tree, n);
        n = p;
    }
}

AVLTree *avltree_create(void)
{
    AVLTree *tree = malloc(sizeof(AVLTree));
    if (!tree) return NULL;

    tree->nil = malloc(sizeof(AVLNode));
    if (!tree->nil) {
        free(tree);
        return NULL;
    }

    tree->nil->height = 0;
    tree->nil->key    = NULL;
    tree->nil->left   = NULL;
    tree->nil->right  = NULL;
    tree->nil->parent = NULL;

    tree->root = tree->nil;
    tree->size = 0;
    return tree;
}

void avltree_free(AVLTree *tree)
{
    if (!tree) return;
    node_free_recursive(tree, tree->root);
    free(tree->nil);
    free(tree);
}

static AVLNode *find_node(AVLTree *tree, PyObject *key)
{
    AVLNode *x = tree->root;
    while (x != tree->nil) {
        int cmp = PyObject_RichCompareBool(key, x->key, Py_LT);
        if (cmp < 0) return NULL;
        if (cmp) {
            x = x->left;
        } else {
            int eq = PyObject_RichCompareBool(key, x->key, Py_EQ);
            if (eq < 0) return NULL;
            if (eq) return x;
            x = x->right;
        }
    }
    return tree->nil;
}

int avltree_contains(AVLTree *tree, PyObject *key)
{
    AVLNode *n = find_node(tree, key);
    if (!n) return -1;
    return (n != tree->nil) ? 1 : 0;
}

AVLNode *avltree_minimum(AVLTree *tree, AVLNode *node)
{
    while (node->left != tree->nil)
        node = node->left;
    return node;
}

AVLNode *avltree_successor(AVLTree *tree, AVLNode *node)
{
    if (node->right != tree->nil)
        return avltree_minimum(tree, node->right);
    AVLNode *y = node->parent;
    while (y != tree->nil && node == y->right) {
        node = y;
        y = y->parent;
    }
    return y;
}

int avltree_insert(AVLTree *tree, PyObject *key)
{
    AVLNode *y = tree->nil;
    AVLNode *x = tree->root;

    while (x != tree->nil) {
        y = x;
        int cmp = PyObject_RichCompareBool(key, x->key, Py_LT);
        if (cmp < 0) return -1;
        if (cmp) {
            x = x->left;
        } else {
            int eq = PyObject_RichCompareBool(key, x->key, Py_EQ);
            if (eq < 0) return -1;
            if (eq) return 0;
            x = x->right;
        }
    }

    AVLNode *z = node_create(tree, key);
    if (!z) {
        PyErr_NoMemory();
        return -1;
    }

    z->parent = y;
    if (y == tree->nil) {
        tree->root = z;
    } else {
        int cmp = PyObject_RichCompareBool(key, y->key, Py_LT);
        if (cmp < 0) {
            Py_DECREF(z->key);
            free(z);
            return -1;
        }
        if (cmp)
            y->left = z;
        else
            y->right = z;
    }

    tree->size++;
    rebalance_up(tree, z->parent);
    return 1;
}

static void transplant(AVLTree *tree, AVLNode *u, AVLNode *v)
{
    if (u->parent == tree->nil)
        tree->root = v;
    else if (u == u->parent->left)
        u->parent->left = v;
    else
        u->parent->right = v;
    v->parent = u->parent;
}

int avltree_remove(AVLTree *tree, PyObject *key)
{
    AVLNode *z = find_node(tree, key);
    if (!z) return -1;
    if (z == tree->nil) return 0;

    AVLNode *rebalance_start;

    if (z->left == tree->nil) {
        rebalance_start = z->parent;
        transplant(tree, z, z->right);
    } else if (z->right == tree->nil) {
        rebalance_start = z->parent;
        transplant(tree, z, z->left);
    } else {
        AVLNode *y = avltree_minimum(tree, z->right);
        rebalance_start = (y->parent == z) ? y : y->parent;

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

    rebalance_up(tree, rebalance_start);
    return 1;
}
