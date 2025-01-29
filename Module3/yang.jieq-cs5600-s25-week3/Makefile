# Compiler and Flags
CC = gcc
CFLAGS = -Wall -Werror -g

# Source and Object Files for genrand
GENRAND_SRC = genrand.c lcg.c
GENRAND_OBJ = genrand.o lcg.o
GENRAND_TARGET = genrand

# Source and Object Files for cipher
CIPHER_SRC = cipher.c polybius.c
CIPHER_OBJ = cipher.o polybius.o
CIPHER_TARGET = cipher

# Default Rule: Build both executables
all: $(GENRAND_TARGET) $(CIPHER_TARGET)

# Rule to compile genrand
$(GENRAND_TARGET): $(GENRAND_OBJ)
	$(CC) $(CFLAGS) $(GENRAND_OBJ) -o $(GENRAND_TARGET)

# Rule to compile cipher
$(CIPHER_TARGET): $(CIPHER_OBJ)
	$(CC) $(CFLAGS) $(CIPHER_OBJ) -o $(CIPHER_TARGET)

# Compile source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean compiled files
clean:
	rm -f $(GENRAND_OBJ) $(CIPHER_OBJ) $(GENRAND_TARGET) $(CIPHER_TARGET)
