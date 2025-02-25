CC = gcc

CFLAGS = -Wall -pthread -g

# Executable names
TARGETS = multithread pqueuepmain

SRCS = multithread.c pqueuepmain.c

OBJS = $(SRCS:.c=.o)

# Default rule: build all targets
all: $(TARGETS)

# Build multithread
multithread: multithread.o
	$(CC) $(CFLAGS) -o $@ $<

# Build pqueuepmain
pqueuepmain: pqueuepmain.o
	$(CC) $(CFLAGS) -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up generated files
clean:
	rm -f $(TARGETS) $(OBJS)

# Run the programs
run-multithread: multithread
	./multithread

run-pqueuepmain: pqueuepmain
	./pqueuepmain
