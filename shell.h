#ifndef SHELL_H
#define SHELL_H

void Wellcome();
void getLocation();
char* extract_path(char* str);
void move(char** args);
void echoppend(char** args);
void echorite(char** args);
void readFile(char** args);
void wordCount(char** args);
char** splitArgument(char* str);
char* strtrim(char* str);
void logout(char* str);
char* join_strings(char** strings, int count, const char* delimiter);
void cd(char** args);
char* extract_quoted_path(char** args, int index);
void cp(char** args);
void delete(char* str);
void mypipe(char** argv1, char** argv2);

#endif