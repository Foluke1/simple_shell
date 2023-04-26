#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_INPUT_LENGTH 1024
#define MAX_ARG_COUNT 64
#define MAX_ARG_LENGTH 64

void display_prompt() {
    printf("simple_shell$ ");
}

void read_command(char *input) {
    if (fgets(input, MAX_INPUT_LENGTH, stdin) == NULL) {
        if (feof(stdin)) {
            printf("\n");
            exit(0);
        } else {
            perror("Error reading input");
            exit(1);
        }
    }
}

void parse_command(char *input, char **command, char **args, int *arg_count) {
    char *token;
    token = strtok(input, " \t\n");
    if (token == NULL) {
        *arg_count = 0;
        return;
    }

    *command = token;
    *arg_count = 0;

    while (token != NULL) {
        token = strtok(NULL, " \t\n");
        if (token != NULL) {
            args[*arg_count] = token;
            (*arg_count)++;
        }
    }
    args[*arg_count] = NULL;
}

void execute_command(char *command, char **args, int arg_count) {
    pid_t pid;
    int status;

    pid = fork();
    if (pid == -1) {
        perror("Error forking process");
        exit(1);
    } else if (pid == 0) {
        // Child process
        if (execvp(command, args) == -1) {
            perror("Error executing command");
            exit(1);
        }
    } else {
        // Parent process
        do {
            waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
}

int main() {
    char input[MAX_INPUT_LENGTH];
    char *command;
    char *args[MAX_ARG_COUNT];
    int arg_count;

    while (1) {
        display_prompt();
        read_command(input);
        parse_command(input, &command, args, &arg_count);

        if (arg_count == 0) {
            continue;
        }

        if (strcmp(command, "exit") == 0) {
            exit(0);
        } else if (strcmp(command, "env") == 0) {
            char **environ = __environ;
            while (*environ != NULL) {
                printf("%s\n", *environ);
                environ++;
            }
        } else if (strcmp(command, "alias") == 0) {
            // TODO: Implement alias functionality
            printf("Alias functionality not implemented\n");
        } else {
            execute_command(command, args, arg_count);
        }
    }

    return 0;
}
