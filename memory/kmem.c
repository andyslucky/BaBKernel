#include "kmem.h"
MemBlock createBlock(void* start, unsigned int size){
    MemBlock mem;
    mem.start = start;
    mem.end = (void*)(start + size);
    mem.size = size;
    mem.isFree = 0;
    return mem;
}

void freeMemBlock(MemBlock* mem){
    mem->isFree = 1;
}