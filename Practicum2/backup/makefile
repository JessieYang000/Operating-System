all: server client

server: server.c
	gcc server.c -o server

client: client.c
	gcc client.c -o client

# Clean up build artifacts and generated files
clean:
	rm -f server
	rm -f client
