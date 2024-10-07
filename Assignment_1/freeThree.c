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
    int *p = malloc(sizeof(int)*100);
    int *q = p;
    free(p);
    free(q);
    exit(0);
}