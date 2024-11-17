all: program_1 program_2

program_1: mysh.c
	gcc -g -std=c99 -Wall -Wvla -fsanitize=address,undefined mysh.c -o mysh

program_2: testing.c
	gcc -g -std=c99 -Wall -Wvla -fsanitize=address,undefined testing.c -o testing

clean:
	rm -rf mysh testing *-

