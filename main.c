#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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