#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_CMD_LENGTH 1024
#define MAX_ARGS 64
#define DELIM ";"

int execute_command(char** args) {
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        return 1;
    } else if (pid == 0) {
        // Child process
        if (execvp(args[0], args) == -1) {
            perror("execvp failed");
            exit(1);
        }
    } else {
        // Parent process
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
            printf("Command '%s' failed with status %d\n", args[0], WEXITSTATUS(status));
            return 1;
        }
    }
    return 0;
}

char** parse_input(char* input) {
    int i = 0;
    char* token;
    char** args = malloc(MAX_ARGS * sizeof(char*));

    if (!args) {
        perror("malloc failed");
        exit(1);
    }

    token = strtok(input, " \t\r\n\a");
    while (token != NULL) {
        args[i] = token;
        i++;

        token = strtok(NULL, " \t\r\n\a");
    }
    args[i] = NULL;

    return args;
}

int main() {
    char input[MAX_CMD_LENGTH];
    char* command;
    char** args;
    int status;

    while (1) {
        printf("> ");
        if (!fgets(input, MAX_CMD_LENGTH, stdin)) {
            printf("\n");
            break;
        }

        command = strtok(input, DELIM);
        while (command != NULL) {
            args = parse_input(command);
            status = execute_command(args);
            free(args);

            command = strtok(NULL, DELIM);
        }
    }

    return 0;
}
