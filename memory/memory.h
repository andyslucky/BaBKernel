#ifndef memory_h
#define memory_h

#ifndef size_t
    #define size_t unsigned long
#endif

void* malloc(size_t);
void free(void *data);
void memcpy(void *dest, void *src, size_t count);
void memzero(void *dest, size_t count);

#endif