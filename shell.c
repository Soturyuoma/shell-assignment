#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <windows.h>
#include <sys/stat.h>
#include "shell.h"

/**
 * Displays a welcome message with ASCII art.
 */
void Wellcome() {
    printf("aSPY//YASa\n");
    printf("apyyyyCY//////////YCa |\n");
    printf("sY//////YSpcs scpCY//Pp | Welcome to myShell\n");
    printf("ayp ayyyyyyySCP//Pp syY//C | Version 2.4.3\n");
    printf("AYAsAYYYYYYYY///Ps cY//S |\n");
    printf("pCCCCY//p cSSps y//Y | https://github.com/Soturyuoma/\n");
    printf("SPPPP///a pP///AC//Y |\n");
    printf("A//A cyP////C | Have fun!\n");
    printf("p///Ac sC///a |\n");
    printf("P////YCpc A//A | Craft packets like it is your last\n");
    printf("scccccp///pSP///p p//Y | day on earth.\n");
    printf("sY/////////y caa S//P | -- Lao-Tze\n");
    printf("cayCyayP//Ya pY/Ya |\n");
    printf("sY/PsY////YCc aC//Yp\n");
    printf("sc sccaCY//PCypaapyCP//YSs\n");
    printf("spCPY//////YPSps\n");
    printf("ccaacs\n");
}

/**
 * Retrieves and displays the current location (username, hostname, and current directory).
 */
void getLocation() {
    char hostname[1024];
    char username[1024];
    DWORD hostnameSize = sizeof(hostname);
    DWORD usernameSize = sizeof(username);

    if (!GetComputerNameA(hostname, &hostnameSize)) {
        fprintf(stderr, "Failed to retrieve hostname.\n");
        return;
    }

    if (!GetUserNameA(username, &usernameSize)) {
        fprintf(stderr, "Failed to retrieve username.\n");
        return;
    }

    char currentDir[1024];
    if (!GetCurrentDirectoryA(sizeof(currentDir), currentDir)) {
        fprintf(stderr, "Failed to retrieve current directory.\n");
        return;
    }

    printf("\033[1m\033[32m%s@%s\033[0m:\033[1m\033[34m%s\033[0m$ ", username, hostname, currentDir);
}

/**
 * Moves a file from the source path to the destination path.
 *
 * @param args Array of command-line arguments.
 *             args[1] should be the source file path.
 *             args[2] should be the destination file path.
 */

void move(char** args) {
    if (args[1] == NULL || args[2] == NULL) {
        fprintf(stderr, "move: missing file operand\n");
        return;
    }

    char* source = extract_quoted_path(args, 1);
    char* dest = extract_quoted_path(args, 2);

    if (source == NULL || dest == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        return;
    }

    printf("Source path: %s\n", source);

    // Check if the source file exists
    if (access(source, F_OK) != 0) {
        fprintf(stderr, "Source file does not exist: %s\n", source);
        free(source);
        free(dest);
        return;
    }

    // Check if the destination directory exists
    char* dest_dir = malloc(strlen(dest) + 1);
    if (dest_dir == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        free(source);
        free(dest);
        return;
    }
    strcpy(dest_dir, dest);

    char* last_slash = strrchr(dest_dir, '\\');
    if (last_slash != NULL) {
        *last_slash = '\0';
    }
    if (access(dest_dir, F_OK) != 0) {
        fprintf(stderr, "Destination directory does not exist: %s\n", dest_dir);
        free(dest_dir);
        free(source);
        free(dest);
        return;
    }
    free(dest_dir);

    // Check if a file with the same name already exists in the destination directory
    struct stat st;
    if (stat(dest, &st) == 0) {
        // File with the same name exists
        printf("File already exists in the destination directory: %s\n", dest);
        // Here, you can prompt the user to choose an action (e.g., overwrite, rename, cancel)
        // For simplicity, let's assume we always overwrite the existing file

        // Remove the existing file
        if (remove(dest) != 0) {
            perror("move");
            free(source);
            free(dest);
            return;
        }
    }

    // Move the file to the destination directory
    if (rename(source, dest) != 0) {
        perror("move");
    } else {
        printf("File moved successfully.\n");
    }

    free(source);
    free(dest);
}

/**
 * Appends the specified string to the end of a file.
 *
 * @param args Array of command-line arguments.
 *             args[1] should be the string to append.
 *             args[3] should be the file path.
 */
void echoppend(char** args) {
    if (args[1] == NULL || args[2] == NULL) {
        fprintf(stderr, "echoppend: missing operand\n");
        return;
    }

    // Extract the file path from args[2]
    char* path = extract_quoted_path(args, 2);
    if (path == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        return;
    }

    FILE* file = fopen(path, "a");
    if (file == NULL) {
        perror("echoppend");
        free(path);
        return;
    }

    // Write the string to the file
    fprintf(file, "%s\n", args[1]);
    fclose(file);

    free(path);
}

/**
 * Writes the specified string to a file, overwriting its contents.
 *
 * @param args Array of command-line arguments.
 *             args[1] should be the string to write.
 *             args[3] should be the file path.
 */
void echorite(char** args) {
    if (args[1] == NULL || args[2] == NULL) {
        fprintf(stderr, "echorite: missing operand\n");
        return;
    }

    char* path = extract_path(args[2]);
    if (path == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        return;
    }

    FILE* file = fopen(path, "w");
    if (file == NULL) {
        perror("echorite");
        free(path);
        return;
    }

    fprintf(file, "%s\n", args[1]);
    fclose(file);

    free(path);
}

/**
 * Reads the contents of a file and displays them on the console.
 *
 * @param args Array of command-line arguments.
 *             args[1] should be the file path.
 */
void readFile(char** args) {
    if (args[1] == NULL) {
        fprintf(stderr, "readFile: missing file operand\n");
        return;
    }

    char* path = extract_path(args[1]);
    if (path == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        return;
    }

    FILE* file = fopen(path, "r");
    if (file == NULL) {
        perror("readFile");
        free(path);
        return;
    }

    char buffer[4096];
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        printf("%s", buffer);
    }

    fclose(file);

    free(path);
}

/**
 * Counts the number of lines or words in a file.
 *
 * @param args Array of command-line arguments.
 *             args[1] should be the option ("-l" for lines, "-w" for words).
 *             args[2] should be the file path.
 */
void wordCount(char** args) {
    if (args[1] == NULL || args[2] == NULL) {
        fprintf(stderr, "wordCount: missing operand\n");
        return;
    }

    char* option = args[1];
    char* path = extract_path(args[2]);
    if (path == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        return;
    }

    FILE* file = fopen(path, "r");
    if (file == NULL) {
        perror("wordCount");
        free(path);
        return;
    }

    int count = 0;
    if (strcmp(option, "-l") == 0) {
        char buffer[4096];
        while (fgets(buffer, sizeof(buffer), file) != NULL) {
            count++;
        }
        printf("Number of lines: %d\n", count);
    } else if (strcmp(option, "-w") == 0) {
        char c;
        int in_word = 0;
        while ((c = fgetc(file)) != EOF) {
            if (isspace(c)) {
                in_word = 0;
            } else if (!in_word) {
                in_word = 1;
                count++;
            }
        }
        printf("Number of words: %d\n", count);
    } else {
        fprintf(stderr, "Invalid option: %s\n", option);
    }

    fclose(file);

    free(path);
}

/**
 * Splits a string into an array of arguments.
 *
 * @param str The string to split.
 * @return An array of arguments, or NULL if memory allocation fails.
 */
char** splitArgument(char* str) {
    int capacity = 10;
    char** args = malloc(capacity * sizeof(char*));
    if (args == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        return NULL;
    }

    int argc = 0;
    char* token = strtok(str, " ");
    while (token != NULL) {
        args[argc++] = token;
        if (argc == capacity) {
            capacity *= 2;
            char** new_args = realloc(args, capacity * sizeof(char*));
            if (new_args == NULL) {
                fprintf(stderr, "Memory allocation failed.\n");
                free(args);
                return NULL;
            }
            args = new_args;
        }
        token = strtok(NULL, " ");
    }

    // Null-terminate the array of arguments
    args[argc] = NULL;
    return args;
}

/**
 * Trims leading and trailing whitespace from a string.
 *
 * @param str The string to trim.
 * @return The trimmed string.
 */
char* strtrim(char* str) {
    char* end;

    // Trim leading whitespace
    while (*str == ' ') {
        str++;
    }

    // Trim trailing whitespace
    end = str + strlen(str) - 1;
    while (end > str && *end == ' ') {
        end--;
    }

    // Write new null terminator
    *(end + 1) = '\0';

    return str;
}

/**
 * Logs out of the shell.
 *
 * @param str The logout command string.
 */
void logout(char*str) {
    char* trimmed = strtrim(str);
    if (strcmp(trimmed, "exit") == 0) {
        printf("Logging out...\n");
        exit(EXIT_SUCCESS);
    }
}

/**
 * Joins an array of strings into a single string using a delimiter.
 *
 * @param strings The array of strings to join.
 * @param count The number of strings in the array.
 * @param delimiter The delimiter to use between the strings.
 * @return The joined string, or NULL if memory allocation fails.
 */
char* join_strings(char** strings, int count, const char* delimiter) {
    int total_length = 0;
    for (int i = 0; i < count; i++) {
        total_length += strlen(strings[i]);
    }
    total_length += (count - 1) * strlen(delimiter);

    char* result = malloc((total_length + 1) * sizeof(char));
    if (result == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        return NULL;
    }

    strcpy(result, strings[0]);

    for (int i = 1; i < count; i++) {
        strcat(result, delimiter);
        strcat(result, strings[i]);
    }

    return result;
}

/**
 * Changes the current directory.
 *
 * @param args Array of command-line arguments.
 *             args[1] should be the directory path.
 */
void cd(char** args) {
    if (args[1] == NULL) {
        fprintf(stderr, "cd: missing operand\n");
        return;
    }

    char* path = args[1];
    if (path[0] == '"') {
        int i = 2;
        while (args[i] != NULL && strchr(args[i], '"') == NULL) {
            i++;
        }
        if (args[i] != NULL) {
            args[i][strcspn(args[i], "\"")] = '\0';
            path = join_strings(args + 1, i - 1, " ");
            if (path == NULL) {
                return;
            }
        }
    }

    if (chdir(path) != 0) {
        perror("cd");
    }

    if (path != args[1]) {
        free(path);
    }
}

/**
 * Extracts a quoted path from an array of arguments.
 *
 * @param args Array of command-line arguments.
 * @param index The index of the argument containing the quoted path.
 * @return The extracted path, or NULL if memory allocation fails.
 */

char* extract_quoted_path(char** args, int index) {
    char* path = args[index];
    if (path[0] == '"') {
        path++;
        char* end = strchr(path, '"');
        if (end != NULL) {
            *end = '\0';
        }
    }
    char* extracted_path = (char*) malloc(strlen(path) + 1);
    if (extracted_path != NULL) {
        strcpy(extracted_path, path);
    }
    return extracted_path;
}

/**
 * Copies a file from the source path to the destination path.
 *
 * @param args Array of command-line arguments.
 *             args[1] should be the source file path.
 *             args[2] should be the destination file path.
 */
void cp(char** args) {
    if (args[1] == NULL || args[2] == NULL) {
        fprintf(stderr, "cp: missing file operand\n");
        return;
    }

    char* source = args[1];
    char* dest = args[2];

    if (source[0] == '"') {
        source = extract_quoted_path(args, 1);
        if (source == NULL) {
            return;
        }
    }

    if (dest[0] == '"') {
        dest = extract_quoted_path(args, 2);
        if (dest == NULL) {
            if (source != args[1]) {
                free(source);
            }
            return;
        }
    }

    FILE* src_file = fopen(source, "rb");
    if (src_file == NULL) {
        perror("cp");
        if (source != args[1]) {
            free(source);
        }
        if (dest != args[2]) {
            free(dest);
        }
        return;
    }

    FILE* dest_file = fopen(dest, "wb");
    if (dest_file == NULL) {
        perror("cp");
        fclose(src_file);
        if (source != args[1]) {
            free(source);
        }
        if (dest != args[2]) {
            free(dest);
        }
        return;
    }

    char buffer[4096];
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), src_file)) > 0) {
        fwrite(buffer, 1, bytes_read, dest_file);
    }

    fclose(src_file);
    fclose(dest_file);

    if (source != args[1]) {
        free(source);
    }
    if (dest != args[2]) {
        free(dest);
    }
}

/**
 * Extracts a path from a string, handling quotes if present.
 *
 * @param str The string containing the path.
 * @return The extracted path, or NULL if memory allocation fails.
 */
char* extract_path(char* str) {
    char* path = str;
    if (str[0] == '"') {
        path = strtok(str, "\"");
        if (path != NULL) {
            char* new_path = malloc(strlen(path) + 1);
            if (new_path == NULL) {
                fprintf(stderr, "Memory allocation failed.\n");
                return NULL;
            }
            strcpy(new_path, path);
            path = new_path;
        }
    }
    return path;
}

/**
 * Deletes a file.
 *
 * @param str The file path to delete.
 */
void delete(char* str) {
    char* path = extract_path(str);
    if (path == NULL) {
        return;
    }

    if (remove(path) != 0) {
        perror("delete");
    }

    if (path != str) {
        free(path);
    }
}

/**
 * Executes a pipeline of two commands.
 *
 * @param argv1 Array of command-line arguments for the first command.
 * @param argv2 Array of command-line arguments for the second command.
 */
void mypipe(char** argv1, char** argv2) {
    HANDLE hPipeRead, hPipeWrite;
    SECURITY_ATTRIBUTES saAttr;

    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = TRUE;
    saAttr.lpSecurityDescriptor = NULL;

    if (!CreatePipe(&hPipeRead, &hPipeWrite, &saAttr, 0)) {
        perror("CreatePipe");
        return;
    }

    STARTUPINFO siStartInfo1, siStartInfo2;
    PROCESS_INFORMATION piProcInfo1, piProcInfo2;

    ZeroMemory(&siStartInfo1, sizeof(STARTUPINFO));
    siStartInfo1.cb = sizeof(STARTUPINFO);
    siStartInfo1.hStdOutput = hPipeWrite;
    siStartInfo1.dwFlags |= STARTF_USESTDHANDLES;

    ZeroMemory(&piProcInfo1, sizeof(PROCESS_INFORMATION));

    if (!CreateProcess(NULL, argv1[0], NULL, NULL, TRUE, 0, NULL, NULL, &siStartInfo1, &piProcInfo1)) {
        DWORD exitCode;
        GetExitCodeProcess(piProcInfo1.hProcess, &exitCode);
        if (exitCode == 0) {
            CloseHandle(hPipeWrite);
            CloseHandle(piProcInfo1.hProcess);
            CloseHandle(piProcInfo1.hThread);
        } else {
            perror("CreateProcess 1");
            CloseHandle(hPipeRead);
            CloseHandle(hPipeWrite);
            return;
        }
    }

    CloseHandle(hPipeWrite);

    ZeroMemory(&siStartInfo2, sizeof(STARTUPINFO));
    siStartInfo2.cb = sizeof(STARTUPINFO);
    siStartInfo2.hStdInput = hPipeRead;
    siStartInfo2.dwFlags |= STARTF_USESTDHANDLES;

    ZeroMemory(&piProcInfo2, sizeof(PROCESS_INFORMATION));

    if (!CreateProcess(NULL, argv2[0], NULL, NULL, TRUE, 0, NULL, NULL, &siStartInfo2, &piProcInfo2)) {
        DWORD exitCode;
        GetExitCodeProcess(piProcInfo2.hProcess, &exitCode);
        if (exitCode == 0) {
            CloseHandle(hPipeRead);
            CloseHandle(piProcInfo2.hProcess);
            CloseHandle(piProcInfo2.hThread);
        } else {
            perror("CreateProcess 2");
            CloseHandle(hPipeRead);
            CloseHandle(piProcInfo1.hProcess);
            CloseHandle(piProcInfo1.hThread);
            return;
        }
    }

    CloseHandle(hPipeRead);

    WaitForSingleObject(piProcInfo1.hProcess, INFINITE);
    WaitForSingleObject(piProcInfo2.hProcess, INFINITE);

    CloseHandle(piProcInfo1.hProcess);
    CloseHandle(piProcInfo1.hThread);
    CloseHandle(piProcInfo2.hProcess);
    CloseHandle(piProcInfo2.hThread);
}