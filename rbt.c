#include <stdio.h>
#include "rbt.h"
#include <stdlib.h>
//#include <ctype.h>
//#include <string.h>
//#include <math.h>
//#include <time.h>
//#include <unistd.h>
//#include <mpi.h>
//#include <omp.h>
//#define 
//typedef 

#define SUCCESS() (__errcode == 0)
#define FAIL() (__errcode = -1)
#define RESET_ERR() (__errcode = 0)

static char __errcode;


void rbt_print_tree_travese(struct rbt_node *node, unsigned depth, struct rbt_node *nil)
{
    unsigned i = depth;
    const char *style = (node->color == RED) ? "\033[1;31;47m" : "\033[5;30;47m";
    if(node != nil) {
        while(i--) putchar('\t');
        printf("%s%d\033[0m\n", style, node->key);
        rbt_print_tree_travese(node->right, depth + 1, nil);
        rbt_print_tree_travese(node->left, depth + 1, nil);
    }
    else {
        while(i--) putchar('\t');
        printf("%s%s\033[0m\n", style, "NIL");
    }
}
void rbt_print_tree(struct rbt_tree *T)
{
    puts("------------- 8< ---------------");
    rbt_print_tree_travese(T->root, 0, T->nil);
}

struct rbt_node *rbt_new_node(rbt_key key)
{
    struct rbt_node *node = (struct rbt_node *)calloc(1, sizeof(struct rbt_node));
    node->key = key;
    return node;
}

struct rbt_tree *rbt_new_tree(void)
{
    struct rbt_tree *t = (struct rbt_tree *)calloc(1, sizeof(struct rbt_tree));
    t->nil = rbt_new_node(0);
    t->nil->color = BLACK;
    t->root = t->nil;
    return t;
}

int rbt_insert_keys(struct rbt_tree *T, rbt_key *keys, int size)
{
    RESET_ERR();
    if(size < 0) return -1;

    int i;
    struct rbt_node *node;
    for (i = 0; i < size; ++i)
    {
        node = rbt_new_node(keys[i]);
        RB_INSERT(T, node);
        if(! SUCCESS()) return -2;
    }
    return 0;
}

int LEFT_ROTATE(struct rbt_tree *T, struct rbt_node *z)
{
    if(!z) 
        return -1;

    struct rbt_node *rchild = z->right, **tmp;
    z->right = rchild->left;
    rchild->left = z;
    rchild->p = z->p;
    if(z->p == T->nil) {
        T->root = rchild;
    }

    tmp = (z->p->left == z) ? &z->p->left : &z->p->right;
    *tmp = rchild;

    z->p = rchild;

    return 0;
}

int RIGHT_ROTATE(struct rbt_tree *T, struct rbt_node *z)
{
    if(!z) 
        return -1;

    struct rbt_node *lchild = z->left, **tmp;
    z->left = lchild->right;
    lchild->right = z;
    lchild->p = z->p;
    if(z->p == T->nil) {
        T->root = lchild;
    }

    tmp = (z->p->left == z) ? &z->p->left : &z->p->right;
    *tmp = lchild;

    z->p = lchild;

    return 0;
}


int RB_TRANSPLANT(struct rbt_tree *T, struct rbt_node *toremove, struct rbt_node *replace)
{
    if(toremove->p == T->nil) {
        T->root = replace;
    }

    replace->p = toremove->p;

    struct rbt_node **tmp;
    tmp = (toremove->p->left == toremove) ? &toremove->p->left : &toremove->p->right;
    *tmp = replace;
    return 0;
}


rbt_key OS_SELECT(struct rbt_node *_root_, int i)
{
    RESET_ERR();
    struct rbt_node *root = _root_;
    while(root->size != 0) {
        if(root->left->size == i - 1) {
            return root->key;
        }
        else if(root->left->size > i - 1) {
            root = root->left;
        }
        else {
            root = root->right;
        }
    }

    FAIL();
    return (rbt_key)0;
}

__attribute__((always_inline)) struct rbt_node* TREE_MINIMUM(struct rbt_node *node) 
{
    if(! node) return NULL;

    // node->left != T.nil
    while(node->left->p != NULL) {
        node = node->left;
    }
    return node;
}

struct rbt_node *RB_DELETE(struct rbt_tree *T, struct rbt_node *node)
{
    rbt_node *tmp, *tofix;
    rbt_color ori_color;

    if(T->root == T->nil) return NULL;

    if(node->left == T->nil) {
        tofix = node->right;
        RB_TRANSPLANT(T, node, node->right);
    }
    else if(node->right == T->nil) {
        tofix = node->left;
        RB_TRANSPLANT(T, node, node->left);
    }
    else {
        tmp = TREE_MINIMUM(node->right);
        ori_color = tmp->color;
        tofix = tmp->right;
    }

    return node;
}

int RB_DELETE_FIXUP(struct rbt_tree *T, struct rbt_node *x)
{
    return 0;
}

int RB_INSERT_FIXUP(struct rbt_tree *T, struct rbt_node *z)
{
    struct rbt_node *y;
    while(z->p->color == RED) {
        if(z->p == z->p->p->left) {
            y = z->p->p->right;
            /*Case 1*/
            if(y->color == RED) {
                z->p->color = BLACK;
                y->color = BLACK;
                z->p->p->color = RED;
                z = z->p->p;
            }
            /*Case 2*/
            else if(z == z->p->right) {
                z = z->p;
                LEFT_ROTATE(T, z);
            }
            /*Case 3*/
            else {
                z->p->color = BLACK;
                z->p->p->color = RED;
                RIGHT_ROTATE(T, z->p->p);
            }
        }
        else {
            y = z->p->p->left;
            /*Case 1*/
            if(y->color == RED) {
                z->p->color = BLACK;
                y->color = BLACK;
                z->p->p->color = RED;
                z = z->p->p;
            }
            /*Case 2*/
            else if(z == z->p->left) {
                z = z->p;
                RIGHT_ROTATE(T, z);
            }
            /*Case 3*/
            else {
                z->p->color = BLACK;
                z->p->p->color = RED;
                LEFT_ROTATE(T, z->p->p);
            }
        }
    }

    T->root->color = BLACK;

    return 0;
}

int RB_INSERT(struct rbt_tree *T, struct rbt_node *z)
{
    struct rbt_node *root, *par, **tmp;
    root = T->root;
    par = T->nil;
    while(root != T->nil) {
        par = root;
        root = (z->key < root->key) ? root->left : root->right;
    }
    z->p = par;
    if(par == T->nil) {
        T->root = z;
    }
    else {
        tmp = (z->key < par->key) ? &par->left : &par->right;
        *tmp = z;
    }
    z->left = z->right = T->nil;
    z->color = RED;

    return RB_INSERT_FIXUP(T, z);
}
