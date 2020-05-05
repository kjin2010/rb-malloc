#include "heap.h"
#include "debug.h"
#include <stdlib.h>

volatile long mCount = 0;
volatile long fCount = 0;

long heapBreak = 0;

long ourBrk(long k){
    heapBreak = k;
    return heapBreak;
}

long ourSbrk(long k){
    heapBreak += k;
    return heapBreak;
}

long m = 0;

//negative means free, positive means allocated in header and footer
void* malloc(size_t bytes){
    if(bytes %8!=0)
        bytes+= 8-(bytes%8);
    if(m == 0){
        the_heap[0] =(long) bytes/8;
        if((long)bytes/8+1 >= HEAP_SIZE/sizeof(long)) {
            mCount++;
            return 0;
        }
        the_heap[(long)bytes/8+1] = (long) bytes/8;
        heapBreak = (long)bytes/8+1;
        m++;
        mCount++;
        return the_heap+1;
    }
    else{
        long curr = 0;
        while(curr < heapBreak){
            if(the_heap[curr] <= 0 && (long)bytes/8 <= -the_heap[curr]-2){
                the_heap[curr+(long)bytes/8+2] = the_heap[curr]+(long)bytes/8+2;
                the_heap[curr-the_heap[curr]+1] = the_heap[curr]+(long)bytes/8+2;
                 
                the_heap[curr] = (long)bytes/8;
                the_heap[curr+(long)bytes/8+1] = (long)bytes/8;
                
                mCount++;
                return the_heap+(curr+1);
            }
            else if(the_heap[curr] <= 0 && (long)bytes/8 == -the_heap[curr]){
                the_heap[curr] *= -1;
                
                mCount++;
                return the_heap+(curr+1);
            }

            curr = curr+abs(the_heap[curr])+2;
//            assert(curr == 28559, "%d",curr+abs(the_heap[curr])+2, __FILE__, __LINE__);
        }
        //now curr>heapBreak
        the_heap[curr] = (long)bytes/8;
        if(curr + (long)bytes/8 + 1 >= HEAP_SIZE/sizeof(long)) {
            mCount++;
            return NULL;
        }
        the_heap[curr+(long)bytes/8+1] = (long)bytes/8;
        heapBreak = curr+(long)bytes/8+1;
        mCount++;
        return the_heap+(curr+1);
    }
}


void free(void* p){
    fCount++;
    if(p == 0)
        return;
    long diff = ((long)((long) p - (long) the_heap))/sizeof(long);
    long indOfMeta = diff-1;
    the_heap[indOfMeta] *= -1;
    the_heap[indOfMeta-the_heap[indOfMeta]+1] *= -1;
    
    long tempHeader = indOfMeta;
    
    
    while(tempHeader > 0 && the_heap[tempHeader-1] <= 0){
        the_heap[indOfMeta-the_heap[indOfMeta]+1] = the_heap[indOfMeta-the_heap[indOfMeta]+1]-2+the_heap[tempHeader-1];
        tempHeader = tempHeader - 1 + the_heap[tempHeader-1] - 1;
        the_heap[tempHeader] = the_heap[indOfMeta-the_heap[indOfMeta]+1];
    }


    long tempFooter = indOfMeta-the_heap[indOfMeta]+1;
    while(tempFooter < heapBreak && the_heap[tempFooter+1] <= 0){
        the_heap[tempHeader] = the_heap[tempHeader]-2+the_heap[tempFooter+1];
        tempFooter = tempFooter+1-the_heap[tempFooter+1]+1;
        the_heap[tempFooter] = the_heap[tempHeader];
    
    }
    
}
