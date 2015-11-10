typedef int rbt_key;
typedef enum { BLACK, RED } rbt_color;

struct rbt_node {
    rbt_key key;
    rbt_color color;
    //int size;
    struct rbt_node *left, *right, *p;
};

struct rbt_tree {
    struct rbt_node *root, *nil;
};


//My Own Functions
extern struct rbt_node *rbt_new_node(rbt_key key);
extern struct rbt_tree *rbt_new_tree(void);
extern struct rbt_node *rbt_locate(struct rbt_tree *T, rbt_key key);
extern int rbt_insert_one_key(struct rbt_tree *T, rbt_key key);
extern int rbt_insert_keys(struct rbt_tree *T, rbt_key *keys, int size);
extern void rbt_print_tree(struct rbt_tree *T, const char *filename);
extern int rbt_del_one_key(struct rbt_tree *T, rbt_key key);


//Functions in the book
extern int RB_INSERT(struct rbt_tree *T, struct rbt_node *z);
extern int RB_INSERT_FIXUP(struct rbt_tree *T, struct rbt_node *z);

extern int LEFT_ROTATE(struct rbt_tree *T, struct rbt_node *z);
extern int RIGHT_ROTATE(struct rbt_tree *T, struct rbt_node *z);


extern int RB_TRANSPLANT(struct rbt_tree *T, struct rbt_node *old, struct rbt_node *alt);
extern struct rbt_node* TREE_MINIMUM(struct rbt_node *node) __attribute__((always_inline)); 
extern int RB_DELETE_FIXUP(struct rbt_tree *T, struct rbt_node *x);
extern struct rbt_node *RB_DELETE(struct rbt_tree *T, struct rbt_node *node);

extern rbt_key OS_SELECT(struct rbt_node *root, int i);

