#include "heap.h"
#include "debug.h"
#include <stdlib.h>
#include <stdio.h>

volatile long mCount;
volatile long fCount;

typedef struct rb_node {
		struct rb_node *left, *right, *parent;
} node;

typedef struct {
		size_t size : 58;
		int is_black : 3;
		int is_used : 3;

} meta;

node *free_list;

node *make_node(void *location, node *left, node *right, node *parent) {
		node *cur_loc = (node*) location;
		node cur_node = {left, right, parent};
		*cur_loc = cur_node;
}

header *make_meta(void *location, size_t size, int is_black, int is_used) { 
		meta *cur_loc = (meta*) location;
		meta cur_meta = {size, is_black, is_used};
		*cur_loc = cur_meta;
}

void init_free_list() {
		// making headers
		make_meta(&the_heap, HEAP_SIZE - 16, 1, 0);
		make_meta(&the_heap[HEAP_SIZE / sizeof(long) - 1], HEAP_SIZE - 16, 1, 0);
		// adding to red black tree
		make_node(&the_heap[1], 0, 0, 0);
		free_list = &the_heap;
} 

void *malloc() {
    return 0;
}

void free(void *ptr) {

}
