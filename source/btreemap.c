#include "btreemap.h"
#include <stdlib.h>
#include <string.h>

static BTMNode *node_create(int order, int leaf)
{
    BTMNode *n = malloc(sizeof(BTMNode));
    if (!n) return NULL;

    n->keys     = malloc(sizeof(PyObject *) * (order - 1));
    n->values   = malloc(sizeof(PyObject *) * (order - 1));
    n->children = malloc(sizeof(BTMNode *)  * order);

    if (!n->keys || !n->values || !n->children) {
        free(n->keys); free(n->values); free(n->children);
        free(n);
        return NULL;
    }

    n->n    = 0;
    n->leaf = leaf;
    memset(n->children, 0, sizeof(BTMNode *) * order);
    return n;
}

static void node_free_recursive(BTMNode *node)
{
    if (!node) return;
    for (int i = 0; i < node->n; i++) {
        Py_DECREF(node->keys[i]);
        Py_DECREF(node->values[i]);
    }
    if (!node->leaf) {
        for (int i = 0; i <= node->n; i++)
            node_free_recursive(node->children[i]);
    }
    free(node->keys);
    free(node->values);
    free(node->children);
    free(node);
}

static int node_search(BTMNode *node, PyObject *key, int *found)
{
    int lo = 0, hi = node->n;
    *found = 0;
    while (lo < hi) {
        int mid = (lo + hi) / 2;
        int cmp = PyObject_RichCompareBool(key, node->keys[mid], Py_LT);
        if (cmp < 0) return -1;
        if (cmp) {
            hi = mid;
        } else {
            int eq = PyObject_RichCompareBool(key, node->keys[mid], Py_EQ);
            if (eq < 0) return -1;
            if (eq) { *found = 1; return mid; }
            lo = mid + 1;
        }
    }
    return lo;
}

BTreeMap *btm_create(int order)
{
    if (order < 3) order = 3;
    BTreeMap *map = malloc(sizeof(BTreeMap));
    if (!map) return NULL;

    map->root = node_create(order, 1);
    if (!map->root) { free(map); return NULL; }

    map->size  = 0;
    map->order = order;
    return map;
}

void btm_free(BTreeMap *map)
{
    if (!map) return;
    node_free_recursive(map->root);
    free(map);
}

int btm_contains(BTreeMap *map, PyObject *key)
{
    BTMNode *x = map->root;
    while (x) {
        int found;
        int i = node_search(x, key, &found);
        if (i < 0) return -1;
        if (found) return 1;
        if (x->leaf) return 0;
        x = x->children[i];
    }
    return 0;
}

PyObject *btm_get(BTreeMap *map, PyObject *key)
{
    BTMNode *x = map->root;
    while (x) {
        int found;
        int i = node_search(x, key, &found);
        if (i < 0) return NULL;
        if (found) {
            Py_INCREF(x->values[i]);
            return x->values[i];
        }
        if (x->leaf) return NULL;
        x = x->children[i];
    }
    return NULL;
}

static int split_child(BTreeMap *map, BTMNode *x, int i)
{
    int order = map->order;
    BTMNode *y = x->children[i];
    int t = (order - 1) / 2;

    BTMNode *z = node_create(order, y->leaf);
    if (!z) return -1;

    z->n = y->n - t - 1;
    for (int j = 0; j < z->n; j++) {
        z->keys[j]   = y->keys[t + 1 + j];
        z->values[j] = y->values[t + 1 + j];
    }
    if (!y->leaf) {
        for (int j = 0; j <= z->n; j++)
            z->children[j] = y->children[t + 1 + j];
    }

    PyObject *med_key = y->keys[t];
    PyObject *med_val = y->values[t];
    y->n = t;

    for (int j = x->n; j > i; j--)
        x->children[j + 1] = x->children[j];
    x->children[i + 1] = z;

    for (int j = x->n - 1; j >= i; j--) {
        x->keys[j + 1]   = x->keys[j];
        x->values[j + 1] = x->values[j];
    }
    x->keys[i]   = med_key;
    x->values[i] = med_val;
    x->n++;
    return 0;
}

static int insert_nonfull(BTreeMap *map, BTMNode *x,
                          PyObject *key, PyObject *value)
{
    int found;
    int i = node_search(x, key, &found);
    if (i < 0) return -1;

    if (found) {
        Py_INCREF(value);
        Py_DECREF(x->values[i]);
        x->values[i] = value;
        return 1;
    }

    if (x->leaf) {
        for (int j = x->n - 1; j >= i; j--) {
            x->keys[j + 1]   = x->keys[j];
            x->values[j + 1] = x->values[j];
        }
        Py_INCREF(key);
        Py_INCREF(value);
        x->keys[i]   = key;
        x->values[i] = value;
        x->n++;
        return 0;
    }

    if (x->children[i]->n == map->order - 1) {
        if (split_child(map, x, i) < 0) return -1;
        int cmp = PyObject_RichCompareBool(key, x->keys[i], Py_LT);
        if (cmp < 0) return -1;
        if (!cmp) {
            int eq = PyObject_RichCompareBool(key, x->keys[i], Py_EQ);
            if (eq < 0) return -1;
            if (eq) {
                Py_INCREF(value);
                Py_DECREF(x->values[i]);
                x->values[i] = value;
                return 1;
            }
            i++;
        }
    }
    return insert_nonfull(map, x->children[i], key, value);
}

int btm_put(BTreeMap *map, PyObject *key, PyObject *value)
{
    BTMNode *r = map->root;

    if (r->n == map->order - 1) {
        BTMNode *s = node_create(map->order, 0);
        if (!s) { PyErr_NoMemory(); return -1; }
        s->children[0] = r;
        map->root = s;
        if (split_child(map, s, 0) < 0) return -1;
        int rc = insert_nonfull(map, s, key, value);
        if (rc == 0) map->size++;
        return rc;
    }

    int rc = insert_nonfull(map, r, key, value);
    if (rc == 0) map->size++;
    return rc;
}

static PyObject *get_pred_key(BTMNode *x)
{
    while (!x->leaf) x = x->children[x->n];
    return x->keys[x->n - 1];
}
static PyObject *get_pred_val(BTMNode *x)
{
    while (!x->leaf) x = x->children[x->n];
    return x->values[x->n - 1];
}
static PyObject *get_succ_key(BTMNode *x)
{
    while (!x->leaf) x = x->children[0];
    return x->keys[0];
}
static PyObject *get_succ_val(BTMNode *x)
{
    while (!x->leaf) x = x->children[0];
    return x->values[0];
}

static void merge_children(BTreeMap *map, BTMNode *x, int i)
{
    BTMNode *left  = x->children[i];
    BTMNode *right = x->children[i + 1];
    int t = (map->order - 1) / 2;

    left->keys[t]   = x->keys[i];
    left->values[t]  = x->values[i];

    for (int j = 0; j < right->n; j++) {
        left->keys[t + 1 + j]   = right->keys[j];
        left->values[t + 1 + j] = right->values[j];
    }
    if (!left->leaf) {
        for (int j = 0; j <= right->n; j++)
            left->children[t + 1 + j] = right->children[j];
    }
    left->n = t + 1 + right->n;

    for (int j = i; j < x->n - 1; j++) {
        x->keys[j]   = x->keys[j + 1];
        x->values[j] = x->values[j + 1];
    }
    for (int j = i + 1; j < x->n; j++)
        x->children[j] = x->children[j + 1];
    x->n--;

    free(right->keys);
    free(right->values);
    free(right->children);
    free(right);
}

static void ensure_min(BTreeMap *map, BTMNode *x, int i)
{
    int t = (map->order - 1) / 2;

    if (i > 0 && x->children[i - 1]->n > t) {
        BTMNode *child = x->children[i];
        BTMNode *lsib  = x->children[i - 1];

        for (int j = child->n - 1; j >= 0; j--) {
            child->keys[j + 1]   = child->keys[j];
            child->values[j + 1] = child->values[j];
        }
        if (!child->leaf) {
            for (int j = child->n; j >= 0; j--)
                child->children[j + 1] = child->children[j];
        }
        child->keys[0]   = x->keys[i - 1];
        child->values[0] = x->values[i - 1];
        x->keys[i - 1]   = lsib->keys[lsib->n - 1];
        x->values[i - 1] = lsib->values[lsib->n - 1];
        if (!child->leaf)
            child->children[0] = lsib->children[lsib->n];
        child->n++;
        lsib->n--;
        return;
    }

    if (i < x->n && x->children[i + 1]->n > t) {
        BTMNode *child = x->children[i];
        BTMNode *rsib  = x->children[i + 1];

        child->keys[child->n]   = x->keys[i];
        child->values[child->n] = x->values[i];
        x->keys[i]   = rsib->keys[0];
        x->values[i] = rsib->values[0];
        if (!child->leaf)
            child->children[child->n + 1] = rsib->children[0];

        for (int j = 0; j < rsib->n - 1; j++) {
            rsib->keys[j]   = rsib->keys[j + 1];
            rsib->values[j] = rsib->values[j + 1];
        }
        if (!rsib->leaf) {
            for (int j = 0; j < rsib->n; j++)
                rsib->children[j] = rsib->children[j + 1];
        }
        child->n++;
        rsib->n--;
        return;
    }

    if (i < x->n)
        merge_children(map, x, i);
    else
        merge_children(map, x, i - 1);
}

static int delete_from(BTreeMap *map, BTMNode *x, PyObject *key)
{
    int found;
    int i = node_search(x, key, &found);
    if (i < 0) return -1;

    int t = (map->order - 1) / 2;

    if (found) {
        if (x->leaf) {
            Py_DECREF(x->keys[i]);
            Py_DECREF(x->values[i]);
            for (int j = i; j < x->n - 1; j++) {
                x->keys[j]   = x->keys[j + 1];
                x->values[j] = x->values[j + 1];
            }
            x->n--;
            return 1;
        }

        if (x->children[i]->n > t) {
            PyObject *pk = get_pred_key(x->children[i]);
            PyObject *pv = get_pred_val(x->children[i]);
            Py_INCREF(pk); Py_INCREF(pv);
            int rc = delete_from(map, x->children[i], pk);
            if (rc < 0) { Py_DECREF(pk); Py_DECREF(pv); return -1; }
            Py_DECREF(x->keys[i]);
            Py_DECREF(x->values[i]);
            x->keys[i]   = pk;
            x->values[i] = pv;
            return 1;
        }

        if (x->children[i + 1]->n > t) {
            PyObject *sk = get_succ_key(x->children[i + 1]);
            PyObject *sv = get_succ_val(x->children[i + 1]);
            Py_INCREF(sk); Py_INCREF(sv);
            int rc = delete_from(map, x->children[i + 1], sk);
            if (rc < 0) { Py_DECREF(sk); Py_DECREF(sv); return -1; }
            Py_DECREF(x->keys[i]);
            Py_DECREF(x->values[i]);
            x->keys[i]   = sk;
            x->values[i] = sv;
            return 1;
        }

        merge_children(map, x, i);
        return delete_from(map, x->children[i], key);
    }

    if (x->leaf) return 0;

    int descend = i;
    if (x->children[descend]->n <= t)
        ensure_min(map, x, descend);
    if (descend > x->n) descend = x->n;

    return delete_from(map, x->children[descend], key);
}

int btm_remove(BTreeMap *map, PyObject *key)
{
    if (map->root->n == 0) return 0;

    int rc = delete_from(map, map->root, key);
    if (rc < 0) return -1;

    if (map->root->n == 0 && !map->root->leaf) {
        BTMNode *old = map->root;
        map->root = old->children[0];
        free(old->keys);
        free(old->values);
        free(old->children);
        free(old);
    }

    if (rc > 0) map->size--;
    return rc;
}

BTMPos btm_first(BTreeMap *map)
{
    BTMPos pos = {NULL, 0};
    if (!map->root || map->root->n == 0) return pos;
    BTMNode *x = map->root;
    while (!x->leaf) x = x->children[0];
    pos.node = x;
    pos.idx  = 0;
    return pos;
}

BTMPos btm_next(BTreeMap *map, BTMPos pos)
{
    BTMPos result = {NULL, 0};
    if (!pos.node) return result;

    BTMNode *cur = pos.node;
    int ci = pos.idx;

    if (!cur->leaf) {
        BTMNode *x = cur->children[ci + 1];
        while (!x->leaf) x = x->children[0];
        result.node = x;
        result.idx  = 0;
        return result;
    }

    if (ci + 1 < cur->n) {
        result.node = cur;
        result.idx  = ci + 1;
        return result;
    }

    PyObject *cur_key = cur->keys[ci];
    BTMNode *x = map->root;
    BTMNode *ancestor = NULL;
    int ancestor_idx = 0;

    while (x) {
        int found;
        int i = node_search(x, cur_key, &found);
        if (i < 0) return result;

        if (found) {
            if (!x->leaf) {
                BTMNode *s = x->children[i + 1];
                while (!s->leaf) s = s->children[0];
                result.node = s;
                result.idx  = 0;
                return result;
            }
            if (i + 1 < x->n) {
                result.node = x;
                result.idx  = i + 1;
                return result;
            }
            if (ancestor) {
                result.node = ancestor;
                result.idx  = ancestor_idx;
            }
            return result;
        }

        if (i < x->n) {
            ancestor = x;
            ancestor_idx = i;
        }
        if (x->leaf) break;
        x = x->children[i];
    }

    if (ancestor) {
        result.node = ancestor;
        result.idx  = ancestor_idx;
    }
    return result;
}
