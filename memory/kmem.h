typedef struct{
    void* start;
    void* end;
    unsigned int size;
    char isFree;
} MemBlock;

extern MemBlock createBlock(void* start,unsigned int size);
extern void freeMemBlock(MemBlock* mem);

