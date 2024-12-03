#include <stdio.h>

int main() {
    int number;
    while (scanf("%d", &number) == 1) {
        number += 10;
        printf("%d\n", number);
        
    }

    return 0;
}