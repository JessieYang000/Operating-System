#include <string.h>
#include <sys/stat.h>
#include <stdio.h>
#include <dirent.h>     // for DIR, opendir, readdir
#include <unistd.h>     // for rmdir(), remove()

// Create all parent directories for a given file path
void ensure_path_exists(const char *full_path)
{
  char path[1024];
  strncpy(path, full_path, sizeof(path));
  path[sizeof(path) - 1] = '\0'; // Ensure null-terminated

  // Find the last slash to isolate directory path
  char *last_slash = strrchr(path, '/');
  if (!last_slash)
    return; // No slashes → nothing to create

  *last_slash = '\0'; // Strip the file name to leave just the directory path

  // Skip if result is empty (e.g., just a filename in current dir)
  if (strlen(path) == 0)
    return;

  // Create each level of directory
  for (char *p = path + 1; *p; p++)
  {
    if (*p == '/')
    {
      *p = '\0';
      mkdir(path, 0755); // Create partial path
      *p = '/';
    }
  }

  // Create final directory
  mkdir(path, 0755);
}

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
    r = rmdir(path);// rmdir() is used to delete the empty directory
  return r;
}

// Validate the local file path before sending the file to the server
int file_exists(const char *path)
{
  struct stat st;
  return stat(path, &st) == 0 && S_ISREG(st.st_mode); // Check existence of the file and whether it's a regular file
}