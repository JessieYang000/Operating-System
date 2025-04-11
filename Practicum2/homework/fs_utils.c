#include <string.h>
#include <sys/stat.h>
#include <stdio.h>

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
