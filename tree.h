#ifndef DEFINED
#define DEFINED

typedef struct rb_node node;
typedef struct Meta meta;

// helper functions
int get_is_black(node *location);
void set_is_black(node *location, int is_black);
int get_is_used(node *location);
void set_it_used(node *location, int is_used);
size_t get_size(node *location);
void set_size(node *location, size_t size);

// insert stuff
int compare(node *orig, node *new);
void left_rotate(node **root, node *ptr);
void right_rotate(node **root, node *ptr);
void fix(node **root, node *cur_node);
void insert(node **root, node *cur_node);

// delete
node *sibling(node *k);
node *replace(node *k);
void fixBothBlack(node **root, node *k);
void delete(node **root, node *new_node);

#endif
