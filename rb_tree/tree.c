#include <stdio.h>
#include <stdlib.h>

// node struct in tree
typedef struct rb_node {
    struct rb_node *left, *right, *parent;
    size_t size : 61;
    int is_black : 3;
} node;

// node contructor
node *make_node(node *left, node *right, node *parent, size_t size, int is_black) {
		node *temp_node = (node*) malloc(sizeof(node));
		temp_node->left = left;
		temp_node->right = right;
		temp_node->parent = 0;
		temp_node->size = size;
		temp_node->is_black = is_black ? 1 : 0;
		return temp_node;
}

void left_rotate(node **root, node *ptr) {
		// right child takes current node's place
		node *new_root = ptr->right;
		// old node's parent becomes new node's parent
		new_root->parent = ptr->parent;
		// new node's left child is original node 
		ptr->right = new_root->left;
		// replacing old node's parent with new node 
		new_root->left = ptr;
		// new node's left child becomes old node's right child
		ptr->parent = new_root;

		// change parent if not null
		if (ptr->right != 0) {
				ptr->right->parent = ptr;
		}

		// replace root of tree if old node was root
		if (new_root->parent == 0) {
				*root = new_root;
		}
		// replace old node's parent's left/right child with new node 
		else if (new_root->parent->left == ptr) {
				new_root->parent->left = new_root;
		}
		else {
				new_root->parent->right = new_root;
		}
}

void right_rotate(node **root, node *ptr) {
		node *new_root = ptr->left;
		new_root->parent = ptr->parent;
		ptr->left = new_root->right;
		new_root->right = ptr;
		ptr->parent = new_root;

		if (ptr->left != 0) {
				ptr->left->parent = ptr;
		}

		if (new_root->parent == 0) {
				*root = new_root;
		}
		else if (new_root->parent->left == ptr) {
				new_root->parent->left = new_root;
		}
		else {
				new_root->parent->right = new_root;
		}
}

void fix(node **root, node *cur_node) {
		while (cur_node != *root && !cur_node->parent->is_black) {
				node *uncle = (cur_node->parent == cur_node->parent->parent->left) ?
							  cur_node->parent->parent->right :
							  cur_node->parent->parent->left;
				
				// if uncle is red
				if (uncle != 0 && !uncle->is_black) {
						cur_node->parent->is_black = 1;
						cur_node->parent->parent->is_black = 0;
						uncle->is_black = 1;
						cur_node = cur_node->parent->parent;
				}
				// if uncle is black
				else {
						node *grandparent = cur_node->parent->parent;
						node *parent = cur_node->parent;

						// left-left case
						if (grandparent->left == parent && parent->left == cur_node) {
								int parent_is_black = parent->is_black;
								parent->is_black = grandparent->is_black;
								grandparent->is_black = parent_is_black;
								right_rotate(root, grandparent);
						}
						// left-right case
						else if (grandparent->left == parent && parent->right == cur_node) {
								left_rotate(root, parent);
								int cur_is_black= cur_node->is_black;
								cur_node->is_black = grandparent->is_black;
								grandparent->is_black = cur_is_black;
								right_rotate(root, grandparent);
						}
						// right-right case
						else if (grandparent->right == parent && parent->right == cur_node) {
								int parent_is_black = parent->is_black;
								parent->is_black = grandparent->is_black;
								grandparent->is_black = parent->is_black;
								left_rotate(root, grandparent);
						}
						// right-left case
						else {
								right_rotate(root, parent);
								int cur_is_black= cur_node->is_black;
								cur_node->is_black = grandparent->is_black;
								grandparent->is_black = cur_is_black;
								left_rotate(root, grandparent);
						}
				}

				(*root)->is_black = 1;
		}
}

void insert(node **root, size_t size) {
		node *new_node = make_node(0, 0, 0, size, 0);
		
		// if empty tree, make current node root
		if (*root == 0) {
				*root = new_node;
				new_node->is_black = 1;
		}
		else {
				node *runner = *root;
				node *prev_node = 0;

				while (runner != 0) {
						prev_node = runner;
						runner = runner->size < size ? runner->right : runner->left;
				}

				new_node->parent = prev_node;
				if (prev_node->size < size) {
						prev_node->right = new_node;
				}
				else {
						prev_node->left = new_node;
				}

				// ADD FIX
				fix(root, new_node);
		}
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

void print_tree(node *root) {
		if (root == 0) return;
		size_t cur_size = root->size;
		printf("%lu\n", cur_size);
		print_tree(root->left);
		print_tree(root->right);
}

int main() {
		node *root = 0;
		insert(&root, 5);
		insert(&root, 3);
		insert(&root, 1);
		insert(&root, 15);
		insert(&root, 0);
		insert(&root, 10);

		display(root);
    	return 0;
}
