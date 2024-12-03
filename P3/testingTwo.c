#include <stdio.h>
#include <signal.h>

int main() {
    raise(SIGINT);
    return 1;
}