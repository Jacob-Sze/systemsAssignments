#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#ifndef REALMALLOC
#include "mymalloc.h"
#endif

struct oLL
{
    int id;
    int id1;
    int id2;
    struct oLL *next;   
} __attribute__((packed));

int main(int argc, char **argv)
{
    printf("Size of struct LL node %lu\n", sizeof(struct oLL));
    struct oLL* node = malloc(sizeof(struct oLL));
    printf("Leak below shows that the 20 byte struct is aligned to 8 bytes (24 bytes) plus the 8 byte header, leading to 32 byte leak.\n");
    
}
