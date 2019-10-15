#include "memory.h"

#define HEAPSTART (void*)0xFF
#define HEAPMAX 10000
#define MAX_ALLOCS 1024

typedef struct {
    void *start;
    size_t size;
    char freed;
} AddressRecord;

AddressRecord records[MAX_ALLOCS];
size_t used_records = 0;

void memzero(void *dest, size_t count)
{
    char *destCp = (char*)dest;
    for (size_t i = 0; i < count; i++)
    {
        destCp[i] = (char)0;
    }
}

void memcpy(void *dest, void *src, size_t count)
{
    char *destCp = (char*)dest;
    char *srcCp = (char*)src;
    for (size_t i = 0; i < count; i++)
    {
        destCp[i] = srcCp[i];
    }
}

int mem_inuse(size_t addr)
{
    for (size_t i = 0; i < used_records; i++)
    {
        AddressRecord *record = &records[i];
        if (record->freed)
        {
            continue;
        }
        if (record->start <= addr && record->start + record->size > addr)
        {
            return 1;
        }
    }
    return 0;
}

void* find_mem_space(size_t size)
{
    for (size_t i = HEAPSTART; i < HEAPMAX; i++)
    {
        if (i + size >= HEAPMAX)
        {
            return (void*)0;
        }
        if (!mem_inuse(i))
        {
            int has_enough = 1;
            for (size_t j = i; j < size; j++)
            {
                if (mem_inuse(j))
                {
                    has_enough = 0;
                    break;
                }
            }
            if (has_enough)
            {
                return (void*)i;
            }
        }
    } 
}

void add_mem_record(void* start, size_t size)
{
    AddressRecord record;
    record.size = size;
    record.start = start;
    record.freed = 0;
    for (size_t i = 0; i < used_records; i++)
    {
        if (records[i].freed)
        {
            records[i] = record;
            return;
        }
    }
    if (used_records + 1 >= MAX_ALLOCS)
    {
        // TODO: Segfault
    }
    records[used_records] = record;
    used_records++;
}

void* malloc(size_t size)
{
    void* start = find_mem_space(size);
    if (start == (void*)0)
    {
        // TODO: Segfault
    }
    add_mem_record(start, size);
}

void free(void* addr)
{
    for (size_t i = 0; i < used_records; i++)
    {
        if (records[i].start == addr)
        {
            records[i].freed = 1;
            return;
        }
    }
    // TODO: Segfault
}