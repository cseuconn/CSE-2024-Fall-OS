#include <iostream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <vector>

// Include your file system operations header
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/statvfs.h>
#include "file_operations.h"

// Function prototypes
void display_menu();
void handle_choice(int choice);

int main() {
    while (true) {
        display_menu();

        int choice;
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        if (choice == 0) {
            std::cout << "Exiting program. Goodbye!" << std::endl;
            break;
        }

        handle_choice(choice);
    }

    return 0;
}

void display_menu() {
    std::cout << "\nFile System Operations Menu:" << std::endl;
    std::cout << "1. Create a directory" << std::endl;
    std::cout << "2. Delete a directory" << std::endl;
    std::cout << "3. Create a file" << std::endl;
    std::cout << "4. Delete a file" << std::endl;
    std::cout << "5. Rename a file or directory" << std::endl;
    std::cout << "6. Move a file or directory" << std::endl;
    std::cout << "7. Copy a file" << std::endl;
    std::cout << "8. Change file permissions" << std::endl;
    std::cout << "9. Get file info" << std::endl;
    std::cout << "10. Read a file" << std::endl;
    std::cout << "11. Write to a file" << std::endl;
    std::cout << "12. List directory contents" << std::endl;
    std::cout << "0. Exit" << std::endl;
}

void handle_choice(int choice) {
    std::string path, new_name, content, destination;
    struct stat info;

    switch (choice) {
        case 1:
            std::cout << "Enter the directory name: ";
            std::cin >> path;
            create_directory(path.c_str());
            break;

        case 2:
            std::cout << "Enter the directory name to delete: ";
            std::cin >> path;
            delete_directory(path.c_str());
            break;

        case 3:
            std::cout << "Enter the file name: ";
            std::cin >> path;
            create_file(path.c_str());
            break;

        case 4:
            std::cout << "Enter the file name to delete: ";
            std::cin >> path;
            delete_file(path.c_str());
            break;

        case 5:
            std::cout << "Enter the old name: ";
            std::cin >> path;
            std::cout << "Enter the new name: ";
            std::cin >> new_name;
            rename_file_or_directory(path.c_str(), new_name.c_str());
            break;

        case 6:
            std::cout << "Enter the source path: ";
            std::cin >> path;
            std::cout << "Enter the destination path: ";
            std::cin >> destination;
            move_file_or_directory(path.c_str(), destination.c_str());
            break;

        case 7:
            std::cout << "Enter the source file path: ";
            std::cin >> path;
            std::cout << "Enter the destination file path: ";
            std::cin >> destination;
            copy_file(path.c_str(), destination.c_str());
            break;

        case 8:
            int permissions;
            std::cout << "Enter the file path: ";
            std::cin >> path;
            std::cout << "Enter the new permissions (octal, e.g., 0755): ";
            std::cin >> std::oct >> permissions;
            change_permissions(path.c_str(), permissions);
            break;

        case 9:
            std::cout << "Enter the file path: ";
            std::cin >> path;
            info = get_file_info(path.c_str());
            if (info.st_size != -1) {
                std::cout << "File info displayed above." << std::endl;
            }
            break;

        case 10:
            std::cout << "Enter the file path: ";
            std::cin >> path;
            std::cout << "File contents:\n" << read_file(path.c_str()) << std::endl;
            break;

        case 11:
            std::cout << "Enter the file path: ";
            std::cin >> path;
            std::cout << "Enter the content to write: ";
            std::cin.ignore();
            std::getline(std::cin, content);
            write_to_file(path.c_str(), content.c_str());
            break;

        case 12:
            std::cout << "Enter the directory path: ";
            std::cin >> path;
            std::cout << "Directory contents:\n" << list_directory_contents(path.c_str()) << std::endl;
            break;

        default:
            std::cout << "Invalid choice. Please try again." << std::endl;
            break;
    }
}
