typedef int rbt_key;
typedef enum { BLACK, RED } rbt_color;

struct rbt_node {
    rbt_key key;
    rbt_color color;
    int size;
    struct rbt_node *left, *right, *p;
};

struct rbt_tree {
    struct rbt_node *root, *nil;
};


//My Own Functions
extern struct rbt_tree *rbt_new_tree(void);
extern int rbt_insert_keys(struct rbt_tree *T, rbt_key *keys, int size);
extern void rbt_print_tree(struct rbt_tree *T);


//Functions in the book
extern int RB_INSERT(struct rbt_tree *T, struct rbt_node *z);
extern int RB_INSERT_FIXUP(struct rbt_tree *T, struct rbt_node *z);
extern int LEFT_ROTATE(struct rbt_tree *T, struct rbt_node *z);
extern int RIGHT_ROTATE(struct rbt_tree *T, struct rbt_node *z);
extern rbt_key OS_SELECT(struct rbt_node *root, int i);
