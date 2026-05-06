#include "btree.h"
#include <stdlib.h>
#include <string.h>

static BTreeNode *node_create(int order, int leaf)
{
    BTreeNode *n = malloc(sizeof(BTreeNode));
    if (!n)
        return NULL;

    n->keys = malloc(sizeof(PyObject *) * (order - 1));
    if (!n->keys)
    {
        free(n);
        return NULL;
    }

    n->children = malloc(sizeof(BTreeNode *) * order);
    if (!n->children)
    {
        free(n->keys);
        free(n);
        return NULL;
    }

    n->n = 0;
    n->leaf = leaf;
    memset(n->children, 0, sizeof(BTreeNode *) * order);
    return n;
}

static void node_free_recursive(BTreeNode *node)
{
    if (!node)
        return;
    for (int i = 0; i < node->n; i++)
        Py_DECREF(node->keys[i]);
    if (!node->leaf)
    {
        for (int i = 0; i <= node->n; i++)
            node_free_recursive(node->children[i]);
    }
    free(node->keys);
    free(node->children);
    free(node);
}

static int node_search(BTreeNode *node, PyObject *key, int *found)
{
    int lo = 0, hi = node->n;
    *found = 0;
    while (lo < hi)
    {
        int mid = (lo + hi) / 2;
        int cmp = PyObject_RichCompareBool(key, node->keys[mid], Py_LT);
        if (cmp < 0)
            return -1;
        if (cmp)
        {
            hi = mid;
        }
        else
        {
            int eq = PyObject_RichCompareBool(key, node->keys[mid], Py_EQ);
            if (eq < 0)
                return -1;
            if (eq)
            {
                *found = 1;
                return mid;
            }
            lo = mid + 1;
        }
    }
    return lo;
}

BTree *btree_create(int order)
{
    if (order < 3)
        order = 3;

    BTree *tree = malloc(sizeof(BTree));
    if (!tree)
        return NULL;

    tree->root = node_create(order, 1);
    if (!tree->root)
    {
        free(tree);
        return NULL;
    }

    tree->size = 0;
    tree->order = order;
    return tree;
}

void btree_free(BTree *tree)
{
    if (!tree)
        return;
    node_free_recursive(tree->root);
    free(tree);
}

int btree_contains(BTree *tree, PyObject *key)
{
    BTreeNode *x = tree->root;
    while (x)
    {
        int found;
        int i = node_search(x, key, &found);
        if (i < 0)
            return -1;
        if (found)
            return 1;
        if (x->leaf)
            return 0;
        x = x->children[i];
    }
    return 0;
}

static int split_child(BTree *tree, BTreeNode *x, int i)
{
    int order = tree->order;
    BTreeNode *y = x->children[i];
    int t = (order - 1) / 2;

    BTreeNode *z = node_create(order, y->leaf);
    if (!z)
        return -1;

    z->n = y->n - t - 1;
    for (int j = 0; j < z->n; j++)
    {
        z->keys[j] = y->keys[t + 1 + j];
    }
    if (!y->leaf)
    {
        for (int j = 0; j <= z->n; j++)
            z->children[j] = y->children[t + 1 + j];
    }

    PyObject *median = y->keys[t];

    y->n = t;

    for (int j = x->n; j > i; j--)
        x->children[j + 1] = x->children[j];
    x->children[i + 1] = z;

    for (int j = x->n - 1; j >= i; j--)
        x->keys[j + 1] = x->keys[j];
    x->keys[i] = median;
    x->n++;

    return 0;
}

static int insert_nonfull(BTree *tree, BTreeNode *x, PyObject *key)
{
    int found;
    int i = node_search(x, key, &found);
    if (i < 0)
        return -1;
    if (found)
        return 0;

    if (x->leaf)
    {
        for (int j = x->n - 1; j >= i; j--)
            x->keys[j + 1] = x->keys[j];
        Py_INCREF(key);
        x->keys[i] = key;
        x->n++;
        return 1;
    }

    if (x->children[i]->n == tree->order - 1)
    {
        if (split_child(tree, x, i) < 0)
            return -1;
        int cmp = PyObject_RichCompareBool(key, x->keys[i], Py_LT);
        if (cmp < 0)
            return -1;
        if (!cmp)
        {
            int eq = PyObject_RichCompareBool(key, x->keys[i], Py_EQ);
            if (eq < 0)
                return -1;
            if (eq)
                return 0;
            i++;
        }
    }
    return insert_nonfull(tree, x->children[i], key);
}

int btree_insert(BTree *tree, PyObject *key)
{
    BTreeNode *r = tree->root;

    if (r->n == tree->order - 1)
    {
        BTreeNode *s = node_create(tree->order, 0);
        if (!s)
        {
            PyErr_NoMemory();
            return -1;
        }
        s->children[0] = r;
        tree->root = s;
        if (split_child(tree, s, 0) < 0)
            return -1;
        int rc = insert_nonfull(tree, s, key);
        if (rc > 0)
            tree->size++;
        return rc;
    }

    int rc = insert_nonfull(tree, r, key);
    if (rc > 0)
        tree->size++;
    return rc;
}

static int delete_from_node(BTree *tree, BTreeNode *x, PyObject *key);

static PyObject *get_predecessor(BTreeNode *x)
{
    while (!x->leaf)
        x = x->children[x->n];
    return x->keys[x->n - 1];
}

static PyObject *get_successor(BTreeNode *x)
{
    while (!x->leaf)
        x = x->children[0];
    return x->keys[0];
}

static void merge_children(BTree *tree, BTreeNode *x, int i)
{
    BTreeNode *left = x->children[i];
    BTreeNode *right = x->children[i + 1];
    int t = (tree->order - 1) / 2;

    left->keys[t] = x->keys[i];

    for (int j = 0; j < right->n; j++)
        left->keys[t + 1 + j] = right->keys[j];
    if (!left->leaf)
    {
        for (int j = 0; j <= right->n; j++)
            left->children[t + 1 + j] = right->children[j];
    }
    left->n = t + 1 + right->n;

    for (int j = i; j < x->n - 1; j++)
        x->keys[j] = x->keys[j + 1];
    for (int j = i + 1; j < x->n; j++)
        x->children[j] = x->children[j + 1];
    x->n--;

    free(right->keys);
    free(right->children);
    free(right);
}

static void ensure_min_keys(BTree *tree, BTreeNode *x, int i)
{
    int t = (tree->order - 1) / 2;

    if (i > 0 && x->children[i - 1]->n > t)
    {
        BTreeNode *child = x->children[i];
        BTreeNode *lsib = x->children[i - 1];

        for (int j = child->n - 1; j >= 0; j--)
            child->keys[j + 1] = child->keys[j];
        if (!child->leaf)
        {
            for (int j = child->n; j >= 0; j--)
                child->children[j + 1] = child->children[j];
        }

        child->keys[0] = x->keys[i - 1];
        x->keys[i - 1] = lsib->keys[lsib->n - 1];
        if (!child->leaf)
            child->children[0] = lsib->children[lsib->n];

        child->n++;
        lsib->n--;
        return;
    }

    if (i < x->n && x->children[i + 1]->n > t)
    {
        BTreeNode *child = x->children[i];
        BTreeNode *rsib = x->children[i + 1];

        child->keys[child->n] = x->keys[i];
        x->keys[i] = rsib->keys[0];
        if (!child->leaf)
            child->children[child->n + 1] = rsib->children[0];

        for (int j = 0; j < rsib->n - 1; j++)
            rsib->keys[j] = rsib->keys[j + 1];
        if (!rsib->leaf)
        {
            for (int j = 0; j < rsib->n; j++)
                rsib->children[j] = rsib->children[j + 1];
        }

        child->n++;
        rsib->n--;
        return;
    }

    if (i < x->n)
        merge_children(tree, x, i);
    else
        merge_children(tree, x, i - 1);
}

static int delete_from_node(BTree *tree, BTreeNode *x, PyObject *key)
{
    int found;
    int i = node_search(x, key, &found);
    if (i < 0)
        return -1;

    int t = (tree->order - 1) / 2;

    if (found)
    {
        if (x->leaf)
        {
            Py_DECREF(x->keys[i]);
            for (int j = i; j < x->n - 1; j++)
                x->keys[j] = x->keys[j + 1];
            x->n--;
            return 1;
        }

        if (x->children[i]->n > t)
        {
            PyObject *pred = get_predecessor(x->children[i]);
            Py_INCREF(pred);
            int rc = delete_from_node(tree, x->children[i], pred);
            if (rc < 0)
            {
                Py_DECREF(pred);
                return -1;
            }
            Py_DECREF(x->keys[i]);
            x->keys[i] = pred;
            return 1;
        }

        if (x->children[i + 1]->n > t)
        {
            PyObject *succ = get_successor(x->children[i + 1]);
            Py_INCREF(succ);
            int rc = delete_from_node(tree, x->children[i + 1], succ);
            if (rc < 0)
            {
                Py_DECREF(succ);
                return -1;
            }
            Py_DECREF(x->keys[i]);
            x->keys[i] = succ;
            return 1;
        }

        merge_children(tree, x, i);
        return delete_from_node(tree, x->children[i], key);
    }

    if (x->leaf)
        return 0;

    int descend = i;
    if (x->children[descend]->n <= t)
        ensure_min_keys(tree, x, descend);

    if (descend > x->n)
        descend = x->n;

    return delete_from_node(tree, x->children[descend], key);
}

int btree_remove(BTree *tree, PyObject *key)
{
    if (tree->root->n == 0)
        return 0;

    int rc = delete_from_node(tree, tree->root, key);
    if (rc < 0)
        return -1;

    if (tree->root->n == 0 && !tree->root->leaf)
    {
        BTreeNode *old_root = tree->root;
        tree->root = old_root->children[0];
        free(old_root->keys);
        free(old_root->children);
        free(old_root);
    }

    if (rc > 0)
        tree->size--;
    return rc;
}

BTreePos btree_first(BTree *tree)
{
    BTreePos pos = {NULL, 0};
    if (!tree->root || tree->root->n == 0)
        return pos;

    BTreeNode *x = tree->root;
    while (!x->leaf)
        x = x->children[0];
    pos.node = x;
    pos.idx = 0;
    return pos;
}

BTreePos btree_next(BTree *tree, BTreePos pos)
{
    BTreePos result = {NULL, 0};
    if (!pos.node)
        return result;

    BTreeNode *cur = pos.node;
    int ci = pos.idx;

    if (!cur->leaf)
    {
        BTreeNode *x = cur->children[ci + 1];
        while (!x->leaf)
            x = x->children[0];
        result.node = x;
        result.idx = 0;
        return result;
    }

    if (ci + 1 < cur->n)
    {
        result.node = cur;
        result.idx = ci + 1;
        return result;
    }

    PyObject *cur_key = cur->keys[ci];
    BTreeNode *x = tree->root;
    BTreeNode *ancestor = NULL;
    int ancestor_idx = 0;

    while (x)
    {
        int found;
        int i = node_search(x, cur_key, &found);
        if (i < 0)
            return result;

        if (found)
        {
            if (!x->leaf)
            {
                BTreeNode *s = x->children[i + 1];
                while (!s->leaf)
                    s = s->children[0];
                result.node = s;
                result.idx = 0;
                return result;
            }

            if (i + 1 < x->n)
            {
                result.node = x;
                result.idx = i + 1;
                return result;
            }
            if (ancestor)
            {
                result.node = ancestor;
                result.idx = ancestor_idx;
            }
            return result;
        }

        if (i < x->n)
        {
            ancestor = x;
            ancestor_idx = i;
        }

        if (x->leaf)
            break;
        x = x->children[i];
    }

    if (ancestor)
    {
        result.node = ancestor;
        result.idx = ancestor_idx;
    }
    return result;
}

int btree_height(BTree *tree)
{
    if (!tree->root || tree->root->n == 0)
        return 0;
    int h = 1;
    BTreeNode *x = tree->root;
    while (!x->leaf)
    {
        x = x->children[0];
        h++;
    }
    return h;
}
