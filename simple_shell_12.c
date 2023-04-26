#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_CMD_LENGTH 1024
#define MAX_ARGS 64
#define DELIM " \t\r\n\a"
#define AND "&&"
#define OR "||"

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

char** parse_input(char* input, const char* delim) {
    int i = 0;
    char* token;
    char** args = malloc(MAX_ARGS * sizeof(char*));

    if (!args) {
        perror("malloc failed");
        exit(1);
    }

    token = strtok(input, delim);
    while (token != NULL) {
        args[i] = token;
        i++;

        token = strtok(NULL, delim);
    }
    args[i] = NULL;

    return args;
}

int main() {
    char input[MAX_CMD_LENGTH];
    char** args;
    int status;

    while (1) {
        printf("> ");
        if (!fgets(input, MAX_CMD_LENGTH, stdin)) {
            printf("\n");
            break;
        }

        // Check for AND and OR operators
        char* and_pos = strstr(input, AND);
        char* or_pos = strstr(input, OR);
        char* pos;
        if (and_pos && (!or_pos || and_pos < or_pos)) {
            pos = and_pos;
        } else {
            pos = or_pos;
        }

        if (pos) {
            *pos = '\0';
            args = parse_input(input, DELIM);
            status = execute_command(args);
            free(args);

            if ((status == 0 && pos == and_pos) || (status != 0 && pos == or_pos)) {
                continue;
            }

            input[0] = '\0';
            strncpy(input, pos + strlen(AND), MAX_CMD_LENGTH);
            input[strlen(input) - 1] = '\0';
        }

        args = parse_input(input, DELIM);
        status = execute_command(args);
        free(args);

        if (status != 0) {
            continue;
        }

        // Handle exit built-in
        if (strcmp(input, "exit") == 0) {
            break;
        }

        // Handle env built-in
        if (strcmp(input, "env") == 0) {
            char** env = environ;
            while (*env) {
                printf("%s\n", *env);
                env++;
            }
            continue;
        }
    }

    return 0;
}
