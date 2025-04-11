// fs_utils.c
#include <string.h>
#include <sys/stat.h>
#include <stdio.h>

void ensure_path_exists(const char *full_path)
{
  char path[1024];
  strcpy(path, full_path);

  for (char *p = path + 1; *p; p++)
  {
    if (*p == '/')
    {
      *p = '\0';
      mkdir(path, 0755); // mkdir is safe to call even if path already exists
      *p = '/';
    }
  }
  mkdir(path, 0755); // Final mkdir call (handles last component)
}
