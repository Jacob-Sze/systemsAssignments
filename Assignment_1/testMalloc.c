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
    printf("Size of Struct LL node payload returned from malloc %lu\nIntentional leak to show that the 8 byte header is added and that the 20 byte struct still malloc's 24 bytes (no header) \n", sizeof(*node));
    
}