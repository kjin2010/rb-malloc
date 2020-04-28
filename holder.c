// #include "heap.h"
// #include "debug.h"
// #include <stdlib.h>
// #include <stdio.h>

// volatile long mCount = 0;
// volatile long fCount = 0;

// typedef struct {
//     size_t size : 61;
//     int isUsed : 3;
// } header; 

// typedef struct listNode{
//     struct listNode *next;
// } freeListNode;

// void makeHeader(size_t size, int isUsed, void *location) {
//     header *curLocation = (header*) location;
//     header temp = {size, isUsed};
//     *curLocation = temp;
// }

// void makeNode(freeListNode *next, void *location) {
//     freeListNode *curLocation = (freeListNode*) location;
//     freeListNode temp = {next};
//     *curLocation = temp;
// }

// freeListNode *freeList;
// freeListNode *endList;

// void initFreeList() {
//     makeHeader(HEAP_SIZE - 16, 0, &the_heap);
//     makeHeader(HEAP_SIZE - 16, 0, &the_heap[HEAP_SIZE / sizeof(long) - 1]);
//     makeNode(0, &the_heap[1]);
//     makeNode(0, &the_heap[2]);
//     freeList = (freeListNode*) &the_heap[1];
//     endList = (freeListNode*) &the_heap[2];
// }

// // makes block from start of current free node
// void *makeBlock(size_t size, freeListNode *curFreeNode) {
//     header *curHeader = (header*) (curFreeNode - 1);
//     size_t prevSize = curHeader->size;
//     header *curFooter = (header*) (curFreeNode + prevSize / 8);
//     curHeader->isUsed = 1;

//     freeListNode *curNext = curFreeNode->next;
//     freeListNode *curPrev = ((freeListNode*) (curFreeNode + 1))->next;

//     size_t usedSize = (size <= 16) ? 16 : size;
//     size_t newSize = prevSize - size - 16;
    
//     makeHeader(usedSize, 1, curFreeNode + usedSize / 8);
//     makeHeader(newSize, 0, curFreeNode + usedSize / 8 + 1);
//     curFooter->size = newSize;

//     makeNode(curNext, curFreeNode + usedSize / 8 + 2);
//     makeNode(curPrev, curFreeNode + usedSize / 8 + 3);

//     if (!curPrev) {
//         freeList = (freeListNode*) (curFreeNode + usedSize / 8 + 2);
//     }
//     else {
//         curPrev->next = (freeListNode*) (curFreeNode + usedSize / 8 + 2);
//     }

//     // printf("malloced %zu\n", usedSize);

//     return curFreeNode;
// }

// void *allocBlock(size_t size, freeListNode *curFreeNode) {
//     size_t newSize = 8 * (((size - 1) / 8) + 1);
//     header *prev = (header*) (curFreeNode - 1);
//     if (prev->size - 32 < newSize) {
//         return 0;
//     }
//     else {
//         return makeBlock(newSize, curFreeNode);
//     }
// }

// void* malloc(size_t bytes) {
//     if (bytes > HEAP_SIZE) {
//         return 0;
//     }
    
//     if (mCount == 0) {
//         initFreeList();
//     }
//     mCount++;

//     freeListNode *curFreeNode = freeList;
//     while (curFreeNode != 0) {
//         void *temp = allocBlock(bytes, curFreeNode);
//         if (temp) {
//             return temp;
//         }
//         curFreeNode = curFreeNode->next;
//     }
    
//     return 0;
// }

// void combine(header *firstHead, header *secondHead) {

// }

// void merge(freeListNode* curFreeListNode) {

// }

// void free(void* p) {
//     fCount++;

//     if (p == 0 || p == NULL) return;
    
//     header *curHeader = (header*) (p - 8);
//     size_t curSize = curHeader->size;
//     header *curFooter = (header*) (p + curSize);

//     curHeader->isUsed = 0;
//     curFooter->isUsed = 0;

//     makeNode(0, p);
//     makeNode(endList - 8, p + 8);
//     endList->next =(freeListNode*) p;
//     endList = (freeListNode*) (p + 8);

//     // merge((freeListNode*) p);
// }