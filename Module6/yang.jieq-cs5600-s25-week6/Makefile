# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -g

# Source files
SRC = main.c polybius.c queue.c strings.c
OBJ = $(SRC:.c=.o)

# Header files
DEPS = polybius.h queue.h strings.h

# Output executable
TARGET = main

# Default rule: build the executable
all: $(TARGET)

# Compile the main target
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

# Compile each .c file into a .o object file
%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up object files and executable
clean:
	rm -f $(OBJ) $(TARGET)

# # Run the program with arguments
# run: $(TARGET)
# 	./$(TARGET) 10000 10 output.txt
