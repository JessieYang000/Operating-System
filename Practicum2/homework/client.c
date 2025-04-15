/*
 * client.c -- TCP Socket Client
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
#include "fs_utils.h"

// Validate the local file path before sending the file to the server
int file_exists(const char *path)
{
  struct stat st;
  return stat(path, &st) == 0 && S_ISREG(st.st_mode); // Check existence of the file and whether it's a regular file
}

int main(void)
{
  int socket_desc;
  struct sockaddr_in server_addr;
  char server_message[2000], client_message[2000];

  // Create socket:
  socket_desc = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_desc < 0)
  {
    printf("Unable to create socket\n");
    return -1;
  }

  printf("Socket created successfully\n");

  // Set port and IP the same as server-side:
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(2000);
  server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

  // Send connection request to server:
  if (connect(socket_desc, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
  {
    printf("Unable to connect\n");
    close(socket_desc);
    return -1;
  }

  printf("Connected with server successfully\n");

  while (1)
  {
    // Clean buffers:
    memset(server_message, '\0', sizeof(server_message));
    memset(client_message, '\0', sizeof(client_message));

    // Get input from the user:
    printf("Enter message: ");
    fgets(client_message, sizeof(client_message), stdin);
    client_message[strcspn(client_message, "\n")] = '\0'; // Remove newline

    // Parse rfs WRITE command
    if (strncmp(client_message, "rfs WRITE ", 10) == 0)
    {
      char cmd[5], action[6], local_path[1024], remote_path[1024];
      int matched = sscanf(client_message, "%s %s %1023s %1023s", cmd, action, local_path, remote_path);

      if (matched != 4)
      {
        printf("Invalid format. Usage: rfs WRITE <local-path> <remote-path>\n");
        continue;
      }

      if (!file_exists(local_path))
      {
        printf("Local file '%s' does not exist.\n", local_path);
        continue;
      }

      // Send WRITE header
      char header[2048];
      snprintf(header, sizeof(header), "WRITE %s\n", remote_path);
      send(socket_desc, header, strlen(header), 0);

      // Send file content
      FILE *fp = fopen(local_path, "rb");
      char buffer[1024];
      size_t bytes_read;
      while ((bytes_read = fread(buffer, 1, sizeof(buffer), fp)) > 0)
      {
        send(socket_desc, buffer, bytes_read, 0);
      }
      fclose(fp);
      shutdown(socket_desc, SHUT_WR); // Indicate sending is done
    }
    // Handle rfs GET command
    else if (strncmp(client_message, "rfs GET ", 8) == 0)
    {
      char cmd[5], action[4], remote_path[1024], local_path[1024];
      int matched = sscanf(client_message, "%s %s %1023s %1023s", cmd, action, remote_path, local_path);

      if (matched != 4)
      {
        printf("Invalid format. Usage: rfs GET <remote-path> <local-path>\n");
        continue;
      }

      // Send GET header
      char header[2048];
      snprintf(header, sizeof(header), "GET %s\n", remote_path);
      send(socket_desc, header, strlen(header), 0);

      // Read the first chunk of response
      char buffer[1024];
      ssize_t bytes_received = recv(socket_desc, buffer, sizeof(buffer), 0);
      if (bytes_received <= 0)
      {
        printf("Error receiving response from server.\n");
        close(socket_desc);
        return -1;
      }

      // Check if it's an error message from server
      if (strncmp(buffer, "FAILED:", 7) == 0)
      {
        buffer[bytes_received] = '\0';        // Null-terminate
        printf("Server error: %s\n", buffer); // Display error
        continue;                             // Go back to prompt
      }

      // Ensure directories in local path exist (like mkdir -p)
      ensure_path_exists(local_path);

      // Open local file for writing
      FILE *fp = fopen(local_path, "ab");
      if (!fp)
      {
        printf("Failed to open local file '%s' for writing.\n", local_path);
        close(socket_desc);
        continue;
      }

      // Write the first chunk already received
      fwrite(buffer, 1, bytes_received, fp);

      // Continue receiving the rest of the file
      while ((bytes_received = recv(socket_desc, buffer, sizeof(buffer), 0)) > 0)
      {
        fwrite(buffer, 1, bytes_received, fp);
      }

      fclose(fp);
      printf("File saved to %s\n", local_path);

      // Close connection and exit after GET
      close(socket_desc);
      return 0;
    }
    // Handle rfs RM command
    else if (strncmp(client_message, "rfs RM ", 7) == 0)
    {
      char cmd[5], action[4], remote_path[1024];
      char extra[1024];
      int matched = sscanf(client_message, "%s %s %1023s %1023s", cmd, action, remote_path, extra);

      if (matched != 3)
      {
        printf("Invalid format. Usage: rfs RM <remote-path>\n");
        continue;
      }

      // Send RM header
      char header[2048];
      snprintf(header, sizeof(header), "RM %s\n", remote_path);
      send(socket_desc, header, strlen(header), 0);

      // Read the first chunk of response
      char buffer[1024];
      ssize_t bytes_received = recv(socket_desc, buffer, sizeof(buffer), 0);
      if (bytes_received <= 0)
      {
        printf("Error receiving response from server.\n");
        close(socket_desc);
        return -1;
      }

      // Check if it's an error message from server
      if (strncmp(buffer, "FAILED:", 7) == 0)
      {
        buffer[bytes_received] = '\0';        // Null-terminate
        printf("Server error: %s\n", buffer); // Display error
        close(socket_desc);
        continue; // Go back to prompt
      }
      printf("Successfully removed file from server.\n");
      // Close connection and exit after GET
      close(socket_desc);
      return 0;
    }
    else
    {
      // Send regular message
      if (send(socket_desc, client_message, strlen(client_message), 0) < 0)
      {
        printf("Unable to send message\n");
        close(socket_desc);
        return -1;
      }
    }

    // Receive the server's response:
    if (recv(socket_desc, server_message, sizeof(server_message), 0) < 0)
    {
      printf("Error while receiving server's msg\n");
      close(socket_desc);
      return -1;
    }

    printf("Server's response: %s\n", server_message);

    // Close the socket and exit:
    close(socket_desc);
    return 0;
  }
}
