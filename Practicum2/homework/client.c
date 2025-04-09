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

// Validate the local file path before sending the file to the server
int file_exists(const char *path)
{ 
  struct stat st;
  return stat(path, &st) == 0 && S_ISREG(st.st_mode); // Check the existence of the file and it is a regular file
}

int main(void)
{
  int socket_desc;
  struct sockaddr_in server_addr;
  char server_message[2000], client_message[2000];

  // Loop to allow repeated input
  while (1)
  {
    // Clean buffers:
    memset(server_message, '\0', sizeof(server_message));
    memset(client_message, '\0', sizeof(client_message));

    // Get input from the user:
    printf("Enter message: ");
    fgets(client_message, sizeof(client_message), stdin);
    client_message[strcspn(client_message, "\n")] = '\0'; // Remove newline

    // Create socket:
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);

    if (socket_desc < 0)
    {
      printf("Unable to create socket\n");
      continue;
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
      continue;
    }
    printf("Connected with server successfully\n");

    // Parse rfs WRITE command
    if (strncmp(client_message, "rfs WRITE ", 10) == 0)
    {
      char cmd[5], action[6], local_path[1024], remote_path[1024];
      int matched = sscanf(client_message, "%s %s %1023s %1023s", cmd, action, local_path, remote_path);

      if (matched != 4)
      {
        printf("Invalid format. Usage: rfs WRITE <local-path> <remote-path>\n");
        close(socket_desc);
        continue;
      }

      if (!file_exists(local_path))
      {
        printf("Local file '%s' does not exist.\n", local_path);
        close(socket_desc);
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
      shutdown(socket_desc, SHUT_WR);
    }
    else
    {
      // Send regular message
      if (send(socket_desc, client_message, strlen(client_message), 0) < 0)
      {
        printf("Unable to send message\n");
        close(socket_desc);
        continue;
      }
    }

    // Receive the server's response:
    if (recv(socket_desc, server_message, sizeof(server_message), 0) < 0)
    {
      printf("Error while receiving server's msg\n");
      close(socket_desc);
      continue;
    }

    printf("Server's response: %s\n", server_message);

    // Close the socket:
    close(socket_desc);
  }

  return 0;
}
