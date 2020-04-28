#include "heap.h"
#include "debug.h"
#include <stdlib.h>
#include <stdio.h>

volatile long mCount;
volatile long fCount;

void *malloc() {
    return 0;
}

void free(void *ptr) {

}
