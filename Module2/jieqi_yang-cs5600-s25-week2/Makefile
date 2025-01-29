CC = gcc
CFLAGS = -Wall -Werror

# Default target to build all executalbes
all: learnc arger

# Target for learnc executable
learnc: learnc.o mpg2km.o
	$(CC) $(CFLAGS) learnc.o mpg2km.o -o learnc

learnc.o: learnc.c mpg2km.h
	$(CC) $(CFLAGS) -c learnc.c

mpg2km.o: mpg2km.c mpg2km.h
	$(CC) $(CFLAGS) -c mpg2km.c

# Target for arger executable
arger: arger.o
	$(CC) $(CFLAGS) arger.o -o arger
arger.o: arger.c
	$(CC) $(CFLAGS) -c arger.c
# Clean up all build files
clean:
	rm -f *.o learnc arger

