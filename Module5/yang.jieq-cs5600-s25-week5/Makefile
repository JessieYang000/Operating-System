CC = gcc
CFLAGS = -Wall -Wextra -g

all: queuetest

queuetest: queue.o queuetest.o
	$(CC) $(CFLAGS) -o queuetest queue.o queuetest.o

queue.o: queue.c queue.h
	$(CC) $(CFLAGS) -c queue.c

queuetest.o: queuetest.c queue.h process.h
	$(CC) $(CFLAGS) -c queuetest.c

clean:
	rm -f *.o queuetest