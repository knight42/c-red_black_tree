#include <stdio.h>
#include "rbt.h"
#include <stdlib.h>
#include <time.h>

#define FIX_SIZE(node) ((node)->size = (node)->left->size + (node)->right->size + 1)

/*The implementation of my own functions*/
/*============== BEGIN ==================*/
void rbt_preord_traverse(struct rbt_node *root, FILE *fout)
{
    if(root->p != NULL) {
        fprintf(fout, "%d\n", root->key);
        rbt_preord_traverse(root->left, fout);
        rbt_preord_traverse(root->right, fout);
    }
}

void rbt_inord_traverse(struct rbt_node *root, FILE *fout)
{
    if(root->p != NULL) {
        rbt_inord_traverse(root->left, fout);
        fprintf(fout, "%d\n", root->key);
        rbt_inord_traverse(root->right, fout);
    }
}

void rbt_postord_traverse(struct rbt_node *root, FILE *fout)
{
    if(root->p != NULL) {
        rbt_postord_traverse(root->left, fout);
        rbt_postord_traverse(root->right, fout);
        fprintf(fout, "%d\n", root->key);
    }
}

void rbt_print_tree_travese(struct rbt_tree *T, struct rbt_node *node, FILE *output)
{
    static int count = 0;
    int cur;
    clock_t t;
    static char tmp[100];

    if(node != T->nil) {
        cur = count;

        if(node->color == RED) {
            fprintf(output, "%d [color=\"red\"];\n", count);
        }

        #ifdef EXTENDED_RBT
        fprintf(output, "%d [label=\"{<f0>key: %d | <f1>rank: %d | <f2>size: %d}\"];\n", \
                    count, node->key, OS_RANK(T, node), node->size);
        #else
        fprintf(output, "%d [label=\"key: %d\"];\n", count, node->key);
        #endif

        if(node->left != T->nil) {
            fprintf(output, "%d -> %d;\n", count, count + 1);
        }
        else {
            t = clock();
            // generate uniq node name
            sprintf(tmp, "nil%ld [shape=point];\n%%d -> nil%ld;\n", t, t);
            fprintf(output, tmp, count);
        }

        ++count;

        rbt_print_tree_travese(T, node->left, output);

        if(node->right != T->nil) {
            fprintf(output, "%d -> %d;\n", cur, count);
        }
        else {
            t = clock();
            sprintf(tmp, "nil%ld [shape=point];\n%%d -> nil%ld;\n", t, t);
            fprintf(output, tmp, cur);
        }

        rbt_print_tree_travese(T, node->right, output);
    }
}
void rbt_print_tree(struct rbt_tree *T, const char *filename)
{
    FILE *result = fopen(filename, "w");
    fputs("digraph BST {\nnode [shape=record];\n", result);
    fputs("node[fontsize = \"10\"];\n", result);
    rbt_print_tree_travese(T, T->root, result);
    fputs("}\n", result);
}

void rbt_free_all_nodes(struct rbt_node *root)
{
    if(root->p != NULL) {
        rbt_free_all_nodes(root->left);
        rbt_free_all_nodes(root->right);
        free(root);
    }
}
int rbt_destroy_tree(struct rbt_tree *T)
{
    if(!T) {
        diehere("%s\n", "Empty tree is invalid!");
        return -1;
    }

    rbt_free_all_nodes(T->root);
    free(T->nil);
    free(T);
    return 0;
}

struct rbt_node *rbt_new_node(rbt_key key)
{
    struct rbt_node *node = (struct rbt_node *)calloc(1, sizeof(struct rbt_node));
    // equals to:
    // node->p = node->left = node->right = NULL
    // node->size = node->key = 0
    node->key = key;
    return node;
}

struct rbt_tree *rbt_new_tree(void)
{
    struct rbt_tree *t = (struct rbt_tree *)calloc(1, sizeof(struct rbt_tree));
    // equals to:
    // t->p = t->left = t->right = NULL

    if(!t) {
        diehere("%s\n", "Fail to alloc memory");
        return NULL;
    }

    t->nil = rbt_new_node(0);
    t->nil->color = BLACK;
    t->root = t->nil;
    return t;
}

int rbt_ins_one_key(struct rbt_tree *T, rbt_key key)
{
    if(!T) {
        diehere("%s\n", "Empty tree is invalid!");
        return -1;
    }

    struct rbt_node *node;
    node = rbt_new_node(key);
    RB_INSERT(T, node);
    return 0;
}

int rbt_ins_keys(struct rbt_tree *T, rbt_key *keys, int size)
{
    if(size < 0 || !T || !keys) {
        diehere("%s\n", "Invalid input.");
        return -1;
    }

    int i;
    for (i = 0; i < size; ++i)
    {
        rbt_ins_one_key(T, keys[i]);
    }
    return 0;
}

struct rbt_node *rbt_locate(struct rbt_tree *T, rbt_key key)
{
    if(!T) {
        diehere("%s\n", "Empty tree is invalid!");
        return NULL;
    }

    struct rbt_node *root = T->root;
    while(root != T->nil) {
        if(root->key == key) return root;
        root = (root->key > key) ? root->left : root->right;
    }
    return NULL;
}

int rbt_del_one_key(struct rbt_tree *T, rbt_key key)
{
    struct rbt_node *position = rbt_locate(T, key);
    if(! position) {
        diehere("%d is not in the Red-Black-Tree\n", key);
        return -1;
    }
    RB_DELETE(T, position);
    return 0;
}

int rbt_del_keys(struct rbt_tree *T, rbt_key *keys, int nmem)
{
    int i;
    for (i = 0; i < nmem; ++i)
    {
        rbt_del_one_key(T, keys[i]);
    }
    return 0;
}

/*============== END ==================*/

#ifdef EXTENDED_RBT

void ext_rbt_allrank(struct rbt_node *root)
{
    if(root->p != NULL) {
        ext_rbt_allrank(root->left);
        ext_rbt_allrank(root->right);
        root->size = root->left->size + root->right->size + 1;
    }
}

int OS_RANK(struct rbt_tree *T, struct rbt_node *node)
{
    int rank = node->left->size + 1;
    while(node != T->root) {
        if(node == node->p->right)
            rank += node->p->left->size + 1;
        node = node->p;
    }
    return rank;
}

struct rbt_node* OS_SELECT(struct rbt_node *root, int i)
{
    if(i <= 0)
        return NULL;

    // root != T->nil
    while(root->p != NULL) {
        if(root->left->size == i - 1) {
            return root;
        }
        else if(root->left->size > i - 1) {
            root = root->left;
        }
        else {
            i -= root->left->size + 1;
            root = root->right;
        }
    }
    return NULL;
}

int RB_SIZE_FIXUP(struct rbt_tree *T, struct rbt_node *node)
{
    while(node != T->nil) {
        node->size = node->left->size + node->right->size + 1;
        node = node->p;
    }
}
#endif


int LEFT_ROTATE(struct rbt_tree *T, struct rbt_node *z)
{
    if(!z) {
        diehere("%s\n", "Invalid input.");
        return -1;
    }

    struct rbt_node *rchild = z->right, **tmp;
    z->right = rchild->left;
    if(rchild->left != T->nil) {
        rchild->left->p = z;
    }
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
    if(!z) {
        diehere("%s\n", "Invalid input.");
        return -1;
    }

    struct rbt_node *lchild = z->left, **tmp;
    z->left = lchild->right;
    if(lchild->right != T->nil) {
        lchild->right->p = z;
    }
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

struct rbt_node* TREE_MINIMUM(struct rbt_node *node)
{
    if(! node) return NULL;

    // node->left != T.nil
    while(node->left->p != NULL) {
        node = node->left;
    }
    return node;
}

int RB_TRANSPLANT(struct rbt_tree *T, struct rbt_node *old, struct rbt_node *alt)
{
    if(!T || !old || !alt)
        return -1;

    if(old->p == T->nil) {
        T->root = alt;
    }

    if(alt != T->nil) {
        alt->p = old->p;
    }

    struct rbt_node **child;
    child = (old->p->left == old) ? &old->p->left : &old->p->right;
    *child = alt;
    return 0;
}

int RB_DELETE_FIXUP(struct rbt_tree *T, struct rbt_node *fix)
{
    if(!fix)
        return -1;

    struct rbt_node *uncle;
    while(fix != T->root && fix->color == BLACK) {
        // left child
        if(fix == fix->p->left) {
            uncle = fix->p->right;
            // Case 1
            if(uncle->color == RED) {
                uncle->color = BLACK;
                fix->p->color = RED;
                LEFT_ROTATE(T, fix->p);

                #ifdef EXTENDED_RBT
                FIX_SIZE(fix->p);
                FIX_SIZE(fix->p->p);
                #endif

                uncle = fix->p->right;
            }
            // Case 2
            if(uncle->left->color == BLACK && uncle->right->color == BLACK) {
                uncle->color = RED;
                fix = fix->p;
            }
            else {
            // Case 3
                if(uncle->right->color == BLACK) {
                    uncle->left->color = BLACK;
                    uncle->color = RED;
                    RIGHT_ROTATE(T, uncle);

                    #ifdef EXTENDED_RBT
                    FIX_SIZE(uncle);
                    FIX_SIZE(uncle->p);
                    #endif

                    /*uncle = fix->p->right;*/
                    uncle = uncle->p;
                }

            // Case 4
                uncle->color = fix->p->color;
                fix->p->color = BLACK;
                uncle->right->color = BLACK;
                LEFT_ROTATE(T, fix->p);

                #ifdef EXTENDED_RBT
                FIX_SIZE(fix->p);
                FIX_SIZE(fix->p->p);
                #endif

                fix = T->root;
            }
        }
        // right child
        else {
            uncle = fix->p->left;
            // Case 1
            if(uncle->color == RED) {
                uncle->color = BLACK;
                fix->p->color = RED;
                RIGHT_ROTATE(T, fix->p);
                uncle = fix->p->left;
            }
            // Case 2
            if(uncle->right->color == BLACK && uncle->left->color == BLACK) {
                uncle->color = RED;
                fix = fix->p;
            }
            else {
            // Case 3
                if(uncle->left->color == BLACK) {
                    uncle->right->color = BLACK;
                    uncle->color = RED;
                    LEFT_ROTATE(T, uncle);

                    #ifdef EXTENDED_RBT
                    FIX_SIZE(uncle);
                    FIX_SIZE(uncle->p);
                    #endif

                    /*uncle = fix->p->left;*/
                    uncle = uncle->p;
                }

            // Case 4
                uncle->color = fix->p->color;
                fix->p->color = BLACK;
                uncle->left->color = BLACK;
                RIGHT_ROTATE(T, fix->p);

                #ifdef EXTENDED_RBT
                FIX_SIZE(fix->p);
                FIX_SIZE(fix->p->p);
                #endif

                fix = T->root;
            }
        }
    }

    fix->color = BLACK;
    return 0;
}

struct rbt_node *RB_DELETE(struct rbt_tree *T, struct rbt_node *node)
{
    struct rbt_node *succ = NULL, *tofix = NULL;
    rbt_color ori_color;

    if(T->root == T->nil || node == NULL) return NULL;

    ori_color = node->color;

    if(node->left == T->nil) {
        tofix = node->right;
        RB_TRANSPLANT(T, node, tofix);

        #ifdef EXTENDED_RBT
        RB_SIZE_FIXUP(T, node->p);
        #endif
    }
    else if(node->right == T->nil) {
        tofix = node->left;
        RB_TRANSPLANT(T, node, tofix);

        #ifdef EXTENDED_RBT
        RB_SIZE_FIXUP(T, node->p);
        #endif
    }
    else {
        succ = TREE_MINIMUM(node->right);
        ori_color = succ->color;
        tofix = succ->right;

        /*
         *The leaf can be identified that its pointer to parent is NULL
         *So T->nil->p cannot be modified
         */
        if(succ->p == node && tofix != T->nil) {
            /*tofix->p = succ;*/
        }
        else {
            RB_TRANSPLANT(T, succ, tofix);

            #ifdef EXTENDED_RBT
            RB_SIZE_FIXUP(T, succ->p);
            #endif

            succ->right = node->right;
            if(succ->right != T->nil) {
                succ->right->p = succ;
            }
        }
        RB_TRANSPLANT(T, node, succ);
        succ->left = node->left;
        succ->left->p = succ;
        succ->color = node->color;

        #ifdef EXTENDED_RBT
        RB_SIZE_FIXUP(T, succ);
        #endif
    }

    // only fix non-leaf node
    if (tofix == T->nil) {

    }
    else if(ori_color == BLACK) {
        RB_DELETE_FIXUP(T, tofix);
    }

    return node;
}

int RB_INSERT_FIXUP(struct rbt_tree *T, struct rbt_node *z)
{
    struct rbt_node *y;

    #ifdef EXTENDED_RBT
    RB_SIZE_FIXUP(T, z);
    #endif

    while(z->p->color == RED) {
        // left child
        if(z->p == z->p->p->left) {
            y = z->p->p->right;
            /*Case 1*/
            if(y->color == RED) {
                z->p->color = BLACK;
                y->color = BLACK;
                z->p->p->color = RED;
                z = z->p->p;
            }
            else {
                /*Case 2*/
                if(z == z->p->right) {
                    z = z->p;
                    LEFT_ROTATE(T, z);
                    #ifdef EXTENDED_RBT
                    FIX_SIZE(z);
                    FIX_SIZE(z->p);
                    #endif
                }

                /*Case 3*/
                z->p->color = BLACK;
                z->p->p->color = RED;
                RIGHT_ROTATE(T, z->p->p);
                #ifdef EXTENDED_RBT
                FIX_SIZE(z->p->right);
                FIX_SIZE(z->p);
                #endif
            }
        }
        // right child
        else {
            y = z->p->p->left;
            /*Case 1*/
            if(y->color == RED) {
                z->p->color = BLACK;
                y->color = BLACK;
                z->p->p->color = RED;
                z = z->p->p;
            }
            else {
            /*Case 2*/
                if(z == z->p->left) {
                    z = z->p;
                    RIGHT_ROTATE(T, z);
                    #ifdef EXTENDED_RBT
                    FIX_SIZE(z);
                    FIX_SIZE(z->p);
                    #endif
                }

            /*Case 3*/
                z->p->color = BLACK;
                z->p->p->color = RED;
                LEFT_ROTATE(T, z->p->p);
                #ifdef EXTENDED_RBT
                FIX_SIZE(z->p->left);
                FIX_SIZE(z->p);
                #endif
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

    RB_INSERT_FIXUP(T, z);
    return 0;
}
