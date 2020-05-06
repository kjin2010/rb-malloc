#ifndef DEFINED
#define DEFINED

typdef struct rb_node {
		struct rb_node *parent, *left, *right;
} node;

typedef struct {
		size_t size : 58;
		int is_black : 3;
		int is_used : 3;
} meta;

size_t get_size(node *cur_node);
int get_is_black(node *cur_node);
int get_is_used(node *cur_node);
void set_size(node *cur_node, size_t size);
void set_is_black(node *cur_node, int is_black);
void set_is_used(node *cur_node, int is_used);

#endif
