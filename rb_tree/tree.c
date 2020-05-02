#include <stdio.h>
#include <stdlib.h>

// node struct in tree
typedef struct rb_node {
    struct rb_node *left, *right, *parent;
    size_t size : 63;
    int is_black : 1;
} node;

// node contructor
node *make_node(node *left, node *right, node *parent, size_t size, int is_black) {
	node *temp_node = (node*) malloc(sizeof(node));
	temp_node->left = left;
	temp_node->right = right;
	temp_node->parent = 0;
	temp_node->size = size;
	temp_node->is_black = is_black;
	return temp_node;
}

node *add(node *root, size_t size, int is_black) {
	if (root == 0) return make_node(0, 0, 0, size, is_black);

	if (root->size < size) {
		root->right = add(root->right, size, is_black);
		root->right->parent = root;
	}
	else {
		root->left = add(root->left, size, is_black);
		root->left->parent = root;
	}
	
	// add rotation shit here 

	return root;
}

void display_driver(node *root, int level) {
    if (root == 0) return;
    char *out = root->is_black ? "black" : "red";
    for (int i = 0; i < level; i++) {
        printf("\t");
    }
	size_t cur_size = root->size;
	size_t parent_size = root->parent == 0 ? 0 : root->parent->size;
    printf("[%lu] %lu:  %s\n", parent_size, cur_size, out);
    display_driver(root->left, level + 1);
    display_driver(root->right, level + 1);
}

void display(node *root) {
    display_driver(root, 0);
}

int main() {
	node *root = make_node(0, 0, 0, 500, 0);
	add(root, 150, 1);
	add(root, 750, 0);

    display(root);
    
    return 0;
}
