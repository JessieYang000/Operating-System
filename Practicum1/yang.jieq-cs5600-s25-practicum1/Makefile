# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra

# Output executable name
TARGET = message

# Source files
SRCS = main.c message.c

# Header dependencies
DEPS = message.h uthash.h

# Object files
OBJS = $(SRCS:.c=.o)

# Default target
all: $(TARGET)

# Link object files to create executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Compile .c files to .o files
%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

# Clean up build artifacts and generated files
clean:
	rm -f $(OBJS) $(TARGET)
	rm -rf messages

