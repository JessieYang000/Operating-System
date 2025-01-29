# Compiler and Flags
CC = gcc
CFLAGS = -Wall -Werror -g

# Source and Object Files
SRC = genrand.c lcg.c
OBJ = genrand.o lcg.o
TARGET = genrand

# Default Rule: Build the Executable
all: $(TARGET)

# Compile the executable by linking object files
$(TARGET): $(OBJ)
	$(CC) $(LDFLAGS) $(OBJ) -o $(TARGET)

# Compile source files into object files
%.o: %.c lcg.h
	$(CC) $(CFLAGS) -c $< -o $@

# Clean compiled files
clean:
	rm -f $(OBJ) $(TARGET)
