#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define MAX_PATH 1024

// Function to list contents of the current directory
void list_directory(const char *path) {
    DIR *dir = opendir(path);
    if (!dir) {
        perror("Failed to open directory");
        return;
    }

    printf("\nContents of directory: %s\n", path);
    printf("=====================================\n");

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        struct stat file_stat;
        char full_path[MAX_PATH];
        snprintf(full_path, MAX_PATH, "%s/%s", path, entry->d_name);

        if (stat(full_path, &file_stat) == 0) {
            printf("%-20s %10ld bytes %s\n", entry->d_name, file_stat.st_size,
                   S_ISDIR(file_stat.st_mode) ? "[DIR]" : "[FILE]");
        } else {
            perror("Failed to get file stats");
        }
    }

    closedir(dir);
}

// Function to create a directory
void create_directory(const char *path) {
    if (mkdir(path, 0777) == 0) {
        printf("Directory '%s' created successfully.\n", path);
    } else {
        perror("Failed to create directory");
    }
}

// Function to remove a directory
void remove_directory(const char *path) {
    if (rmdir(path) == 0) {
        printf("Directory '%s' removed successfully.\n", path);
    } else {
        perror("Failed to remove directory");
    }
}

// Function to remove a file
void remove_file(const char *path) {
    if (remove(path) == 0) {
        printf("File '%s' removed successfully.\n", path);
    } else {
        perror("Failed to remove file");
    }
}

int main() {
    char current_path[MAX_PATH];
    getcwd(current_path, MAX_PATH);

    char command[50], argument[MAX_PATH];

    printf("Welcome to the CLI File Explorer!\n");
    printf("Commands:\n");
    printf("  ls                - List directory contents\n");
    printf("  mkdir <name>      - Create a new directory\n");
    printf("  rmdir <name>      - Remove a directory\n");
    printf("  rmfile <name>     - Remove a file\n");
    printf("  cd <path>         - Change directory\n");
    printf("  ..                - Go up one level\n");
    printf("  pwd               - Print working directory\n");
    printf("  q                 - Quit the program\n\n");

    while (1) {
        printf("%s> ", current_path);
        scanf("%s", command);

        if (strcmp(command, "ls") == 0) {
            list_directory(current_path);
        } else if (strcmp(command, "mkdir") == 0) {
            scanf("%s", argument);
            char new_dir[MAX_PATH];
            snprintf(new_dir, MAX_PATH, "%s/%s", current_path, argument);
            create_directory(new_dir);
        } else if (strcmp(command, "rmdir") == 0) {
            scanf("%s", argument);
            char target_dir[MAX_PATH];
            snprintf(target_dir, MAX_PATH, "%s/%s", current_path, argument);
            remove_directory(target_dir);
        } else if (strcmp(command, "rmfile") == 0) {
            scanf("%s", argument);
            char target_file[MAX_PATH];
            snprintf(target_file, MAX_PATH, "%s/%s", current_path, argument);
            remove_file(target_file);
        } else if (strcmp(command, "cd") == 0) {
            scanf("%s", argument);
            if (chdir(argument) == 0) {
                getcwd(current_path, MAX_PATH);
            } else {
                perror("Failed to change directory");
            }
        } else if (strcmp(command, "..") == 0) {
            if (chdir("..") == 0) {
                getcwd(current_path, MAX_PATH);
            } else {
                perror("Failed to change directory");
            }
        } else if (strcmp(command, "pwd") == 0) {
            printf("Current directory: %s\n", current_path);
        } else if (strcmp(command, "q") == 0) {
            printf("Exiting the program.\n");
            break;
        } else {
            printf("Unknown command: %s\n", command);
        }
    }

    return 0;
}
