#ifndef __MY_RBT__
#define __MY_RBT__

#include <stdio.h>
#include "rbt_core.h"

//My wrapper functions
extern struct rbt_node *rbt_new_node(rbt_key key);
extern struct rbt_tree *rbt_new_tree(void);


extern struct rbt_node *rbt_locate(struct rbt_tree *T, rbt_key key);


extern void rbt_preord_tranverse(struct rbt_node *root, FILE *fout);
extern void rbt_inord_tranverse(struct rbt_node *root, FILE *fout);
extern void rbt_postord_tranverse(struct rbt_node *root, FILE *fout);



extern int rbt_insert_one_key(struct rbt_tree *T, rbt_key key);
extern int rbt_insert_keys(struct rbt_tree *T, rbt_key *keys, int size);
extern int rbt_del_one_key(struct rbt_tree *T, rbt_key key);
extern int rbt_del_keys(struct rbt_tree *T, rbt_key *keys, int nmem);


extern void rbt_print_tree(struct rbt_tree *T, const char *filename);
#endif
