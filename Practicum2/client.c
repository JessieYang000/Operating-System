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

#define CHUNK_SIZE 1024
#define FILE_TO_SEND "myfile.txt"

int main(void)
{
  int socket_desc;
  struct sockaddr_in server_addr;
  char buffer[CHUNK_SIZE]; // use chunk buffer for sending file
  FILE *fp;                // file pointer

  fp = fopen(FILE_TO_SEND, "rb");
  if (fp == NULL)
  {
    perror("File open failed");
    return 1;
  }

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
  printf("Sending file: %s\n", FILE_TO_SEND);

  // send file content in chunks
  size_t bytes_read;
  while ((bytes_read = fread(buffer, 1, CHUNK_SIZE, fp)) > 0)
  {
    if (send(socket_desc, buffer, bytes_read, 0) < 0)
    {
      perror("Send failed");
      fclose(fp);
      close(socket_desc);
      return 1;
    }
  }

  // send EOF marker to end the transfer
  send(socket_desc, "EOF", 3, 0);

  printf("File sent successfully.\n");

  // cleanup
  fclose(fp);
  close(socket_desc);
  return 0;
}
