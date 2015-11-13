#ifndef __MY_RBT__
#define __MY_RBT__

#include <stdio.h>
#include "rbt_core.h"

#define diehere(format, ...) fprintf(stderr, "%s: " format, __FUNCTION__, __VA_ARGS__)

//My wrapper functions
extern int rbt_destroy_tree(struct rbt_tree *T);

extern struct rbt_node *rbt_new_node(rbt_key key);
extern struct rbt_tree *rbt_new_tree(void);


extern struct rbt_node *rbt_locate(struct rbt_tree *T, rbt_key key);


typedef void (*trav_func_t) (struct rbt_node*, FILE *);

extern void rbt_preord_traverse(struct rbt_node *root, FILE *fout);
extern void rbt_inord_traverse(struct rbt_node *root, FILE *fout);
extern void rbt_postord_traverse(struct rbt_node *root, FILE *fout);


extern int rbt_ins_one_key(struct rbt_tree *T, rbt_key key);
extern int rbt_ins_keys(struct rbt_tree *T, rbt_key *keys, int size);
extern int rbt_del_one_key(struct rbt_tree *T, rbt_key key);
extern int rbt_del_keys(struct rbt_tree *T, rbt_key *keys, int nmem);


extern void rbt_print_tree(struct rbt_tree *T, const char *filename);

#ifdef EXTENDED_RBT
void ext_rbt_allrank(struct rbt_node *root);
#endif

#endif
