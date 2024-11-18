#ifndef FILE_OPERATIONS_H
#define FILE_OPERATIONS_H

#include <string>

int create_directory(const char *name);
int delete_directory(const char *name);
int create_file(const char *name);
int write_to_file(const char *name, const char *content);
std::string  read_file(const char *name);
int change_directory(const char *path);
std::string list_directory_contents(const char *path);
int delete_file(const char *name);


// Extended functionalities
int rename_file_or_directory(const char *old_name, const char *new_name);
int move_file_or_directory(const char *source, const char *destination);
int copy_file(const char *source, const char *destination);
bool exists(const char *path);
struct stat get_file_info(const char *path);
int change_permissions(const char *path, mode_t mode);
int create_symlink(const char *target, const char *linkpath);
struct statvfs get_disk_usage(const char *path);
void check_file_or_directory(const char *path);
std::string search_file_in_directory(const char *dir_path, const char *file_name);
std::string create_temp_file();

#endif // FILE_OPERATIONS_H
