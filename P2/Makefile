all: program_1

program_1: words.c
	gcc -g -std=c99 -Wall -Wvla -fsanitize=address,undefined words.c -o words

clean:
	rm -rf words *-
