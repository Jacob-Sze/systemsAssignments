#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#ifndef REALMALLOC
#include "mymalloc.h"
#endif

int main(int argc, char **argv)
{
    int** arr[20];
    for(int i = 0; i<20; i++){
        arr[i] = malloc(1);
    }
    exit(0);
}