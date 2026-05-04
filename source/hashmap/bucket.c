#include "bucket.h"
#include <stdlib.h>
#include <stdint.h>

bucket *bucket_new(void)
{
    bucket *b = calloc(1, sizeof(bucket));
    if (!b)
        return NULL;
    b->kind = BK_LIST;
    return b;
}

void bucket_free(bucket *b)
{
    if (!b)
        return;
    hm_node *n = b->head;
    while (n)
    {
        hm_node *next = n->next;
        free(n);
        n = next;
    }
    free(b);
}

static void list_push_front(bucket *b, hm_node *n)
{
    n->prev = NULL;
    n->next = b->head;
    if (b->head)
        b->head->prev = n;
    b->head = n;
    b->size++;
}

static void list_unlink(bucket *b, hm_node *n)
{
    if (n->prev)
        n->prev->next = n->next;
    else
        b->head = n->next;
    if (n->next)
        n->next->prev = n->prev;
    b->size--;
}

static int tie_break(void *a, void *b)
{
    uintptr_t ua = (uintptr_t)a, ub = (uintptr_t)b;
    if (ua < ub)
        return -1;
    if (ua > ub)
        return 1;
    return 0;
}

static int tree_cmp(hm_node *n, size_t h, void *k,
                    int (*cmp)(void *, void *), int *err)
{
    if (h < n->hash)
        return -1;
    if (h > n->hash)
        return 1;
    int c = cmp(n->key, k);
    if (c < 0)
    {
        *err = 1;
        return 0;
    }
    if (c == 0)
        return 0;
    return tie_break(n->key, k);
}

static void rotate_left(bucket *b, hm_node *x)
{
    hm_node *y = x->right;
    x->right = y->left;
    if (y->left)
        y->left->parent = x;
    y->parent = x->parent;
    if (!x->parent)
        b->root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;
    y->left = x;
    x->parent = y;
}

static void rotate_right(bucket *b, hm_node *x)
{
    hm_node *y = x->left;
    x->left = y->right;
    if (y->right)
        y->right->parent = x;
    y->parent = x->parent;
    if (!x->parent)
        b->root = y;
    else if (x == x->parent->right)
        x->parent->right = y;
    else
        x->parent->left = y;
    y->right = x;
    x->parent = y;
}

static void insert_fixup(bucket *b, hm_node *z)
{
    while (z->parent && z->parent->red)
    {
        hm_node *p = z->parent;
        hm_node *g = p->parent;
        if (!g)
            break;
        if (p == g->left)
        {
            hm_node *u = g->right;
            if (u && u->red)
            {
                p->red = 0;
                u->red = 0;
                g->red = 1;
                z = g;
            }
            else
            {
                if (z == p->right)
                {
                    z = p;
                    rotate_left(b, z);
                    p = z->parent;
                }
                p->red = 0;
                g->red = 1;
                rotate_right(b, g);
            }
        }
        else
        {
            hm_node *u = g->left;
            if (u && u->red)
            {
                p->red = 0;
                u->red = 0;
                g->red = 1;
                z = g;
            }
            else
            {
                if (z == p->left)
                {
                    z = p;
                    rotate_right(b, z);
                    p = z->parent;
                }
                p->red = 0;
                g->red = 1;
                rotate_left(b, g);
            }
        }
    }
    b->root->red = 0;
}

static hm_node *tree_insert(bucket *b, hm_node *n,
                            int (*cmp)(void *, void *), int *err)
{
    hm_node *p = NULL, *cur = b->root;
    int c = 0;
    while (cur)
    {
        c = tree_cmp(cur, n->hash, n->key, cmp, err);
        if (*err)
            return NULL;
        if (c == 0)
            return cur;
        p = cur;
        cur = (c > 0) ? cur->left : cur->right;
    }
    n->parent = p;
    n->left = n->right = NULL;
    n->red = 1;
    if (!p)
        b->root = n;
    else if (c > 0)
        p->left = n;
    else
        p->right = n;
    insert_fixup(b, n);
    return NULL;
}

static hm_node *tree_find(bucket *b, size_t h, void *k,
                          int (*cmp)(void *, void *), int *err)
{
    hm_node *cur = b->root;
    while (cur)
    {
        int c = tree_cmp(cur, h, k, cmp, err);
        if (*err)
            return NULL;
        if (c == 0)
            return cur;
        cur = (c > 0) ? cur->left : cur->right;
    }
    return NULL;
}

static hm_node *tree_min(hm_node *n)
{
    while (n->left)
        n = n->left;
    return n;
}

static void transplant(bucket *b, hm_node *u, hm_node *v)
{
    if (!u->parent)
        b->root = v;
    else if (u == u->parent->left)
        u->parent->left = v;
    else
        u->parent->right = v;
    if (v)
        v->parent = u->parent;
}

static void delete_fixup(bucket *b, hm_node *x, hm_node *x_parent)
{
    while (x != b->root && (!x || !x->red))
    {
        if (x == x_parent->left)
        {
            hm_node *w = x_parent->right;
            if (w && w->red)
            {
                w->red = 0;
                x_parent->red = 1;
                rotate_left(b, x_parent);
                w = x_parent->right;
            }
            if (w && (!w->left || !w->left->red) &&
                (!w->right || !w->right->red))
            {
                w->red = 1;
                x = x_parent;
                x_parent = x->parent;
            }
            else
            {
                if (!w)
                    break;
                if (!w->right || !w->right->red)
                {
                    if (w->left)
                        w->left->red = 0;
                    w->red = 1;
                    rotate_right(b, w);
                    w = x_parent->right;
                }
                w->red = x_parent->red;
                x_parent->red = 0;
                if (w->right)
                    w->right->red = 0;
                rotate_left(b, x_parent);
                x = b->root;
                break;
            }
        }
        else
        {
            hm_node *w = x_parent->left;
            if (w && w->red)
            {
                w->red = 0;
                x_parent->red = 1;
                rotate_right(b, x_parent);
                w = x_parent->left;
            }
            if (w && (!w->right || !w->right->red) &&
                (!w->left || !w->left->red))
            {
                w->red = 1;
                x = x_parent;
                x_parent = x->parent;
            }
            else
            {
                if (!w)
                    break;
                if (!w->left || !w->left->red)
                {
                    if (w->right)
                        w->right->red = 0;
                    w->red = 1;
                    rotate_left(b, w);
                    w = x_parent->left;
                }
                w->red = x_parent->red;
                x_parent->red = 0;
                if (w->left)
                    w->left->red = 0;
                rotate_right(b, x_parent);
                x = b->root;
                break;
            }
        }
    }
    if (x)
        x->red = 0;
}

static void tree_remove(bucket *b, hm_node *z)
{
    hm_node *y = z;
    int y_was_red = y->red;
    hm_node *x;
    hm_node *x_parent;

    if (!z->left)
    {
        x = z->right;
        x_parent = z->parent;
        transplant(b, z, z->right);
    }
    else if (!z->right)
    {
        x = z->left;
        x_parent = z->parent;
        transplant(b, z, z->left);
    }
    else
    {
        y = tree_min(z->right);
        y_was_red = y->red;
        x = y->right;
        if (y->parent == z)
        {
            x_parent = y;
        }
        else
        {
            x_parent = y->parent;
            transplant(b, y, y->right);
            y->right = z->right;
            if (y->right)
                y->right->parent = y;
        }
        transplant(b, z, y);
        y->left = z->left;
        if (y->left)
            y->left->parent = y;
        y->red = z->red;
    }
    if (!y_was_red)
        delete_fixup(b, x, x_parent);
}

int bucket_insert(bucket *b, size_t h, void *k, void *v,
                  int (*cmp)(void *, void *), void **old_val)
{
    if (b->kind == BK_LIST)
    {
        for (hm_node *n = b->head; n; n = n->next)
        {
            if (n->hash == h)
            {
                int c = cmp(n->key, k);
                if (c < 0)
                    return -1;
                if (c == 0)
                {
                    if (old_val)
                        *old_val = n->val;
                    n->val = v;
                    return 0;
                }
            }
        }
        hm_node *n = calloc(1, sizeof(hm_node));
        if (!n)
            return -1;
        n->hash = h;
        n->key = k;
        n->val = v;
        list_push_front(b, n);
        return 1;
    }
    else
    {
        int err = 0;
        hm_node *existing = tree_find(b, h, k, cmp, &err);
        if (err)
            return -1;
        if (existing)
        {
            if (old_val)
                *old_val = existing->val;
            existing->val = v;
            return 0;
        }
        hm_node *n = calloc(1, sizeof(hm_node));
        if (!n)
            return -1;
        n->hash = h;
        n->key = k;
        n->val = v;
        list_push_front(b, n);
        err = 0;
        hm_node *dup = tree_insert(b, n, cmp, &err);
        if (err || dup)
        {
            list_unlink(b, n);
            free(n);
            return err ? -1 : 0;
        }
        return 1;
    }
}

int bucket_find(bucket *b, size_t h, void *k,
                int (*cmp)(void *, void *), void **out)
{
    if (b->kind == BK_LIST)
    {
        for (hm_node *n = b->head; n; n = n->next)
        {
            if (n->hash == h)
            {
                int c = cmp(n->key, k);
                if (c < 0)
                    return -1;
                if (c == 0)
                {
                    *out = n->val;
                    return 1;
                }
            }
        }
        return 0;
    }
    else
    {
        int err = 0;
        hm_node *n = tree_find(b, h, k, cmp, &err);
        if (err)
            return -1;
        if (!n)
            return 0;
        *out = n->val;
        return 1;
    }
}

int bucket_delete(bucket *b, size_t h, void *k,
                  int (*cmp)(void *, void *),
                  void **out_key, void **out_val)
{
    if (b->kind == BK_LIST)
    {
        for (hm_node *n = b->head; n; n = n->next)
        {
            if (n->hash == h)
            {
                int c = cmp(n->key, k);
                if (c < 0)
                    return -1;
                if (c == 0)
                {
                    if (out_key)
                        *out_key = n->key;
                    if (out_val)
                        *out_val = n->val;
                    list_unlink(b, n);
                    free(n);
                    return 1;
                }
            }
        }
        return 0;
    }
    else
    {
        int err = 0;
        hm_node *n = tree_find(b, h, k, cmp, &err);
        if (err)
            return -1;
        if (!n)
            return 0;
        if (out_key)
            *out_key = n->key;
        if (out_val)
            *out_val = n->val;
        tree_remove(b, n);
        list_unlink(b, n);
        free(n);
        return 1;
    }
}

void bucket_treeify(bucket *b, int (*cmp)(void *, void *))
{
    if (b->kind == BK_TREE)
        return;
    b->kind = BK_TREE;
    b->root = NULL;
    for (hm_node *n = b->head; n; n = n->next)
    {
        n->parent = n->left = n->right = NULL;
        n->red = 1;
        int err = 0;
        hm_node *dup = tree_insert(b, n, cmp, &err);
        (void)dup;
        if (err)
        {
            b->kind = BK_LIST;
            b->root = NULL;
            for (hm_node *m = b->head; m; m = m->next)
            {
                m->parent = m->left = m->right = NULL;
                m->red = 0;
            }
            return;
        }
    }
}

void bucket_untreeify(bucket *b)
{
    if (b->kind == BK_LIST)
        return;
    b->kind = BK_LIST;
    b->root = NULL;
    for (hm_node *n = b->head; n; n = n->next)
    {
        n->parent = n->left = n->right = NULL;
        n->red = 0;
    }
}