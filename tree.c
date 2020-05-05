#include <stdio.h>
#include <stdlib.h>
#include "nodes.h"

// helper function that gets is_black
int get_is_black(node *location) {
		meta *cur_meta = (meta*) (((void*) location) - 8);
		return cur_meta->is_black;
}

// helper function that sets is_black
void set_is_black(node *location, int is_black) {
		meta *cur_meta = (meta*) (((void*) location) - 8);
		cur_meta->is_black = is_black;
}

// helper function that gets is_used
int get_is_used(node *location) {
		meta *cur_meta = (meta*) (((void*) location) - 8);
		return cur_meta->is_used;
}

// helper function that sets is_used
void set_is_used(node *location, int is_used) {
		meta *cur_meta = (meta*) (((void*) location) - 8);
		cur_meta->is_used = is_used;
}

// helper function that gets size
size_t get_size(node *location) {
		meta *cur_meta = (meta*) (((void*) location) - 8);
		return cur_meta->size;
}

// helper function that sets size
void set_size(node *location, size_t size) {
		meta *cur_meta = (meta*) (((void*) location) - 8);
		cur_meta->size = size;
}

// returns a node of at least size_t size from list
node *search(size_t size, node *list) {
		if (list == 0) return 0;

		node *temp_list = list;

		while (temp_list != 0 && get_size(temp_list) != size) {
				temp_list = get_size(temp_list) < size ? temp_list->right : temp_list->left;
		}

		if (temp_list == 0) return temp_list->parent;
		else if (get_size(temp_list) == size) return temp_list;
		else if (get_size(temp_list->parent) > size) return temp_list->parent;
		else return 0;
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

// function that fixes the insertion of cur_node from the bottom up
void fix(node **root, node *cur_node) {
		while (cur_node != *root && !get_is_black(cur_node->parent)) {
				node *uncle = (cur_node->parent == cur_node->parent->parent->left) ?
							  cur_node->parent->parent->right :
							  cur_node->parent->parent->left;

				// if uncle is red
				if (uncle != 0 && !get_is_black(uncle)) {
						set_is_black(cur_node->parent, 1);
						set_is_black(curNode->parent->parent, 0);
						set_is_black(uncle, 1);
						cur_node = cur_node->parent->parent;
				}
				// if uncle is black
				else {
						node *grandparent = cur_node->parent->parent;
						node *parent = cur_node->parent;

						// left-left case
						if (grandparent->left == parent && parent->left == cur_node) {
								int parent_is_black = get_is_black(parent);
								set_is_black(parent, get_is_black(grandparent));
								set_is_black(grandparent, parent_is_black);
								right_rotate(root, grandparent);
						}
						// left-right case
						else if (grandparent->left == parent && parent->right == cur_node) {
								left_rotate(root, parent);
								int cur_is_black= get_is_black(cur_node);
								set_is_black(cur_node, get_is_black(grandparent));
								set_is_black(grandparent, cur_is_black);
								right_rotate(root, grandparent);
						}
						// right-right case
						else if (grandparent->right == parent && parent->right == cur_node) {
								int parent_is_black = get_is_black(parent);
								set_is_black(parent, get_is_black(grandparent));
								set_is_black(grandparent, parent_is_black);
								left_rotate(root, grandparent);
						}
						// right-left case
						else {
								right_rotate(root, parent);
								int cur_is_black= get_is_black(cur_node);
								set_is_black(cur_node, get_is_black(grandparent));
								set_is_black(grandparent, cur_is_black);
								left_rotate(root, grandparent);
						}
				}

				set_is_black(*root, 1);
		}
}

// inserts new_node into *root using bst property
void insert(node **root, node *new_node) {
		size_t size = get_size(new_node);

		if (*root == 0) {
				*root = new_node;
				set_is_black(new_node, 1);
		}
		else {
				node *runner = *root;
				node *prev_node = 0;

				while (runner != 0) {
						prev_node = runner;
						runner = get_size(runner) < size ? runner->right : runner->left;
				}

				new_node->parent = prev_node;
				if (get_size(prev_node) < size) {
						prev_node->right = new_node;
				}
				else {
						prev_node->left = new_node;
				}

				fix(root, new_node);
		}
}
