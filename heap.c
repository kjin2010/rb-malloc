#include "heap.h"
#include "debug.h"
#include "nodes.h"
#include <stdlib.h>
#include <stdio.h>

volatile long mCount;
volatile long fCount;

node *free_list = 0;

void make_node(void *location, node *left, node *right, node *parent) {
		node *cur_loc = (node*) location;
		node cur_node = {left, right, parent};
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

		node *temp_list = list;

		while (temp_list != 0 && get_size(temp_list) != size) {
				temp_list = get_size(temp_list) < size ? temp_list->right : temp_list->left;
		}

		if (temp_list == 0) return temp_list->parent;
		else if (get_size(temp_list) == size) return temp_list;
		else if (get_size(temp_list->parent) > size) return temp_list->parent;
		else return 0;
}

void init_free_list() {
		// making headers
		make_meta(&the_heap, HEAP_SIZE - 16, 1, 0);
		make_meta(&the_heap[HEAP_SIZE / sizeof(long) - 1], HEAP_SIZE - 16, 1, 0);
		// adding to red black tree
		make_node(&the_heap[1], 0, 0, 0);
		// insert(&free_list, (node*) &the_heap[1]);
		free_list = (void*) &the_heap[1];
} 

void *malloc(size_t size) {
		if (size - 16 > HEAP_SIZE || size == 0) return 0;
		size = size + (8 - (size % 8));
		size = size <= 24 ? 24 : size;

		node *valid_node = search(size, free_list);
		if (valid_node) {
				size_t cur_size = get_size(valid_node);
				// split if splittable
				if (cur_size - size >= 40) {

					// reinsert if splitted
					
				}
				// return valid_node
		}
		
		return 0;
}

void free(void *ptr) {

}
