.PHONY: all testqueue clean

TOP=.

all: o1_queue.so test run

CFLAGS = $(CFLAG)
CFLAGS += -g3 -O2 -rdynamic -Wall -fPIC -shared -lm

o1_queue.so: o1_queue.c
	gcc $(CFLAGS) -o $@ $^

clean:
	rm -f o1_queue.so testqueue test

testqueue:
	gcc -lm -fsanitize=address -ggdb -DDEBUG o1_queue.c testqueue.c -o testqueue

test:
	gcc -lm $(CFLAG) o1_queue.c testqueue.c -o testqueue

run:
	-./testqueue.exe
