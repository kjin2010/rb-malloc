#ifndef _HEAP_H_
#define _HEAP_H_

#include <stdlib.h>

#define HEAP_SIZE (1 << 20)

extern long the_heap[];
extern volatile long mCount;
extern volatile long fCount;

#endif
