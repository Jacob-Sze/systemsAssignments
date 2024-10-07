all: program_1 program_2

program_1: memtest.c
	gcc -g -std=c99 -Wall -Wvla -fsanitize=address,undefined mymalloc.c mymalloc.h memtest.c -o memtest

program_2: memgrind.c
	gcc -g -std=c99 -Wall -Wvla -fsanitize=address,undefined mymalloc.c mymalloc.h memgrind.c -o memgrind

clean:
	rm -rf memtest memgrind *-

