CC=clang
CFLAGS=-O3 -Wall -Wextra

all:
	mkdir -p bin
	$(CC) $(CFLAGS) hanoi.c -o bin/hanoi
	$(CC) $(CFLAGS) test.c -o bin/test
	$(CC) $(CFLAGS) uarmin.c -o bin/uarmin

clean:
	rm -rf bin
