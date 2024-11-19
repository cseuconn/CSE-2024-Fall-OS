#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <dirent.h>
#include <iostream>
#include <cerrno>
#include <sys/statvfs.h>

struct stat get_file_info(const char *path);

int create_directory(const char *name) {
    if (mkdir(name, 0777) == -1) {
        perror("mkdir failed");
        return errno;
    } else {
        std::cout << "Directory created: " << name << std::endl;
    }
    return 0;
}

int delete_directory(const char *name) {
    DIR *dir = opendir(name);
    if (dir == NULL) {
        perror("opendir failed");
        return errno;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        std::string path = std::string(name) + "/" + entry->d_name;
        struct stat statbuf;
        if (stat(path.c_str(), &statbuf) == -1) {
            perror("stat failed");
            closedir(dir);
            return errno;
        }

        if (S_ISDIR(statbuf.st_mode)) {
            if (delete_directory(path.c_str()) != 0) {
                closedir(dir);
                return errno;
            }
        } else {
            if (unlink(path.c_str()) == -1) {
                perror("unlink failed");
                closedir(dir);
                return errno;
            }
        }
    }

    closedir(dir);

    if (rmdir(name) == -1) {
        perror("rmdir failed");
        return errno;
    } else {
        std::cout << "Directory deleted: " << name << std::endl;
    }
    return 0;
}

int rename_file_or_directory(const char *old_name, const char *new_name) {
    if (rename(old_name, new_name) == -1) {
        perror("rename failed");
        return errno;
    } else {
        std::cout << "Renamed: " << old_name << " to " << new_name << std::endl;
    }
    return 0;
}

int move_file_or_directory(const char *source, const char *destination) {
    if (rename(source, destination) == 0) {
        std::cout << "Moved: " << source << " to " << destination << std::endl;
        return 0;
    } else {
        perror("rename failed");
        // Attempt copy and delete if rename fails (for cross-filesystem moves)
        // if (S_ISDIR(stat(destination, &statbuf)) && copy_directory(source, destination) != 0){
        //     return -1;
        // }  
    }
}


int copy_file(const char *source, const char *destination) {
    int source_fd = open(source, O_RDONLY);
    if (source_fd == -1) {
        perror("open source file failed");
        return errno;
    }

    int dest_fd = open(destination, O_CREAT | O_WRONLY, 0666);
    if (dest_fd == -1) {
        perror("open destination file failed");
        close(source_fd);
        return errno;
    }

    char buffer[1024];
    ssize_t bytes_read;
    while ((bytes_read = read(source_fd, buffer, sizeof(buffer))) > 0) {
        if (write(dest_fd, buffer, bytes_read) == -1) {
            perror("write to destination file failed");
            close(source_fd);
            close(dest_fd);
            return errno;
        }
    }

    if (bytes_read == -1) {
        perror("read from source file failed");
    } else {
        std::cout << "File copied from " << source << " to " << destination << std::endl;
    }

    close(source_fd);
    close(dest_fd);
    return 0;
}


bool exists(const char *path) {
    struct stat statbuf;
    return (stat(path, &statbuf) == 0);
}

struct stat get_file_info(const char *path){
    struct stat statbuf;

    if (stat(path, &statbuf) == -1) {
        perror("stat failed");
        return statbuf;
    }

    // std::cout << "File: " << path << std::endl;
    // std::cout << "Size: " << statbuf.st_size << " bytes" << std::endl;
    // std::cout << "Permissions: " << (statbuf.st_mode & 0777) << std::endl;
    // std::cout << "Last modified: " << ctime(&statbuf.st_mtime);
    // std::cout << "Last accessed: " << ctime(&statbuf.st_atime);
    // std::cout << "Creation time: " << ctime(&statbuf.st_ctime);

    return statbuf;

}


int change_permissions(const char *path, mode_t mode) {
    if (chmod(path, mode) == -1) {
        perror("chmod failed");
        return errno;
    } else {
        std::cout << "Permissions changed for: " << path << std::endl;
    }
    return 0;
}


int create_symlink(const char *target, const char *linkpath) {
    if (symlink(target, linkpath) == -1) {
        perror("symlink creation failed");
        return errno;
    } else {
        std::cout << "Symbolic link created: " << linkpath << " -> " << target << std::endl;
    }
    return 0;
}


struct statvfs  get_disk_usage(const char* path) {
    struct statvfs stat;
    
    // Get filesystem stats
    if (statvfs(path, &stat) != 0) {
        perror("statvfs failed");
        return stat;
    }

    // You can now use the stat structure to get disk usage info
    unsigned long free_space = stat.f_bfree * stat.f_frsize;
    unsigned long total_space = stat.f_blocks * stat.f_frsize;
    unsigned long used_space = total_space - free_space;

    // std::cout << "Free space: " << free_space << " bytes\n";
    // std::cout << "Used space: " << used_space << " bytes\n";
    // std::cout << "Total space: " << total_space << " bytes\n";

    return stat;

}


void check_file_or_directory(const char *path) {
    struct stat statbuf;
    if (stat(path, &statbuf) == -1) {
        perror("stat failed");
        return;
    }

    if (S_ISDIR(statbuf.st_mode)) {
        std::cout << path << " is a directory.\n";
    } else if (S_ISREG(statbuf.st_mode)) {
        std::cout << path << " is a regular file.\n";
    } else {
        std::cout << path << " is some other type of file.\n";
    }
}


std::string search_file_in_directory(const char *dir_path, const char *file_name) {
    DIR *dir = opendir(dir_path);
    if (!dir) {
        perror("opendir failed");
        return "Error: " + std::string(strerror(errno));
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, file_name) == 0) {
            std::string full_path = std::string(dir_path) + "/" + entry->d_name;
            closedir(dir);
            return full_path;
        }
    }

    closedir(dir);
    return "File not found";
}





int create_file(const char *name) {
    int fd = open(name, O_CREAT | O_WRONLY, 0666);
    if (fd == -1) {
        perror("File creation failed");
        return errno;
    } else {
        std::cout << "File created: " << name << std::endl;
        close(fd);
    }
    return 0;
}

int write_to_file(const char *name, const char *content) {
    int fd = open(name, O_WRONLY | O_APPEND);
    if (fd == -1) {
        perror("File opening for writing failed");
        return errno;
    }
    if (write(fd, content, strlen(content)) == -1) {
        perror("Write failed");
        return errno;
    } else {
        std::cout << "Data written to file: " << name << std::endl;
    }
    close(fd);
    return 0;
}

std::string read_file(const char *name) {
    int fd = open(name, O_RDONLY);
    if (fd == -1) {
        perror("File opening for reading failed");
        return "Error: " + std::string(strerror(errno));
    }

    char buffer[1024];
    ssize_t bytesRead;
    std::string content;
    while ((bytesRead = read(fd, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[bytesRead] = '\0';
        std::cout << buffer;
        content += buffer;
    }

    if (bytesRead == -1) {
        perror("Read failed");
        content = "Error: " + std::string(strerror(errno));
    }
    close(fd);
    return content;
}

int change_directory(const char *path) {
    if (chdir(path) == -1) {
        perror("chdir failed");
        return errno;
    } else {
        std::cout << "Changed directory to: " << path << std::endl;
    }

    return 0;
}

int delete_file(const char *name) {
    if (unlink(name) == -1) {
        perror("unlink failed");
        return errno;
    } else {
        std::cout << "File deleted: " << name << std::endl;
    }
    return 0;
}

std::string list_directory_contents(const char *path) {
    DIR *dir = opendir(path);
    if (dir == NULL) {
        return "Error: " + std::string(strerror(errno));
    }

    struct dirent *entry;
    std::string contents;
    while ((entry = readdir(dir)) != NULL) {
        contents += entry->d_name;
        contents += "\n";
    }

    closedir(dir);
    return contents;
}
