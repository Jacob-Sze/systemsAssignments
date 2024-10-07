all: program_1 program_2 program_3 program_4 program_5 program_6

program_1: badMalloc.c
	gcc -g -std=c99 -Wall -Wvla -fsanitize=address,undefined mymalloc.c mymalloc.h badMalloc.c -o badMalloc

program_2: memgrind.c
	gcc -g -std=c99 -Wall -Wvla -fsanitize=address,undefined mymalloc.c mymalloc.h memgrind.c -o memgrind

program_3: leakTester.c
	gcc -g -std=c99 -Wall -Wvla -fsanitize=address,undefined mymalloc.c mymalloc.h leakTester.c -o leakTester

program_4: freeOne.c
	gcc -g -std=c99 -Wall -Wvla -fsanitize=address,undefined mymalloc.c mymalloc.h freeOne.c -o freeOne

program_5: freeTwo.c
	gcc -g -std=c99 -Wall -Wvla -fsanitize=address,undefined mymalloc.c mymalloc.h freeTwo.c -o freeTwo

program_6: freeThree.c
	gcc -g -std=c99 -Wall -Wvla -fsanitize=address,undefined mymalloc.c mymalloc.h freeThree.c -o freeThree

clean:
	rm -rf memtest memgrind badMalloc leakTester freeOne freeTwo freeThree *-
