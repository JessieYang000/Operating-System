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
    close(socket_desc);
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

  // Loop to send multiple messages
  while (1)
  {
    // Clean buffers in each loop iteration
    memset(server_message, '\0', sizeof(server_message));
    memset(client_message, '\0', sizeof(client_message));

    // Get input from the user
    printf("Enter message (type 'exit' to quit): ");
    fgets(client_message, sizeof(client_message), stdin); 
    client_message[strcspn(client_message, "\n")] = '\0'; // Strip newline

    // Send message to the server
    if (send(socket_desc, client_message, strlen(client_message), 0) < 0)
    {
      printf("Unable to send message\n");
      break;
    }

    if (strcmp(client_message, "exit") == 0)
    {
      break;
    }

    // Receive responses from the server
    if (recv(socket_desc, server_message, sizeof(server_message), 0) < 0)
    {
      printf("Error while receiving server's msg\n");
      break;
    }

    printf("Server's response: %s\n", server_message);
  }

  close(socket_desc);
  return 0;
}
