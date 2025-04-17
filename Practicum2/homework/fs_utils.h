#ifndef FS_UTILS_H
#define FS_UTILS_H

int file_exists(const char *path);
void ensure_path_exists(const char *full_path);
int remove_directory(const char *path);
#endif
