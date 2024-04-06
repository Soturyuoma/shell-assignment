#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <windows.h>
#include <sys/stat.h>
#include "shell.h"

/**
 * Main function of the shell program.
 *
 * This function displays a welcome message, prompts the user for input,
 * and executes the corresponding command based on the user's input.
 * It continues to prompt the user for input until the "exit" command is entered.
 *
 * @return 0 on successful execution.
 */
int main() {
    printf("Welcome to my shell assignment!\n");
    Wellcome();

    char input[1024];
    char** args;

    while (1) {
        getLocation();
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';  // Remove trailing newline

        args = splitArgument(input);

        if (args == NULL) {
            fprintf(stderr, "Memory allocation failed.\n");
            continue;
        }

        if (args[0] == NULL) {
            free(args);
            continue;
        } else if (strcmp(args[0], "exit") == 0) {
            logout(input);
            free(args);
        } else if (strcmp(args[0], "move") == 0) {
            move(args);
            free(args);
        } else if (strcmp(args[0], "echoppend") == 0) {
            echoppend(args);
            free(args);
        } else if (strcmp(args[0], "echorite") == 0) {
            echorite(args);
            free(args);
        } else if (strcmp(args[0], "readFile") == 0) {
            readFile(args);
            free(args);
        } else if (strcmp(args[0], "wordCount") == 0) {
            wordCount(args);
            free(args);
        } else if (strcmp(args[0], "cd") == 0) {
            cd(args);
            free(args);
        } else if (strcmp(args[0], "cp") == 0) {
            cp(args);
            free(args);
        } else if (strcmp(args[0], "delete") == 0) {
            delete(args[1]);
            free(args);
        } else if (strcmp(args[0], "mypipe") == 0) {
    // Parse the arguments for mypipe
    char** argv1 = malloc((MAX_ARGS + 1) * sizeof(char*));
    char** argv2 = malloc((MAX_ARGS + 1) * sizeof(char*));
    if (argv1 == NULL || argv2 == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        free(args);
        free(argv1);
        free(argv2);
        continue;
    }

    int i, j, pipe_index = -1;
    for (i = 1; args[i] != NULL; i++) {
        if (strcmp(args[i], "|") == 0) {
            pipe_index = i;
            break;
        }
    }

    if (pipe_index == -1) {
        fprintf(stderr, "Invalid pipe command.\n");
        free(args);
        free(argv1);
        free(argv2);
        continue;
    }

    for (i = 1, j = 0; i < pipe_index; i++, j++) {
        argv1[j] = args[i];
    }
    argv1[j] = NULL;

    for (i = pipe_index + 1, j = 0; args[i] != NULL; i++, j++) {
        argv2[j] = args[i];
    }
    argv2[j] = NULL;

    mypipe(argv1, argv2);

    free(argv1);
    free(argv2);
    free(args);
} else {
            printf("Unknown command: %s\n", args[0]);
            free(args);
        }
    }

    return 0;
}