#include "kmem.h"
#define HEAPSTART (void*)0x0

void* memoryMarker = HEAPSTART;

void* malloc(unsigned int size)
{
    MemBlock start = createBlock(memoryMarker,size);
    void* end = start.end;
    memoryMarker = ++end;
    return start.start;
}