#include "hashmap_internal.h"
#include <stdlib.h>

#define TREEIFY_THRESHOLD 8
#define UNTREEIFY_THRESHOLD 6
#define MIN_TREEIFY_CAPACITY 64
#define LOAD_FACTOR_MAX 0.75
#define LOAD_FACTOR_MIN 0.10
#define HASHMAP_MIN_CAP 16

static char hm_new_key_marker;
static char hm_error_marker;
void *const HM_NEW_KEY = &hm_new_key_marker;
void *const HM_ERROR = &hm_error_marker;

static size_t round_up_pow2(size_t x)
{
    size_t p = HASHMAP_MIN_CAP;
    while (p < x)
        p <<= 1;
    return p;
}

hashmap *hm_new(size_t cap, hash_fn h, cmp_fn c)
{
    hashmap *hm = calloc(1, sizeof(hashmap));
    if (!hm)
        return NULL;
    hm->cap = round_up_pow2(cap < HASHMAP_MIN_CAP ? HASHMAP_MIN_CAP : cap);
    hm->hash = h;
    hm->cmp = c;
    hm->buckets = calloc(hm->cap, sizeof(bucket *));
    if (!hm->buckets)
    {
        free(hm);
        return NULL;
    }
    return hm;
}

void hm_set_err_flag(hashmap *hm, int *flag)
{
    hm->err_flag = flag;
}

static int hm_safe_hash(hashmap *hm, void *k, size_t *out)
{
    if (hm->err_flag)
        *hm->err_flag = 0;
    *out = hm->hash(k);
    if (hm->err_flag && *hm->err_flag)
        return -1;
    return 0;
}

static bucket *bucket_at(hashmap *hm, size_t h, int create)
{
    size_t i = h & (hm->cap - 1);
    if (!hm->buckets[i] && create)
    {
        hm->buckets[i] = bucket_new();
    }
    return hm->buckets[i];
}

static int hm_resize(hashmap *hm, size_t new_cap)
{
    new_cap = round_up_pow2(new_cap);
    bucket **new_buckets = calloc(new_cap, sizeof(bucket *));
    if (!new_buckets)
        return -1;

    bucket **old = hm->buckets;
    size_t old_cap = hm->cap;

    for (size_t i = 0; i < old_cap; i++)
    {
        bucket *b = old[i];
        if (!b)
            continue;

        hm_node *n = b->head;
        while (n)
        {
            hm_node *next = n->next;
            size_t j = n->hash & (new_cap - 1);
            bucket *nb = new_buckets[j];
            if (!nb)
            {
                nb = bucket_new();
                if (!nb)
                {
                    free(new_buckets);
                    return -1;
                }
                new_buckets[j] = nb;
            }

            n->prev = NULL;
            n->next = nb->head;
            n->parent = n->left = n->right = NULL;
            n->red = 0;
            if (nb->head)
                nb->head->prev = n;
            nb->head = n;
            nb->size++;
            n = next;
        }
        free(b);
    }
    free(old);
    hm->buckets = new_buckets;
    hm->cap = new_cap;

    for (size_t i = 0; i < new_cap; i++)
    {
        bucket *b = new_buckets[i];
        if (b && b->size >= TREEIFY_THRESHOLD)
        {
            bucket_treeify(b, hm->cmp);
        }
    }
    return 0;
}

void *hm_set(hashmap *hm, void *k, void *v)
{
    size_t h;
    if (hm_safe_hash(hm, k, &h) < 0)
        return HM_ERROR;

    if ((double)(hm->size + 1) / hm->cap > LOAD_FACTOR_MAX)
    {
        if (hm_resize(hm, hm->cap * 2) < 0)
            return HM_ERROR;
    }

    bucket *b = bucket_at(hm, h, 1);
    if (!b)
        return HM_ERROR;

    void *old = NULL;
    int r = bucket_insert(b, h, k, v, hm->cmp, &old);
    if (r < 0)
        return HM_ERROR;

    hm->version++;
    if (r == 1)
    {
        hm->size++;
        if (b->size >= TREEIFY_THRESHOLD && b->kind == BK_LIST)
        {
            if (hm->cap < MIN_TREEIFY_CAPACITY)
            {
                if (hm_resize(hm, hm->cap * 2) < 0)
                    return HM_ERROR;
            }
            else
            {
                bucket_treeify(b, hm->cmp);
            }
        }
        return HM_NEW_KEY;
    }
    return old;
}

int hm_get_ex(hashmap *hm, void *k, void **out)
{
    size_t h;
    if (hm_safe_hash(hm, k, &h) < 0)
        return -1;
    bucket *b = bucket_at(hm, h, 0);
    if (!b)
        return 0;
    return bucket_find(b, h, k, hm->cmp, out);
}

int hm_del_ex(hashmap *hm, void *k, void **out_key, void **out_val)
{
    size_t h;
    if (hm_safe_hash(hm, k, &h) < 0)
        return -1;

    size_t i = h & (hm->cap - 1);
    bucket *b = hm->buckets[i];
    if (!b)
        return 0;

    int r = bucket_delete(b, h, k, hm->cmp, out_key, out_val);
    if (r <= 0)
        return r;

    hm->size--;
    hm->version++;

    if (b->kind == BK_TREE && b->size <= UNTREEIFY_THRESHOLD)
    {
        bucket_untreeify(b);
    }

    if (b->size == 0)
    {
        bucket_free(b);
        hm->buckets[i] = NULL;
    }

    if (hm->cap > HASHMAP_MIN_CAP &&
        (double)hm->size / hm->cap < LOAD_FACTOR_MIN)
    {
        hm_resize(hm, hm->cap / 2);
    }
    return 1;
}

size_t hm_len(hashmap *hm) { return hm->size; }
unsigned long hm_version(hashmap *hm) { return hm->version; }

void hm_free(hashmap *hm)
{
    if (!hm)
        return;
    for (size_t i = 0; i < hm->cap; i++)
    {
        bucket_free(hm->buckets[i]);
    }
    free(hm->buckets);
    free(hm);
}