# Shell Assignment

This is a simple shell program implemented in C for an operating systems course assignment.

## Features

- Displays a welcome message with ASCII art
- Shows the current user, hostname, and directory
- Supports basic shell commands:
  - `move`: Move a file from source path to destination path
  - `echoppend`: Append a string to a file
  - `echorite`: Overwrite a file with a string
  - `read`: Read and display the contents of a file
  - `wordCount`: Count the number of lines or words in a file
  - `cd`: Change the current directory
  - `cp`: Copy a file from source path to destination path
  - `delete`: Delete a file
  - `mypipe`: Execute two commands with piping
  - `logout`: Exit the shell program

## Usage

1. Compile the program using a C compiler: gcc -o myshell main.c shell.c -std=c99
2. Run the compiled executable: ./myshell
3. Enter shell commands at the prompt.
4. Command Examples:
Move a file: move "tests\test1\file1.txt" "tests\test2\file1.txt"
Append text to a file: echoppend text "tests\test1\file1.txt"
Read a file: readFile "tests\test2\file2.txt"
Overwrite a file with text: echorite text "tests\test1\file1.txt"
Count lines in a file: wordCount -l "tests\test1\file1.txt"
Count words in a file: wordCount -w "tests\test1\file1.txt"
Copy a file: cp "tests\test1\file1.txt" "tests\test1\file1_copy.txt"
Change directory: cd "filepath"
Exit the shell: exit
Note: File paths containing spaces should be enclosed in double quotes.

## Assumptions and Limitations

- The program assumes a Windows operating system and uses Windows-specific functions for certain operations.
- File paths containing spaces should be enclosed in double quotes.
- The `mypipe` command supports piping between two commands only.
- Error handling is basic and may not cover all possible scenarios.
- The program has been tested on a limited set of inputs and may have undiscovered bugs or limitations.

## Code Structure

- `shell.h`: Header file containing function declarations
- `main.c`: Main source file containing function implementations

## Testing

The program has been tested with the following scenarios:

- Moving files with simple paths and paths containing spaces
- Appending and overwriting files with strings
- Reading contents of files
- Counting lines and words in files
- Changing directories
- Copying files
- Deleting files
- Executing commands with piping

However, the testing may not be exhaustive, and there could be additional edge cases or scenarios that have not been covered.

## Known Issues

- The program may not handle all possible error cases gracefully.
- The `mypipe` command may have limitations and may not work correctly in all situations.

## Future Improvements

- Enhance error handling to cover more scenarios
- Implement additional shell commands and features
- Improve the user interface and command parsing
- Support more advanced piping and redirection
- Add support for other operating systems

## Author

[Soturyuoma]
[sakerkhier@gmail.com]

## Date

[30/03/2024]
