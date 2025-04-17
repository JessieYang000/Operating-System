# Compiler and flags
CC = gcc
CFLAGS = -Wall

# Shared sources
UTILS = fs_utils.c
HEADERS = fs_utils.h

# Targets
all: server client

server: server.c $(UTILS) $(HEADERS)
	$(CC) $(CFLAGS) server.c $(UTILS) -o server

client: client.c $(UTILS) $(HEADERS)
	$(CC) $(CFLAGS) client.c $(UTILS) -o client

# Clean up build artifacts
clean:
	rm -f server client data/permission.txt
	rm -rf data2 data3 server_root 	

