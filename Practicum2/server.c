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
#include <stdlib.h>    // for exit()
#include <sys/types.h> // for pid_t
#include <sys/stat.h>  // for mkdir
#include <libgen.h>    // for dirname()
#include <errno.h>

#define CHUNK_SIZE 1024        // define fixed chunk size
#define ROOT_DIR "server_root" // all files stored under this dir

void ensure_dir(const char *path)
{
  char path_copy[1024];
  strcpy(path_copy, path);
  char *dir = dirname(path_copy);

  char full_path[1024];
  snprintf(full_path, sizeof(full_path), "%s/%s", ROOT_DIR, dir);
  mkdir(full_path, 0777); // only creates one level
}

int main(void)
{
  int socket_desc, client_sock;
  socklen_t client_size;
  struct sockaddr_in server_addr, client_addr;
  char buffer[CHUNK_SIZE]; // smaller buffer for chunks

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
  if (listen(socket_desc, 5) < 0)
  { // Allow up to 5 pending clients
    printf("Error while listening\n");
    close(socket_desc);
    return -1;
  }
  printf("\nListening for incoming connections.....\n");

  // loop for accepting multiple clients
  while (1)
  {
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

    // Fork a process to handle this client
    pid_t pid = fork();
    if (pid == 0)
    {
      // CHILD process
      close(socket_desc); // Child doesn't need the listener

      // receive command
      memset(buffer, '\0', CHUNK_SIZE);
      if (recv(client_sock, buffer, CHUNK_SIZE, 0) <= 0)
      {
        printf("Failed to receive command.\n");
        close(client_sock);
        exit(1);
      }

      char *cmd = strtok(buffer, " ");
      char *remote_path = strtok(NULL, " ");

      if (cmd == NULL || remote_path == NULL || strcmp(cmd, "WRITE") != 0)
      {
        printf("Invalid command.\n");
        close(client_sock);
        exit(1);
      }

      // prepare full path
      char full_path[1024];
      snprintf(full_path, sizeof(full_path), "%s/%s", ROOT_DIR, remote_path); // full path under server_root
      ensure_di(remote_path); // create folder if needed

      FILE *fp = fopen(full_path, "wb");
      if (!fp)
      {
        perror("File open failed");
        close(client_sock);
        exit(1);
      }

      //receive and write chunks
      while (1)
      {
        memset(buffer, '\0', CHUNK_SIZE);
        int bytes = recv(client_sock, buffer, CHUNK_SIZE, 0);
        if (bytes <= 0)
        {
          printf("Client disconnected.\n");
          break;
        }
        if (strncmp(buffer, "EOF", 3) == 0)
        {
          printf("File written to: %s\n", full_path);
          break;
        }
        fwrite(buffer, 1, bytes, fp);
      }

      fclose(fp);
      close(client_sock);
      exit(0); // Exit child process
    }
    else if (pid > 0)
    {
      // PARENT process
      close(client_sock); // Parent doesn't handle this client
    }
    else
    {
      perror("fork failed");
    }
  }

  // Closing the socket
  close(socket_desc);
  return 0;
}
