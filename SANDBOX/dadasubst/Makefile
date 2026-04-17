CC = gcc
CFLAGS = -Og -g

all: worker

worker: worker.c ipc.o
	$(CC) $(CFLAGS) -o worker worker.c ipc.o dadasubst.o

ipc.o: ipc.c ipc.h
	$(CC) $(CFLAGS) -c ipc.c

clean:
	rm -f worker ipc.o
