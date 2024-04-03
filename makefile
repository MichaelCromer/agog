
all: main

main: src/main.c
	gcc src/main.c -o agogo

debug: src/main.c
	gcc -g src/main.c -o agogo

clean:
	rm -f agogo

