all: program_1 program_2 program_3 program_4 program_5

program_1: mysh.c
	gcc -g -std=c99 -Wall -Wvla -fsanitize=address,undefined mysh.c -o mysh

program_2: testing.c
	gcc -g -std=c99 -Wall -Wvla -fsanitize=address,undefined testing.c -o testing

program_3: inf.c
	gcc -g -std=c99 -Wall -Wvla -fsanitize=address,undefined inf.c -o inf

program_4: pipeTest.c
	gcc -g -std=c99 -Wall -Wvla -fsanitize=address,undefined pipeTest.c -o pipeTest

program_5: testingOutput.c
	gcc -g -std=c99 -Wall -Wvla -fsanitize=address,undefined testingOutput.c -o testingOutput
	
clean:
	rm -rf mysh testing inf pipeTest testingOutput *-

