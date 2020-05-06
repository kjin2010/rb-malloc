#include "heap.h"
#include "debug.h"
#include <stdlib.h>
#include <stdio.h>

#define LENGTH (HEAP_SIZE / sizeof(long))

volatile long mCount;
volatile long fCount;


typedef struct rb_node {
		struct rb_node *parent, *left, *right;
} node;

typedef struct {
		size_t size : 58;
		int is_black : 3;
		int is_used : 3;
} meta;

node *temp = (node*) the_heap;
node **free_list = &temp; 
int has_init = 0;

size_t get_size(node *cur_node);
int get_is_black(node *cur_node);
int get_is_used(node *cur_node);
void set_size(node *cur_node, size_t size);
void set_is_black(node *cur_node, int is_black);
void set_is_used(node *cur_node, int is_used);

// helper function that gets is_black
int get_is_black(node *location)
{
    meta *cur_meta = (meta *)(((void *)location) - 8);
    return cur_meta->is_black;
}

// helper function that sets is_black
void set_is_black(node *location, int is_black) {
		meta *cur_meta = (meta*) (((void*) location) - 8);
		meta *next_meta = (meta*) (((void*) location) + get_size((node*) location));
		cur_meta->is_black = is_black;
		next_meta->is_black = is_black;
}

// helper function that gets is_used
int get_is_used(node *location)
{
    meta *cur_meta = (meta *)(((void *)location) - 8);
    return cur_meta->is_used;
}

// helper function that sets is_used
void set_is_used(node *location, int is_used) {
		meta *cur_meta = (meta*) (((void*) location) - 8);
		meta *next_meta = (meta*) (((void*) location) + get_size((node*) location));
		cur_meta->is_used = is_used;
		next_meta->is_used = is_used;
}

// helper function that gets size
size_t get_size(node *location)
{
    meta *cur_meta = (meta *)(((void *)location) - 8);
    return cur_meta->size;
}

// helper function that sets size
void set_size(node *location, size_t size) {
		meta *cur_meta = (meta*) (((void*) location) - 8);
		meta *next_meta = (meta*) (((void*) location) + size);
		cur_meta->size = size;
		next_meta->size = size;
}

// helper function that compares two nodes
// returns
int compare(node *orig, node *new)
{
    size_t orig_size = get_size(orig);
    size_t new_size = get_size(new);
    if (orig_size == new_size)
    {
        return new > orig;
    }
    else
        return new_size > orig_size;
}

void left_rotate(node **root, node *ptr)
{
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
    if (ptr->right != 0)
    {
        ptr->right->parent = ptr;
    }

    // replace root of tree if old node was root
    if (new_root->parent == 0)
    {
        *root = new_root;
    }
    // replace old node's parent's left/right child with new node
    else if (new_root->parent->left == ptr)
    {
        new_root->parent->left = new_root;
    }
    else
    {
        new_root->parent->right = new_root;
    }
}

void right_rotate(node **root, node *ptr)
{
    node *new_root = ptr->left;
    new_root->parent = ptr->parent;
    ptr->left = new_root->right;
    new_root->right = ptr;
    ptr->parent = new_root;

    if (ptr->left != 0)
    {
        ptr->left->parent = ptr;
    }

    if (new_root->parent == 0)
    {
        *root = new_root;
    }
    else if (new_root->parent->left == ptr)
    {
        new_root->parent->left = new_root;
    }
    else
    {
        new_root->parent->right = new_root;
    }
}

// function that fixes the insertion of cur_node from the bottom up
void fix(node **root, node *cur_node)
{
    while (cur_node != *root && !get_is_black(cur_node->parent))
    {
        node *uncle = (cur_node->parent == cur_node->parent->parent->left) ? cur_node->parent->parent->right : cur_node->parent->parent->left;

        // if uncle is red
        if (uncle != 0 && !get_is_black(uncle))
        {
            set_is_black(cur_node->parent, 1);
            set_is_black(cur_node->parent->parent, 0);
            set_is_black(uncle, 1);
            cur_node = cur_node->parent->parent;
        }
        // if uncle is black
        else
        {
            node *grandparent = cur_node->parent->parent;
            node *parent = cur_node->parent;

            // left-left case
            if (grandparent->left == parent && parent->left == cur_node)
            {
                int parent_is_black = get_is_black(parent);
                set_is_black(parent, get_is_black(grandparent));
                set_is_black(grandparent, parent_is_black);
                right_rotate(root, grandparent);
            }
            // left-right case
            else if (grandparent->left == parent && parent->right == cur_node)
            {
                left_rotate(root, parent);
                int cur_is_black = get_is_black(cur_node);
                set_is_black(cur_node, get_is_black(grandparent));
                set_is_black(grandparent, cur_is_black);
                right_rotate(root, grandparent);
            }
            // right-right case
            else if (grandparent->right == parent && parent->right == cur_node)
            {
                int parent_is_black = get_is_black(parent);
                set_is_black(parent, get_is_black(grandparent));
                set_is_black(grandparent, parent_is_black);
                left_rotate(root, grandparent);
            }
            // right-left case
            else
            {
                right_rotate(root, parent);
                int cur_is_black = get_is_black(cur_node);
                set_is_black(cur_node, get_is_black(grandparent));
                set_is_black(grandparent, cur_is_black);
                left_rotate(root, grandparent);
            }
        }

        set_is_black(*root, 1);
    }
}

// inserts new_node into *root using bst property
void insert(node **root, node *new_node)
{
    size_t size = get_size(new_node);

    if (*root == 0)
    {
        *root = new_node;
        set_is_black(new_node, 1);
    }
    else
    {
        node *runner = *root;
        node *prev_node = 0;

        while (runner != 0)
        {
            prev_node = runner;
            // runner = get_size(runner) < size ? runner->right : runner->left;
            runner = compare(runner, new_node) ? runner->right : runner->left;
        }

        new_node->parent = prev_node;
        // if (get_size(prev_node) < size) {
        if (compare(prev_node, new_node))
        {
            prev_node->right = new_node;
        }
        else
        {
            prev_node->left = new_node;
        }

        fix(root, new_node);
    }
}
/////DELETE is below
////
node *sibling(node *k)
{
    if (k->parent == 0)
        return 0;
    if (k == k->parent->left)
        return k->parent->right;
    else
        return k->parent->left;
}

node *replace(node *k)
{
    //node has two children
    if (k->left != 0 && k->right != 0)
    {
        node *temp = k->right;
        while (temp->left != 0)
        {
            temp = temp->left;
        }
        return temp;
    }

    //node is leef
    if (k->left == 0 && k->right == 0)
    {
        return 0;
    }

    //node has exactly one child
    if (k->left != 0)
        return k->left;
    else
        return k->right;
}

void fixBothBlack(node **root, node *k)
{
    if (k == *root)
    {
        return;
    }
    node *kPar = k->parent;
    node *kSib = sibling(k);

    if (kSib == 0)
    {
        fixBothBlack(root, kPar);
    }
    else
    {
        if (get_is_black(kSib) == 0)
        { //red sib
            //kPar->is_black = 0;
            //kSib->is_black = 1;
            set_is_black(kPar, 0);
            set_is_black(kSib, 1);

            if (kSib == kSib->parent->left)
            {
                right_rotate(root, kPar);
            }
            else
            {
                left_rotate(root, kPar);
            }
            fixBothBlack(root, k);
        }
        else
        { //black sib
            if ((kSib->left != 0 && get_is_black(kSib->left) == 0) ||
                (kSib->right != 0 && get_is_black(kSib->right) == 0))
            {
                //sib has at least one red child
                if (kSib->left != 0 && get_is_black(kSib->left) == 0)
                {
                    if (kSib == kSib->parent->left)
                    { //LL
                        //kSib->left->is_black = kSib->is_black;
                        //kSib->is_black = kPar->is_black;
                        set_is_black(kSib->left, get_is_black(kSib));
                        set_is_black(kSib, get_is_black(kPar));
                        right_rotate(root, kPar);
                    }
                    else
                    { //RL
                        //kSib->left->is_black = kPar->is_black;
                        set_is_black(kSib->left, get_is_black(kPar));
                        right_rotate(root, kSib);
                        left_rotate(root, kPar);
                    }
                }
                else
                {
                    if (kSib == kSib->parent->right)
                    { //RR
                        //kSib->right->is_black = kSib->is_black;
                        //kSib->is_black = kPar->is_black;
                        set_is_black(kSib->right, get_is_black(kSib));
                        set_is_black(kSib, get_is_black(kPar));
                        left_rotate(root, kPar);
                    }
                    else
                    { //LR
                        //kSib->right->is_black = kPar->is_black;
                        set_is_black(kSib->right, get_is_black(kPar));
                        left_rotate(root, kSib);
                        right_rotate(root, kPar);
                    }
                }
                //kPar->is_black = 1;
                set_is_black(kPar, 1);
            }
            else
            { //both children black
                //kSib->is_black = 0;
                set_is_black(kSib, 0);
                if (get_is_black(kPar) == 1)
                {
                    fixBothBlack(root, kPar);
                }
                else
                {
                    //kPar->is_black = 1;
                    set_is_black(kPar, 1);
                }
            }
        }
    }
}

void delete (node **root, node *new_node)
{   
    //first verify new_node is indeed in the tree
    node *temp = *root;
    while(temp != 0 && temp != new_node){
        temp = compare(temp, new_node) ? temp->right : temp->left;
    }
    if(temp == 0) return;

    
    node *runner = new_node; //idk why i did this line but whatev
    /*
    while (runner != 0 && runner != new_node) {
		//runner = get_size(runner) < size ? runner->right : runner->left;
        runner = compare(runner, new_node) ? runner->right : runner->left
	}
    */
    node *repl = replace(runner);
    int bothBlack = (get_is_black(runner)) && (repl == 0 || get_is_black(repl));
    //runner is leaf (no children)
    if (repl == 0)
    {
        if (runner == *root)
        {
            *root = 0;
        }
        else
        {
            if (bothBlack)
            {
                fixBothBlack(root, runner);
            }
            else if (sibling(runner) != 0)
            {
                //sibling(runner)->is_black = 0;
                set_is_black(sibling(runner), 0);
            }
            if (runner == runner->parent->left)
            {
                runner->parent->left = 0;
            }
            else
            {
                runner->parent->right = 0;
            }
        }
        return;
    }
    //runner has one child
    if (runner->left == 0 || runner->right == 0)
    {
        if (runner == *root)
        {
            //runner->size = repl->size;
            set_size(runner, get_size(repl));
            runner->left = 0;
            runner->right = 0;
            //remove repl from tree
            if (repl == repl->parent->left)
            {
                repl->parent->left = 0;
            }
            else
            {
                repl->parent->right = 0;
            }
        }
        else
        {
            if (runner == runner->parent->left)
            {
                runner->parent->left = repl;
            }
            else
            {
                runner->parent->right = repl;
            }
            repl->parent = runner->parent;

            if (bothBlack)
            {
                fixBothBlack(root, repl);
            }
            else
            {
                //repl->is_black = 1;
                set_is_black(repl, 1);
            }
        }
        return;
    }
    //runner has 2 children

    //int ct = repl->size;
    //repl->size = runner->size;
    //runner->size = ct;
    int ct = get_size(repl);
    set_size(repl, get_size(runner));
    set_size(runner, ct);

    delete (root, repl);
}

void make_node(void *location, node *left, node *right, node *parent) {
		node *cur_loc = (node*) location;
		node cur_node = {parent, left, right};
		*cur_loc = cur_node;
}

void make_meta(void *location, size_t size, int is_black, int is_used) { 
		meta *cur_loc = (meta*) location;
		meta cur_meta = {size, is_black, is_used};
		*cur_loc = cur_meta;
}

// returns a node of at least size_t size from list
node *search(size_t size, node *list) {
		if (list == 0) return 0;

		node *runner = list;
		node *prev_node = 0;

		while (runner != 0 && get_size(runner) != size) {
				prev_node = runner;
				runner = get_size(runner) < size ? runner->right : runner->left;
		}

		if (runner == 0) {
				while (prev_node != 0 && get_size(prev_node) < size) {
						prev_node = prev_node->parent;
				}						
				return prev_node; 
		}
		else {
				return runner;	
		}

		/*
		if (runner == 0) {
				if (get_size(prev_node) >= size) return prev_node;
				else return 0;
		}
		else if (get_size(runner) == size) return runner;
		else if (get_size(runner) > size) return runner->parent;
		else return 0;
		*/
}

void init_free_list() {
		// making headers
		make_meta(&the_heap, HEAP_SIZE - 16, 1, 0);
		make_meta(&the_heap[LENGTH - 1], HEAP_SIZE - 16, 1, 0);
		// adding to red black tree
		make_node(&the_heap[1], 0, 0, 0);
		// insert(&free_list, (node*) &the_heap[1]);
		*free_list = (node*) &the_heap[1];
} 

void set_headers(node *cur_node, size_t size, int is_used) {
		void *start = (void*) cur_node;
		make_meta(start - 8, size, 0, is_used);
		make_meta(start + size, size, 0, is_used);	
}

void *malloc(size_t size) {
		if (!has_init) {
				init_free_list();
				has_init = !has_init;
		}

		if (size - 16 > HEAP_SIZE || size == 0) return 0;
		size = size + (8 - size % 8) % 8;
		size = size <= 24 ? 24 : size;

		node *valid_node = search(size, *free_list);
		if (valid_node) {
				delete(free_list, valid_node);
				size_t cur_size = get_size(valid_node);
				// split if splittable
				if (cur_size - size >= 40) {
						// resets header size
						set_headers(valid_node, size, 1);
						void *split_block = ((void*) valid_node) + size + 16;
						make_node(split_block, 0, 0, 0);
						set_headers((node*) split_block, cur_size - size - 16, 0);
						// reinsert if splitted
						insert(free_list, ((node*) split_block));
				}
				else {
						set_is_used(valid_node, 1);
				}
				// return valid_node
				return valid_node;
		}
		else {
				return 0;
		}
}

int check_post_empty(node *cur_node) {
		size_t size = get_size(cur_node);
		meta *cur_footer = (meta*) (((void*) cur_node) +size);
		if (((void*) cur_footer) == ((void*) the_heap[LENGTH - 1])) {
				return 0;
		}
		else {
				meta *next_footer = (meta*) (cur_footer + 1);
				return next_footer->is_used;
		}
}

int check_prev_empty(node *cur_node) {
		if (((void*) cur_node) == ((void*) &the_heap[1])) {
				return 0;
		}
		else {
				meta *prev_footer = (meta*) (((void*) cur_node) - 16);
				return prev_footer->is_used;	
		}
}

void merge(node *first_node, node *second_node) {
		delete(free_list, first_node);
		delete(free_list, second_node);

		size_t total_size = get_size(first_node) + get_size(second_node);
		set_headers(first_node, total_size + 16, 0);
		
		insert(free_list, first_node);
}

void free(void *ptr) {		
		node *cur_node = (node*) ptr;
		int is_insert = 0;
		if (check_post_empty(cur_node)) {
				node *next_node = (node*) (ptr + get_size(cur_node) + 16);
				merge(cur_node, next_node);
				is_insert = 1;
		}

		if (check_prev_empty(cur_node)) {
				meta *prev_footer = (meta*) (ptr - 16);
				node *prev_node = (node*) (ptr - 16 - prev_footer->size);
				merge(prev_node, cur_node);
				is_insert = 1;
		}

		if (!is_insert) {
				insert(free_list, cur_node);
		}
}
