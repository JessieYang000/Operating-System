# Compiler and Flags
CC = gcc
CFLAGS = -Wall -Werror -g

# Source and Object Files for allocmem
ALLOCMEM = allocmem.c
ALLOCMEM_OBJ = allocmem.o
ALLOCMEM_TARGET = allocmem

# Source and Object Files for loadmem
LOADMEM_SRC = loadmem.c
LOADMEM_OBJ = loadmem.o
LOADMEM_TARGET = loadmem

# Default Rule: Build both executables
all: $(ALLOCMEM_TARGET) $(LOADMEM_TARGET)

# Rule to compile allocmem
$(ALLOCMEM_TARGET): $(ALLOCMEM_OBJ)
	$(CC) $(CFLAGS) $(ALLOCMEM_OBJ) -o $(ALLOCMEM_TARGET)

# Rule to compile loadmem
$(LOADMEM_TARGET): $(LOADMEM_OBJ)
	$(CC) $(CFLAGS) $(LOADMEM_OBJ) -o $(LOADMEM_TARGET)

# Compile source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean compiled files
clean:
	rm -f $(ALLOCMEM_OBJ) $(ALLOCMEM_TARGET) $(LOADMEM_OBJ) $(LOADMEM_TARGET)
