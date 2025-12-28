.PHONY: all clean

all: hexeditplus

hexeditplus: task1.c
	gcc -m32 -Wall -g -o hexeditplus task1.c

clean:
	rm -f hexeditplus task4 *.o

