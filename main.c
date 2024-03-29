#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <windows.h>
#include "shell.h"

int main() {
    printf("Welcome to my shell assignment!\n");
    Wellcome();
    getLocation();
    return 0;
}

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

void getLocation() {
    char hostname[1024];
    char username[1024];
    DWORD hostnameSize = sizeof(hostname);
    DWORD usernameSize = sizeof(username);

    GetComputerNameA(hostname, &hostnameSize);
    GetUserNameA(username, &usernameSize);

    char currentDir[1024];
    GetCurrentDirectoryA(sizeof(currentDir), currentDir);

    printf("\033[1m\033[32m%s@%s\033[0m:\033[1m\033[34m%s\033[0m$ ", username, hostname, currentDir);
}

void move(char** args) {
    char* source = extract_path(args[1]);
    char* dest = extract_path(args[2]);

    if (rename(source, dest) != 0) {
        perror("move");
    }

    if (source != args[1]) {
        free(source);
    }
    if (dest != args[2]) {
        free(dest);
    }
}

void echoppend(char** args) {
    char* path = extract_path(args[3]);

    FILE* file = fopen(path, "a");
    if (file == NULL) {
        perror("echoppend");
        return;
    }

    fprintf(file, "%s\n", args[1]);
    fclose(file);

    if (path != args[3]) {
        free(path);
    }
}

void echorite(char** args) {
    char* path = extract_path(args[3]);

    FILE* file = fopen(path, "w");
    if (file == NULL) {
        perror("echorite");
        return;
    }

    fprintf(file, "%s\n", args[1]);
    fclose(file);

    if (path != args[3]) {
        free(path);
    }
}

void readFile(char** args) {
    char* path = extract_path(args[1]);

    FILE* file = fopen(path, "r");
    if (file == NULL) {
        perror("read");
        return;
    }

    char buffer[4096];
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        printf("%s", buffer);
    }

    fclose(file);

    if (path != args[1]) {
        free(path);
    }
}

void wordCount(char** args) {
    char* option = args[1];
    char* path = extract_path(args[2]);

    FILE* file = fopen(path, "r");
    if (file == NULL) {
        perror("wordCount");
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
    }

    fclose(file);

    if (path != args[2]) {
        free(path);
    }
}

char** splitArgument(char* str) {
    int capacity = 10;
    char** args = malloc(capacity * sizeof(char*));
    int argc = 0;
    char* token = str;
    while (*token != '\0') {
        while (*token == ' ') {
            *token = '\0';
            token++;
        }
        if (*token != '\0') {
            args[argc++] = token;
            if (argc == capacity) {
                capacity *= 2;
                args = realloc(args, capacity * sizeof(char*));
            }
        }
        while (*token != ' ' && *token != '\0') {
            token++;
        }
    }
    args[argc] = NULL;
    return args;
}

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

void logout(char* str) {
    char* trimmed = strtrim(str);
    if (strcmp(trimmed, "exit") == 0) {
        printf("Logging out...\n");
        free(trimmed);
        exit(EXIT_SUCCESS);
    }
    free(trimmed);
}

char* join_strings(char** strings, int count, const char* delimiter) {
    int total_length = 0;
    for (int i = 0; i < count; i++) {
        total_length += strlen(strings[i]);
    }
    total_length += (count - 1) * strlen(delimiter);

    char* result = malloc((total_length + 1) * sizeof(char));
    strcpy(result, strings[0]);

    for (int i = 1; i < count; i++) {
        strcat(result, delimiter);
        strcat(result, strings[i]);
    }

    return result;
}

void cd(char** args) {
    char* path = args[1];
    if (path[0] == '"') {
        int i = 2;
        while (args[i] != NULL && strchr(args[i], '"') == NULL) {
            i++;
        }
        if (args[i] != NULL) {
            args[i][strcspn(args[i], "\"")] = '\0';
            path = join_strings(args + 1, i - 1, " ");
        }
    }

    if (chdir(path) != 0) {
        perror("cd");
    }

    if (path != args[1]) {
        free(path);
    }
}

char* extract_quoted_path(char** args, int index) {
    char* path = args[index];
    if (path[0] == '"') {
        int i = index + 1;
        while (args[i] != NULL && strchr(args[i], '"') == NULL) {
            i++;
        }
        if (args[i] != NULL) {
            args[i][strcspn(args[i], "\"")] = '\0';
            path = join_strings(args + index, i - index + 1, " ");
        }
    }
    return path;
}

void cp(char** args) {
    char* source = args[1];
    char* dest = args[2];

    if (source[0] == '"') {
        source = extract_quoted_path(args, 1);
    }

    if (dest[0] == '"') {
        dest = extract_quoted_path(args, 2);
    }

    FILE* src_file = fopen(source, "rb");
    if (src_file == NULL) {
        perror("cp");
        return;
    }

    FILE* dest_file = fopen(dest, "wb");
    if (dest_file == NULL) {
        perror("cp");
        fclose(src_file);
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

char* extract_path(char* str) {
    char* path = str;
    if (str[0] == '"') {
        path = strtok(str, "\"");
        if (path != NULL) {
            path = strdup(path);
        }
    }
    return path;
}

void delete(char* str) {
    char* path = extract_path(str);

    if (remove(path) != 0) {
        perror("delete");
    }

    if (path != str) {
        free(path);
    }
}

void mypipe(char** argv1, char** argv2) {
    HANDLE hChildStd_OUT_Rd = NULL;
    HANDLE hChildStd_OUT_Wr = NULL;
    SECURITY_ATTRIBUTES saAttr;

    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = TRUE;
    saAttr.lpSecurityDescriptor = NULL;

    if (!CreatePipe(&hChildStd_OUT_Rd, &hChildStd_OUT_Wr, &saAttr, 0)) {
        perror("CreatePipe");
        return;
    }

    PROCESS_INFORMATION piProcInfo1;
    STARTUPINFO siStartInfo1;

    ZeroMemory(&piProcInfo1, sizeof(PROCESS_INFORMATION));
    ZeroMemory(&siStartInfo1, sizeof(STARTUPINFO));

    siStartInfo1.cb = sizeof(STARTUPINFO);
    siStartInfo1.hStdOutput = hChildStd_OUT_Wr;
    siStartInfo1.dwFlags |= STARTF_USESTDHANDLES;

    if (!CreateProcess(NULL, argv1[0], NULL, NULL, TRUE, 0, NULL, NULL, &siStartInfo1, &piProcInfo1)) {
        perror("CreateProcess");
        return;
    }

    PROCESS_INFORMATION piProcInfo2;
    STARTUPINFO siStartInfo2;

    ZeroMemory(&piProcInfo2, sizeof(PROCESS_INFORMATION));
    ZeroMemory(&siStartInfo2, sizeof(STARTUPINFO));

    siStartInfo2.cb = sizeof(STARTUPINFO);
    siStartInfo2.hStdInput = hChildStd_OUT_Rd;
    siStartInfo2.dwFlags |= STARTF_USESTDHANDLES;

    if (!CreateProcess(NULL, argv2[0], NULL, NULL, TRUE, 0, NULL, NULL, &siStartInfo2, &piProcInfo2)) {
        perror("CreateProcess");
        return;
    }

    CloseHandle(hChildStd_OUT_Wr);

    WaitForSingleObject(piProcInfo1.hProcess, INFINITE);
    WaitForSingleObject(piProcInfo2.hProcess, INFINITE);

    CloseHandle(piProcInfo1.hProcess);
    CloseHandle(piProcInfo1.hThread);
    CloseHandle(piProcInfo2.hProcess);
    CloseHandle(piProcInfo2.hThread);
}