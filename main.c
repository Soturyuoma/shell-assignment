#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <windows.h>

void Wellcome();
void getLocation();

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