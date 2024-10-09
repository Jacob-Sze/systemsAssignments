#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include "mymalloc.h"

#define MEMLENGTH 4096

static int init = 0;

static union
{
    char bytes[MEMLENGTH];
    double not_used;
} heap;

/*
Stuff we have to do:
- Make test programs
*/

// Struct for header information
typedef struct chunk
{
    int size;
    int allocated;
} Chunk;

// Leak Checker function
static void leakChecker(void)
{
    int count = 0;
    int size = 0;
    int i = 0;
    Chunk *a = (Chunk*)(&(heap.bytes[i]));
    while (i < MEMLENGTH)
    {
        a = (Chunk*)(&heap.bytes[i]);
        if (a->allocated == 1)
        {
            count++;
            size += a->size;
        }
        i += a->size;
    }
    if (count != 0)
    {
        printf("mymalloc: %d bytes leaked in %d objects\n", size, count);
    }

}

// Initialization function
void inital()
{
    init = 1;
    Chunk* a = (Chunk*)(&heap.bytes[0]);
    a->size = MEMLENGTH;
    a->allocated = 0;
    atexit(leakChecker);
}



// Malloc function
void *mymalloc(size_t size, char *file, int line)
{
    
    if (init == 0)
    {
        inital();
    }
    if(size == 0) {
        size += 8;
    }
    // Alignment for size
    if (size % 8 != 0)
    {
        size += 8 - size % 8;
    }
    size += 8;
    // Moves on until we reach an empty slot inside of the array
    int i = 0;
    Chunk *a = (Chunk*)(&heap.bytes[i]);

    while (i < MEMLENGTH && (a->allocated != 0 && a->size >= size))
    {
        i += a->size;
        a = (Chunk*)(&heap.bytes[i]);
    }
    if (i + size > MEMLENGTH)
    {
        printf("malloc: Unable to allocate %ld bytes (%s:%d)\n", size, file, line);
        return NULL;
    }
    // Allocates item inside of heap array
    Chunk *node = (Chunk*)(&heap.bytes[i]);
    int prevSize = node->size;
    node->size = size;
    node->allocated = 1;
    int j = i + size;
    // Decreases size and reallocates for free chunk
    if (prevSize != size)
    {
        Chunk *nodeTwo = (Chunk*)(&heap.bytes[j]);
        nodeTwo->size = prevSize - size;
        nodeTwo->allocated = 0;
    }
    return &heap.bytes[i+8];
}

// Free function
void myfree(void *ptr, char *file, int line)
{
    if (init == 0)
    {
        inital();
    }
    int i = 0;
    int found = 0;
    int sizes = 0;
    int start = 0;
    int checker = 0;
    // Moves on until we reach the end or the ptr is equal to one of the chunks
    while (i < MEMLENGTH)
    {
        Chunk *a = (Chunk*)(&heap.bytes[i]);
        if (a->allocated == 0)
        {
            if (checker == 0)
            {
                checker = 1;
                start = i;
            }
        }
        else
        {
            checker = 0;
        }
        // Frees chunk and coalescing free chunks
        if (((&heap.bytes[i+8]) == ptr) && (a->allocated == 1))
        {
            found = 1;
            a->allocated = 0;
            if (checker == 0)
            {
                start = i;
            }
            int j = start;
            Chunk *b = (Chunk*)(&heap.bytes[j]);
            while (j < MEMLENGTH && b->allocated == 0)
            {
                sizes += b->size;
                j += b->size;
                b->size = 0;
                b = (Chunk*)(&heap.bytes[j]);
            }
            Chunk *c = (Chunk*)(&heap.bytes[start]);
            c->size = sizes;
            c->allocated = 0;
            break;
        }
        i += a->size;
    }
    // If the pointer was not found
    if (found != 1)
    {
        printf("free: Inappropriate pointer (%s:%d)\n", file, line);
        exit(2);
    }
}
