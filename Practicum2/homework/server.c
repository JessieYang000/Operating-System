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
#include "fs_utils.h"

#define BUFFER_SIZE 8196
#define ROOT_DIR "./server_root" // Define server root for file writes

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

    // Receive client's message (command or plain text)
    ssize_t msg_len = recv(client_sock, client_message, sizeof(client_message), 0);
    if (msg_len < 0)
    {
      printf("Couldn't receive\n");
      close(client_sock);
      continue;
    }

    // Check if it's a WRITE command
    if (strncmp(client_message, "WRITE ", 6) == 0)
    {
      char *remote_path = client_message + 6;         // Skip "WRITE " prefix to extract the target remote file path
      remote_path[strcspn(remote_path, "\n")] = '\0'; // Remove newline if present

      char full_path[1024];
      snprintf(full_path, sizeof(full_path), "%s/%s", ROOT_DIR, remote_path); // Prepend root directory

      // Ensure all parent directories in remote path exist
      ensure_path_exists(full_path);
      FILE *fp = fopen(full_path, "ab"); // Append to avoid overwriting
      if (!fp)
      {
        snprintf(server_message, sizeof(server_message), "FAILED: Could not open file for writing.\n");
        send(client_sock, server_message, strlen(server_message), 0);
        printf("Failed to open %s for writing.\n", full_path);
        close(client_sock);
        continue;
      }

      printf("Receiving file content for: %s\n", full_path);

      // Read file content from client after header
      char buffer[1024];
      ssize_t bytes_received;
      while ((bytes_received = recv(client_sock, buffer, sizeof(buffer), 0)) > 0)
      {
        fwrite(buffer, 1, bytes_received, fp);
      }

      fclose(fp);
      snprintf(server_message, sizeof(server_message), "SUCCESS: File written to %s\n", full_path);
      printf("File saved successfully to %s\n", full_path);
    }
    // Handle GET command
    else if (strncmp(client_message, "GET ", 4) == 0)
    {
      char *remote_path = client_message + 4; // Skip "GET " prefix
      remote_path[strcspn(remote_path, "\n")] = '\0'; // Remove newline if present

      char full_path[1024];
      snprintf(full_path, sizeof(full_path), "%s/%s", ROOT_DIR, remote_path); // Prepend root directory

      FILE *fp = fopen(full_path, "rb");
      if (!fp)
      {
        snprintf(server_message, sizeof(server_message), "FAILED: File not found: %s\n", full_path);
        send(client_sock, server_message, strlen(server_message), 0);
        printf("File not found: %s\n", full_path);
        close(client_sock);
        continue;
      }

      printf("Sending file: %s\n", full_path);
      char buffer[1024];
      size_t bytes_read;
      while ((bytes_read = fread(buffer, 1, sizeof(buffer), fp)) > 0)
      {
        send(client_sock, buffer, bytes_read, 0);
      }

      fclose(fp);
      printf("File sent successfully.\n");
    }
    else
    {
      // Respond to plain messages
      printf("Received message: %s\n", client_message);
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
