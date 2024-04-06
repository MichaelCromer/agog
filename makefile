
all: main

main: src/main.c 
	gcc src/main.c src/project.c src/clock.c  src/task.c -o agogo

debug: src/main.c
	gcc -g src/main.c -o agogo

clean:
	rm -f agogo

