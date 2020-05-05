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

///////Delete is below
//////
node *sibling(node *k){
    if(k->parent == 0)
        return 0;
    if(k == k->parent->left)
        return k->parent->right;
    else
        return k->parent->left;
    
}

node *replace(node *k){
    //node has two children
    if(k->left != 0 && k->right != 0){
        node *temp = k->right;
        while(temp->left != 0){
            temp = temp->left;
        }
        return temp;
    }

    //node is leef
    if(k->left == 0 && k->right == 0){
        return 0;
    }

    //node has exactly one child
    if(k->left != 0) return k->left;
    else return k->right;
}

void fixBothBlack(node** root, node* k){
    if(k == *root){
        return;
    }
    node *kPar = k->parent;
    node *kSib = sibling(k);

    if(kSib == 0){
        fixBothBlack(root, kPar);
    }
    else{
        if(kSib->is_black == 0){//red sib
            kPar->is_black = 0;
            kSib->is_black = 1;
            if(kSib == kSib->parent->left){
                right_rotate(root, kPar);
            }
            else
            {
                left_rotate(root, kPar);
            }
            fixBothBlack(root, k);
        }
        else{//black sib
            if((kSib->left != 0 && kSib->left->is_black == 0) || 
            (kSib->right != 0 && kSib->right->is_black == 0)){
                //sib has at least one red child
                if(kSib->left != 0 && kSib->left->is_black == 0){
                    if(kSib == kSib->parent->left){//LL
                        kSib->left->is_black = kSib->is_black;
                        kSib->is_black = kPar->is_black;
                        right_rotate(root, kPar);
                    }
                    else{//RL
                        kSib->left->is_black = kPar->is_black;
                        right_rotate(root, kSib);
                        left_rotate(root, kPar);
                    }
                }
                else{
                    if(kSib == kSib->parent->right){//RR
                        kSib->right->is_black = kSib->is_black;
                        kSib->is_black = kPar->is_black;
                        left_rotate(root, kPar);
                    }
                    else{//LR
                        kSib->right->is_black = kPar->is_black;
                        left_rotate(root, kSib);
                        right_rotate(root, kPar);
                    }
                }
                kPar->is_black = 1;
            }
            else{//both children black
                kSib->is_black = 0;
                if(kPar->is_black == 1){
                    fixBothBlack(root, kPar);
                }
                else{
                    kPar->is_black = 1;
                }
            }
        }
    }
}

void delete(node **root, size_t size){
    node *runner = *root;
    while (runner != 0 && runner->size != size) {
		runner = runner->size < size ? runner->right : runner->left;
	}

    node *repl = replace(runner);
    int bothBlack = (runner->is_black) && (repl == 0 || repl->is_black);
    //runner is leaf (no children)
    if(repl == 0){
        if(runner == *root){
            root = 0;
        }
        else {
            if(bothBlack){
                fixBothBlack(root, runner);
            }
            else if(sibling(runner) != 0){
                sibling(runner)->is_black = 0;
            }
            if(runner == runner->parent->left){
                runner->parent->left = 0;
            }
            else{
                runner->parent->right = 0;
            }
        }        
        return;
    }
    //runner has one child
    if(runner->left == 0 || runner->right == 0){
        if(runner == *root){
            runner->size = repl->size;
            runner->left = 0;
            runner->right = 0;
            //remove repl from tree
            if(repl == repl->parent->left){
                repl->parent->left = 0;
            }
            else{
                repl->parent->right = 0;
            }
        }
        else{
            if(runner == runner->parent->left){
                runner->parent->left = repl;
            }
            else{
                runner->parent->right = repl;
            }
            repl->parent = runner->parent;

            if(bothBlack){
                fixBothBlack(root, repl);
            }
            else{
                repl->is_black = 1;
            }
        }
        return;
    }
    //runner has 2 children
    //int left = repl->size <= runner->size;
    int ct = repl->size;
    repl->size = runner->size;
    runner->size = ct;
    //replsize=3 runnersize=5
    delete(&((*root)->right), size);

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
        insert(&root, 3);
        delete(&root, 3);

		display(root);
    	return 0;
}
