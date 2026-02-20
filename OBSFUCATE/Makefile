CC = gcc
CFLAGS = -Wall -Wextra -O2

patch: patcher.c
	$(CC) $(CFLAGS) -o patch patcher.c
	chmod 777 patcher.c

clean:
	rm -f patch
