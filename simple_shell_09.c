#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_INPUT_LENGTH 1024
#define MAX_ARGS 64
#define DELIM " \t\r\n\a"

void display_prompt() {
    printf("simple_shell $ ");
}

char* read_command() {
    char* command = (char*)malloc(MAX_INPUT_LENGTH * sizeof(char));
    if (command == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    fgets(command, MAX_INPUT_LENGTH, stdin);
    return command;
}

void execute_command(char* command) {
    int status;
    char* args[MAX_ARGS];
    int num_args = 0;
    pid_t pid;
    
    // Tokenize the command into arguments
    char* token = strtok(command, DELIM);
    while (token != NULL) {
        args[num_args++] = token;
        token = strtok(NULL, DELIM);
    }
    args[num_args] = NULL;
    
    if (num_args > 0) {
        if (strcmp(args[0], "exit") == 0) {
            exit(0);
        } else if (strcmp(args[0], "env") == 0) {
            for (int i = 0; environ[i] != NULL; i++) {
                printf("%s\n", environ[i]);
            }
        } else if (strcmp(args[0], "setenv") == 0) {
            if (num_args != 3) {
                fprintf(stderr, "Error: Incorrect syntax for setenv. Usage: setenv VARIABLE VALUE\n");
            } else {
                if (setenv(args[1], args[2], 1) == -1) {
                    perror("setenv");
                }
            }
        } else if (strcmp(args[0], "unsetenv") == 0) {
            if (num_args != 2) {
                fprintf(stderr, "Error: Incorrect syntax for unsetenv. Usage: unsetenv VARIABLE\n");
            } else {
                if (unsetenv(args[1]) == -1) {
                    perror("unsetenv");
                }
            }
        } else {
            pid = fork();
            if (pid < 0) {
                perror("fork");
                exit(EXIT_FAILURE);
            } else if (pid == 0) {
                // Child process
                execvp(args[0], args);
                perror("execvp");
                exit(EXIT_FAILURE);
            } else {
                // Parent process
                waitpid(pid, &status, 0);
            }
        }
    }
}

int main() {
    char* command;
    
    while (1) {
        display_prompt();
        command = read_command();
        execute_command(command);
        free(command);
    }
    
    return 0;
}
