#include "rbtree.h"
#include <stdlib.h>

static RBNode *node_create(RBTree *tree, PyObject *key)
{
    RBNode *n = malloc(sizeof(RBNode));
    if (!n)
        return NULL;

    Py_INCREF(key);
    n->key    = key;
    n->color  = RB_RED;
    n->left   = tree->nil;
    n->right  = tree->nil;
    n->parent = tree->nil;
    return n;
}

static void node_free_recursive(RBTree *tree, RBNode *node)
{
    if (node == tree->nil)
        return;
    node_free_recursive(tree, node->left);
    node_free_recursive(tree, node->right);
    Py_DECREF(node->key);
    free(node);
}

static void rotate_left(RBTree *tree, RBNode *x)
{
    RBNode *y = x->right;
    x->right = y->left;
    if (y->left != tree->nil)
        y->left->parent = x;
    y->parent = x->parent;
    if (x->parent == tree->nil)
        tree->root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;
    y->left = x;
    x->parent = y;
}

static void rotate_right(RBTree *tree, RBNode *x)
{
    RBNode *y = x->left;
    x->left = y->right;
    if (y->right != tree->nil)
        y->right->parent = x;
    y->parent = x->parent;
    if (x->parent == tree->nil)
        tree->root = y;
    else if (x == x->parent->right)
        x->parent->right = y;
    else
        x->parent->left = y;
    y->right = x;
    x->parent = y;
}

RBTree *rbtree_create(void)
{
    RBTree *tree = malloc(sizeof(RBTree));
    if (!tree)
        return NULL;

    tree->nil = malloc(sizeof(RBNode));
    if (!tree->nil) {
        free(tree);
        return NULL;
    }

    tree->nil->color  = RB_BLACK;
    tree->nil->key    = NULL;
    tree->nil->left   = NULL;
    tree->nil->right  = NULL;
    tree->nil->parent = NULL;

    tree->root = tree->nil;
    tree->size = 0;
    return tree;
}

void rbtree_free(RBTree *tree)
{
    if (!tree) return;
    node_free_recursive(tree, tree->root);
    free(tree->nil);
    free(tree);
}

static void insert_fixup(RBTree *tree, RBNode *z)
{
    while (z->parent->color == RB_RED) {
        if (z->parent == z->parent->parent->left) {
            RBNode *y = z->parent->parent->right;
            if (y->color == RB_RED) {
                z->parent->color = RB_BLACK;
                y->color = RB_BLACK;
                z->parent->parent->color = RB_RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    rotate_left(tree, z);
                }
                z->parent->color = RB_BLACK;
                z->parent->parent->color = RB_RED;
                rotate_right(tree, z->parent->parent);
            }
        } else {
            RBNode *y = z->parent->parent->left;
            if (y->color == RB_RED) {
                z->parent->color = RB_BLACK;
                y->color = RB_BLACK;
                z->parent->parent->color = RB_RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    rotate_right(tree, z);
                }
                z->parent->color = RB_BLACK;
                z->parent->parent->color = RB_RED;
                rotate_left(tree, z->parent->parent);
            }
        }
    }
    tree->root->color = RB_BLACK;
}

int rbtree_insert(RBTree *tree, PyObject *key)
{
    RBNode *y = tree->nil;
    RBNode *x = tree->root;

    while (x != tree->nil) {
        y = x;
        int cmp = PyObject_RichCompareBool(key, x->key, Py_LT);
        if (cmp < 0) return -1;  /* comparison error */
        if (cmp) {
            x = x->left;
        } else {
            /* check equality */
            int eq = PyObject_RichCompareBool(key, x->key, Py_EQ);
            if (eq < 0) return -1;
            if (eq) return 0;  /* duplicate */
            x = x->right;
        }
    }

    RBNode *z = node_create(tree, key);
    if (!z) {
        PyErr_NoMemory();
        return -1;
    }

    z->parent = y;
    if (y == tree->nil)
        tree->root = z;
    else {
        int cmp = PyObject_RichCompareBool(key, y->key, Py_LT);
        if (cmp < 0) {
            Py_DECREF(key);
            free(z);
            return -1;
        }
        if (cmp)
            y->left = z;
        else
            y->right = z;
    }

    insert_fixup(tree, z);
    tree->size++;
    return 1;
}

int rbtree_contains(RBTree *tree, PyObject *key)
{
    RBNode *x = tree->root;
    while (x != tree->nil) {
        int cmp = PyObject_RichCompareBool(key, x->key, Py_LT);
        if (cmp < 0) return -1;
        if (cmp) {
            x = x->left;
        } else {
            int eq = PyObject_RichCompareBool(key, x->key, Py_EQ);
            if (eq < 0) return -1;
            if (eq) return 1;
            x = x->right;
        }
    }
    return 0;
}

RBNode *rbtree_minimum(RBTree *tree, RBNode *node)
{
    while (node->left != tree->nil)
        node = node->left;
    return node;
}

RBNode *rbtree_successor(RBTree *tree, RBNode *node)
{
    if (node->right != tree->nil)
        return rbtree_minimum(tree, node->right);
    RBNode *y = node->parent;
    while (y != tree->nil && node == y->right) {
        node = y;
        y = y->parent;
    }
    return y;
}

static void transplant(RBTree *tree, RBNode *u, RBNode *v)
{
    if (u->parent == tree->nil)
        tree->root = v;
    else if (u == u->parent->left)
        u->parent->left = v;
    else
        u->parent->right = v;
    v->parent = u->parent;
}

static void delete_fixup(RBTree *tree, RBNode *x)
{
    while (x != tree->root && x->color == RB_BLACK) {
        if (x == x->parent->left) {
            RBNode *w = x->parent->right;
            if (w->color == RB_RED) {
                w->color = RB_BLACK;
                x->parent->color = RB_RED;
                rotate_left(tree, x->parent);
                w = x->parent->right;
            }
            if (w->left->color == RB_BLACK && w->right->color == RB_BLACK) {
                w->color = RB_RED;
                x = x->parent;
            } else {
                if (w->right->color == RB_BLACK) {
                    w->left->color = RB_BLACK;
                    w->color = RB_RED;
                    rotate_right(tree, w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = RB_BLACK;
                w->right->color = RB_BLACK;
                rotate_left(tree, x->parent);
                x = tree->root;
            }
        } else {
            RBNode *w = x->parent->left;
            if (w->color == RB_RED) {
                w->color = RB_BLACK;
                x->parent->color = RB_RED;
                rotate_right(tree, x->parent);
                w = x->parent->left;
            }
            if (w->right->color == RB_BLACK && w->left->color == RB_BLACK) {
                w->color = RB_RED;
                x = x->parent;
            } else {
                if (w->left->color == RB_BLACK) {
                    w->right->color = RB_BLACK;
                    w->color = RB_RED;
                    rotate_left(tree, w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = RB_BLACK;
                w->left->color = RB_BLACK;
                rotate_right(tree, x->parent);
                x = tree->root;
            }
        }
    }
    x->color = RB_BLACK;
}

static RBNode *find_node(RBTree *tree, PyObject *key)
{
    RBNode *x = tree->root;
    while (x != tree->nil) {
        int cmp = PyObject_RichCompareBool(key, x->key, Py_LT);
        if (cmp < 0) return NULL;  /* error */
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

int rbtree_remove(RBTree *tree, PyObject *key)
{
    RBNode *z = find_node(tree, key);
    if (!z) return -1;
    if (z == tree->nil) return 0;

    RBNode *y = z;
    RBNode *x;
    RBColor y_orig_color = y->color;

    if (z->left == tree->nil) {
        x = z->right;
        transplant(tree, z, z->right);
    } else if (z->right == tree->nil) {
        x = z->left;
        transplant(tree, z, z->left);
    } else {
        y = rbtree_minimum(tree, z->right);
        y_orig_color = y->color;
        x = y->right;
        if (y->parent == z) {
            x->parent = y;
        } else {
            transplant(tree, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        transplant(tree, z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }

    Py_DECREF(z->key);
    free(z);
    tree->size--;

    if (y_orig_color == RB_BLACK)
        delete_fixup(tree, x);

    return 1;
}
