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
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <sys/wait.h> // for waitpid
#include <fcntl.h>    // for file control (flock)
#include <sys/time.h> // for gettimeofday()

#define BUFFER_SIZE 8196
#define ROOT_DIR "./server_root" // Define server root for file writes

// Recursively remove a directory
int remove_directory(const char *path)
{
  DIR *d = opendir(path);
  int r = -1;

  if (d)
  {
    struct dirent *p;
    r = 0;
    while (!r && (p = readdir(d)))
    {
      // safety check in directory traversal
      if (!strcmp(p->d_name, ".") || !strcmp(p->d_name, ".."))
        continue;

      char buf[1024];
      snprintf(buf, sizeof(buf), "%s/%s", path, p->d_name);
      struct stat statbuf;

      // check if the file at buf exists and is accessible.
      if (!stat(buf, &statbuf))
      {
        if (S_ISDIR(statbuf.st_mode))
          r = remove_directory(buf); // Recursively remove a derectory
        else
          r = remove(buf); // Remove a file
      }
    }
    closedir(d);
  }

  if (!r) // Return true only when r == 0
    r = rmdir(path);
  return r;
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
  if (listen(socket_desc, 10) < 0) // up to 10 clients can be waiting to be accepted
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

    pid_t pid = fork(); // create a new process for this client
    if (pid < 0)
    {
      perror("fork failed");
      close(client_sock);
      continue;
    }
    else if (pid == 0) // child process
    {
      close(socket_desc); // child doesn't need the listening socket

      printf("Client connected at IP: %s and port: %i\n",
             inet_ntoa(client_addr.sin_addr),
             ntohs(client_addr.sin_port));

      // Receive client's message (command or plain text)
      ssize_t msg_len = recv(client_sock, client_message, sizeof(client_message), 0);
      if (msg_len < 0)
      {
        printf("Couldn't receive\n");
        close(client_sock);
        exit(1);
      }

      // Check if it's a WRITE command
      if (strncmp(client_message, "WRITE ", 6) == 0)
      {
        char *remote_path = client_message + 6;         // Skip "WRITE " prefix to extract the target remote file path
        remote_path[strcspn(remote_path, "\n")] = '\0'; // Remove newline if present

        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", ROOT_DIR, remote_path); // Prepend root directory

        ensure_path_exists(full_path);

        // Open the file at full_path for appending. If it doesn’t exist, create it with read/write permission for owner, and read-only for others.
        int fd = open(full_path, O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (fd < 0)
        {
          snprintf(server_message, sizeof(server_message), "FAILED: Could not open file for writing.\n");
          send(client_sock, server_message, strlen(server_message), 0);
          printf("Failed to open %s for writing.\n", full_path);
          close(client_sock);
          exit(1);
        }

        flock(fd, LOCK_EX); // lock the file for exclusive write

        FILE *fp = fdopen(fd, "ab"); // Append to avoid overwriting
        if (!fp)
        {
          snprintf(server_message, sizeof(server_message), "FAILED: Could not open file for writing.\n");
          send(client_sock, server_message, strlen(server_message), 0);
          printf("Failed to open %s for writing.\n", full_path);
          close(fd);
          close(client_sock);
          exit(1);
        }

        printf("Receiving file content for: %s\n", full_path);

        // Read file content from client after header
        char buffer[1024];
        ssize_t bytes_received;
        while ((bytes_received = recv(client_sock, buffer, sizeof(buffer), 0)) > 0)
        {
          fwrite(buffer, 1, bytes_received, fp);
          fflush(fp);        // Ensure data is flushed from stdio buffer
          fsync(fileno(fp)); // Ensure data is flushed to disk before releasing lock
          usleep(100);       // Simulate slow disk(0.1 ms delay per chunk)
        }

        fclose(fp);
        flock(fd, LOCK_UN); // unlock the file
        close(fd);
        snprintf(server_message, sizeof(server_message), "SUCCESS: File written to %s\n", full_path);
        printf("File saved successfully to %s\n", full_path);
      }
      // Handle GET command
      else if (strncmp(client_message, "GET ", 4) == 0)
      {
        char *remote_path = client_message + 4;         // Skip "GET " prefix
        remote_path[strcspn(remote_path, "\n")] = '\0'; // Remove newline if present

        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", ROOT_DIR, remote_path); // Prepend root directory

        int fd = open(full_path, O_RDONLY); // Use open() to enable flock
        if (fd < 0)
        {
          snprintf(server_message, sizeof(server_message), "FAILED: File not found: %s\n", full_path);
          send(client_sock, server_message, strlen(server_message), 0);
          printf("File not found: %s\n", full_path);
          close(client_sock);
          exit(1);
        }

        struct timeval t1, t2;
        gettimeofday(&t1, NULL);
        flock(fd, LOCK_SH); // Acquire shared lock for safe reading
        gettimeofday(&t2, NULL);
        printf("Time waiting for shared lock: %.3f seconds\n",
               (t2.tv_sec - t1.tv_sec) + (t2.tv_usec - t1.tv_usec) / 1000000.0);

        FILE *fp = fdopen(fd, "rb"); // Wrap fd into FILE* stream
        if (!fp)
        {
          snprintf(server_message, sizeof(server_message), "FAILED: File not found: %s\n", full_path);
          send(client_sock, server_message, strlen(server_message), 0);
          printf("File not found: %s\n", full_path);
          flock(fd, LOCK_UN); // Release lock on failure
          close(fd);
          close(client_sock);
          exit(1);
        }

        printf("Sending file: %s\n", full_path);
        char buffer[1024];
        size_t bytes_read;
        while ((bytes_read = fread(buffer, 1, sizeof(buffer), fp)) > 0)
        {
          send(client_sock, buffer, bytes_read, 0);
        }

        // This closes both FILE* and fd flock is released automatically when file descriptor is closed
        fclose(fp);
        printf("File sent successfully.\n");
      }
      // Handle RM command
      else if (strncmp(client_message, "RM ", 3) == 0)
      {
        char *remote_path = client_message + 3;
        remote_path[strcspn(remote_path, "\n")] = '\0';

        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", ROOT_DIR, remote_path); // Prepend root directory

        struct stat st;
        if (stat(full_path, &st) != 0) // check the existence of the path
        {
          snprintf(server_message, sizeof(server_message), "FAILED: Path not found: %s\n", full_path);
          send(client_sock, server_message, strlen(server_message), 0);
          printf("File or directory not found: %s\n", full_path);
          close(client_sock);
          exit(1);
        }

        int result = 0;
        if (S_ISDIR(st.st_mode))
        {
          // Deleting a directory: Use a lock file to protect directory deletion
          char lock_path[1024];
          snprintf(lock_path, sizeof(lock_path), "%s.lock", full_path);

          int lock_fd = open(lock_path, O_CREAT | O_RDWR, 0644);
          if (lock_fd < 0)
          {
            snprintf(server_message, sizeof(server_message), "FAILED: Unable to lock folder %s\n", full_path);
            send(client_sock, server_message, strlen(server_message), 0);
            printf("Failed to create lock for: %s\n", full_path);
            close(client_sock);
            exit(1);
          }

          flock(lock_fd, LOCK_EX); // Lock the folder before deletion
          result = remove_directory(full_path);
          flock(lock_fd, LOCK_UN); // Unlock
          close(lock_fd);
          remove(lock_path); // Cleanup lock file
        }
        else
        {
          // Deleting a file: Lock the file itself before deleting
          int fd = open(full_path, O_RDONLY);
          if (fd >= 0)
          {
            flock(fd, LOCK_EX);         // Lock the file
            result = remove(full_path); // delete file
            flock(fd, LOCK_UN);         // Unlock
            close(fd);
          }
          else
          {
            result = remove(full_path); // fallback: try to delete even file is failed to open
          }
        }

        if (result == 0)
        {
          snprintf(server_message, sizeof(server_message), "SUCCESS: Deleted %s\n", full_path);
          printf("Deleted: %s\n", full_path);
        }
        else
        {
          snprintf(server_message, sizeof(server_message), "FAILED: Unable to delete %s\n", full_path);
          printf("Failed to delete: %s\n", full_path);
        }

        send(client_sock, server_message, strlen(server_message), 0);
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
      exit(0);
    }
    else
    {
      // parent process. close client socket, reap zombies
      close(client_sock);
      waitpid(-1, NULL, WNOHANG); // non-blocking cleanup
    }
  }

  // Close the server socket
  close(socket_desc);
  return 0;
}
