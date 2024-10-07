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
    for (int i = 0; i < 1; i++)
    {
        /*
        //Test Case 1
        for (int j = 0; j < 120; j++)
        {
            int *a = malloc(1);
            free(a);
        }
        //Test Case 2
        int **arr[120];
        for (int j = 0; j < 120; j++)
        {
            arr[j] = malloc(1);
        }
        for(int j = 0; j<120;j++){
            free(arr[j]);
        }
        */
        //Test Case 3
        int count = 0;
        int j = 0;
        int **arrTwo[120];
        while (j < 120)
        {
            int choice = rand() % 2;
            if ((choice == 1) || (count == 0))
            {
                arrTwo[count] = malloc(1);
                count++;
                j++;
            }
            else
            {
                count--;
                free(arrTwo[count]);
            }
        }
        for(int j = 0; j<count;j++){
            free(arrTwo[count]);
        }
        /*
        //Test Case 4
        int** arrThree[120];
        for(int j = 0; j<120;j++){
            arrThree[j] = malloc(1);
        }
        for(int j = 0; j<60;j++){
            free(arrThree[j*2]);
        }
        for(int j = 0; j<60;j++){
            free(arrThree[j*2+1]);
        }
        
        //Test Case 5
        for(int j = 0; j<60;j++){
            arrThree[j] = malloc(j);
        }
        for(int j = 0; j<60;j++){
            free(arrThree[j]);
        }
        */
    }
    exit(0);
}