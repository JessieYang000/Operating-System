/*
 * server.c -- TCP Socket Server
 *
 * adapted from:
 *   https://www.educative.io/answers/how-to-implement-tcp-sockets-in-c
 */

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>

#define BUFFER_SIZE 8196
#define ROOT_DIR "./server_root" // Define server root for file writes

// Helper: Recursively create directories in a given path
void ensure_directory(const char *path)
{
  char temp[1024];
  strcpy(temp, path);
  for (char *p = temp + 1; *p; p++)
  {
    if (*p == '/')
    {
      *p = '\0';
      mkdir(temp, 0755);
      *p = '/';
    }
  }
}

int main(void)
{
  int socket_desc, client_sock;
  socklen_t client_size;
  struct sockaddr_in server_addr, client_addr;
  char server_message[BUFFER_SIZE], client_message[BUFFER_SIZE];

  // Create socket:
  socket_desc = socket(AF_INET, SOCK_STREAM, 0);

  if (socket_desc < 0)
  {
    printf("Error while creating socket\n");
    return -1;
  }
  printf("Socket created successfully\n");

  // Set port and IP:
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(2000);
  server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

  // Bind to the set port and IP:
  if (bind(socket_desc, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
  {
    printf("Couldn't bind to the port\n");
    return -1;
  }
  printf("Done with binding\n");

  // Listen for clients:
  if (listen(socket_desc, 1) < 0)
  {
    printf("Error while listening\n");
    close(socket_desc);
    return -1;
  }
  printf("\nListening for incoming connections.....\n");

  mkdir(ROOT_DIR, 0755); // Ensure root directory exists

  // Loop for keeping the server running all the time
  while (1)
  {
    // Clean buffers:
    memset(client_message, '\0', sizeof(client_message));
    memset(server_message, '\0', sizeof(server_message));
    
    // Accept an incoming connection:
    client_size = sizeof(client_addr);
    client_sock = accept(socket_desc, (struct sockaddr *)&client_addr, &client_size);

    if (client_sock < 0)
    {
      printf("Can't accept\n");
      continue;
    }
    printf("Client connected at IP: %s and port: %i\n",
           inet_ntoa(client_addr.sin_addr),
           ntohs(client_addr.sin_port));

    // Receive client's message:
    if (recv(client_sock, client_message,
             sizeof(client_message), 0) < 0)
    {
      printf("Couldn't receive\n");
      close(client_sock);
      continue;
    }
    // Check if it's a WRITE command
    if (strncmp(client_message, "WRITE ", 6) == 0) // [MODIFIED]
    {
      char *remote_path = client_message + 6;
      remote_path[strcspn(remote_path, "\n")] = '\0'; // Remove newline if present

      char full_path[1024];
      snprintf(full_path, sizeof(full_path), "%s/%s", ROOT_DIR, remote_path); // Prepend root directory

      ensure_directory(full_path); // [ADDED] Make sure directories exist

      FILE *fp = fopen(full_path, "ab"); // Append to avoid overwriting
      if (!fp)
      {
        snprintf(server_message, sizeof(server_message), "FAILED: Could not open file for writing.\n");
        send(client_sock, server_message, strlen(server_message), 0);
        close(client_sock);
        continue;
      }

      // Read file content from client after header
      char buffer[1024];
      ssize_t bytes_received;
      while ((bytes_received = recv(client_sock, buffer, sizeof(buffer), 0)) > 0)
      {
        fwrite(buffer, 1, bytes_received, fp);
      }

      fclose(fp);

      snprintf(server_message, sizeof(server_message),
               "SUCCESS: File written to %s\n", full_path);
    }
    else
    {
      // Respond to non-WRITE messages (e.g., regular chat)
      strcpy(server_message, "This is the server's response message.");
    }

    // Send response to client:
    send(client_sock, server_message, strlen(server_message), 0);

    // Closing the socket:
    close(client_sock);
    printf("Closed connection with client.\n\n");
  }

  // Close the server socket
  close(socket_desc);
  return 0;
}