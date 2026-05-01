#include "hashmap_internal.h"
#include "hashmap_iter.h"

void hm_iter_init(hm_iter *it, hashmap *hm)
{
    it->hm = hm;
    it->bucket_index = 0;
    it->cur = NULL;
    it->version = hm->version;
}

int hm_iter_next(hm_iter *it, void **out_key, void **out_val)
{
    if (it->version != it->hm->version)
        return -1;

    if (it->cur && it->cur->next)
    {
        it->cur = it->cur->next;
        *out_key = it->cur->key;
        *out_val = it->cur->val;
        return 1;
    }
    if (it->cur)
    {
        it->bucket_index++;
        it->cur = NULL;
    }

    while (it->bucket_index < it->hm->cap)
    {
        bucket *b = it->hm->buckets[it->bucket_index];
        if (b && b->head)
        {
            it->cur = b->head;
            *out_key = it->cur->key;
            *out_val = it->cur->val;
            return 1;
        }
        it->bucket_index++;
    }
    return 0;
}