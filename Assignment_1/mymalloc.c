#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mymalloc.h"

#define MEMLENGTH 4096
static union {
char bytes[MEMLENGTH];
double not_used;
} heap;

/*
Stuff we have to do:
- Make test programs
- Check for objects in heap after program ends
- Account for alignment
- (Add stuff here if need to)
*/

//Struct for header information
typedef struct chunk{
    size_t size;
} Chunk;

//Malloc function
void *mymalloc(size_t size, char *file, int line){
    //Gets the line malloc was called in
    FILE *file = fopen(file, "r");
    int i = 1;
    while(i != line){
        fscanf(file, "%*[^\n]\n");
        i++;
    }
    char lines[500];
    fscanf(file, lines);
    //Moves on until we reach an empty slot inside of the array
    i = 0;
    while(heap.bytes[i] != 0){
        Chunk* a = heap.bytes[i];
        i += a->size;
    }
    //Allocates item inside of heap array
    Chunk* node = heap.bytes[i];
    node->size = size;
    return node;
}

//Free function
void myfree(void *ptr, char *file, int line){
    //Gets the line free was called in
    FILE *file = fopen(file, "r");
    int i = 1;
    while(i != line){
        fscanf(file, "%*[^\n]\n");
        i++;
    }
    char lines[500];
    fscanf(file, lines);
    i = 0;
    int found = 0;
    //Moves on until we reach an empty slot inside of the array or the ptr is equal to one of the items
    while(heap.bytes[i] != 0){
        Chunk* a = heap.bytes[i];
        if(a == ptr){
            found = 1;
            int j = i+a->size;
            //Replacing the item and manipulating the array to have no weird free space after the removal
            while(heap.bytes[j]!=0){
                Chunk* b = heap.bytes[j];
                int b_size = b -> size;
                heap.bytes[i] = NULL;
                memcpy(heap.bytes[i], heap.bytes[j], b_size);
                heap.bytes[j] = NULL;
                i += b->size;
                j += b->size;
            }
            break;
        }
        i += a->size;
    }
    //If the pointer was not found
    if(found != 1){
        printf("free: Inappropriate pointer (%s:%d)",file, line);
    }
}
