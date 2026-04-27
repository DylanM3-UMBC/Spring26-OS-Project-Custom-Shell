build:
	gcc -g -Wall shell.c -o shell

run:
	./shell

ValRun:
	valgrind ./shell

clean:
	rm -f shell