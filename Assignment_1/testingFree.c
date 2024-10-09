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
    int key1;
    int key2;
    int key3; 
    struct oLL *next;
};

int main(int argc, char **argv)
{
    struct oLL** objHolder = malloc(10*sizeof(struct oLL*));
    for(int i = 0; i < 10; i++) {
        objHolder[i] = NULL;
    }
    for(int i = 0; i < 10; i++) {
        objHolder[i] = malloc(sizeof(struct oLL));
    }
    for(int i = 0; i < 10; i++) {
        objHolder[i]->id = i;
        objHolder[i]->key1 = i+1;
        objHolder[i]->key2 = i+2;
        objHolder[i]->key3 = i+3;
    }
    for(int i = 0; i < 10; i++) {
       printf("Expected ID/Keys: %d %d %d %d   Actual ID/Keys:  %d %d %d %d \n", i, i+1, i+2, i+3, objHolder[i]->id, objHolder[i]->key1, objHolder[i]->key2, objHolder[i]->key3);
    }
    for(int i = 0; i < 10; i++) {
        free(objHolder[i]);
    }
    free(objHolder);
}