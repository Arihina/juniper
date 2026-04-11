#include "bucket_tree.h"
#include <stdlib.h>

bucket_tree *bt_new(){ return calloc(1,sizeof(bucket_tree)); }

static int node_cmp(rb_node*n,size_t h,void*k,int(*cmp)()){
    if(h<n->hash) return -1;
    if(h>n->hash) return 1;
    return cmp(n->key,k);
}

static rb_node* find_node(rb_node*n,size_t h,void*k,int(*cmp)()){
    while(n){
        int c=node_cmp(n,h,k,cmp);
        if(c==0) return n;
        n=(c<0)?n->r:n->l;
    }
    return NULL;
}

void *bt_find(bucket_tree*t,size_t h,void*k,int(*cmp)()){
    rb_node*n=find_node(t->root,h,k,cmp);
    return n? n->val:NULL;
}

void bt_insert(bucket_tree*t,size_t h,void*k,void*v,int(*cmp)()){
    rb_node **p=&t->root;
    while(*p){
        int c=node_cmp(*p,h,k,cmp);
        if(c==0){ (*p)->val=v; return; }
        p=(c<0)?&(*p)->r:&(*p)->l;
    }
    *p=calloc(1,sizeof(rb_node));
    (*p)->hash=h; (*p)->key=k; (*p)->val=v;
    t->size++;
}

static rb_node* min_node(rb_node*n){
    while(n->l) n=n->l;
    return n;
}

static rb_node* delete_rec(rb_node*root,size_t h,void*k,int(*cmp)()){
    if(!root) return NULL;

    int c=node_cmp(root,h,k,cmp);

    if(c<0) root->r=delete_rec(root->r,h,k,cmp);
    else if(c>0) root->l=delete_rec(root->l,h,k,cmp);
    else{
        if(!root->l){
            rb_node*tmp=root->r;
            free(root);
            return tmp;
        }
        if(!root->r){
            rb_node*tmp=root->l;
            free(root);
            return tmp;
        }

        rb_node*succ=min_node(root->r);
        root->hash=succ->hash;
        root->key=succ->key;
        root->val=succ->val;
        root->r=delete_rec(root->r,succ->hash,succ->key,cmp);
    }
    return root;
}

void bt_delete(bucket_tree*t,size_t h,void*k,int(*cmp)()){
    t->root=delete_rec(t->root,h,k,cmp);
    if(t->size) t->size--;
}

