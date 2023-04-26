#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_INPUT_LENGTH 256
#define MAX_ARGS 64

void display_prompt()
{
    printf("simple_shell$ ");
}

void read_input(char *input)
{
    fgets(input, MAX_INPUT_LENGTH, stdin);
    input[strcspn(input, "\n")] = '\0'; // Remove trailing newline character
}

void tokenize_input(char *input, char **args, int *num_args)
{
    char *token = strtok(input, " ");
    *num_args = 0;
    while (token != NULL)
    {
        args[*num_args] = token;
        (*num_args)++;
        token = strtok(NULL, " ");
    }
    args[*num_args] = NULL; // Set the last element to NULL as required by execve
}

void execute_command(char **args)
{
    pid_t pid;
    int status;

    pid = fork();
    if (pid == 0)
    {
        // Child process
        if (execve(args[0], args, environ) == -1)
        {
            perror("execve");
            exit(EXIT_FAILURE);
        }
    }
    else if (pid < 0)
    {
        // Forking error
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else
    {
        // Parent process
        waitpid(pid, &status, 0);
    }
}

int main(void)
{
    char input[MAX_INPUT_LENGTH];
    char *args[MAX_ARGS];
    int num_args;

    while (1)
    {
        display_prompt();
        read_input(input);

        if (feof(stdin))
        {
            // End of file (Ctrl+D) condition
            printf("\n");
            break;
        }

        tokenize_input(input, args, &num_args);

        if (num_args == 0)
        {
            // Empty command line, display prompt again
            continue;
        }

        if (strcmp(args[0], "exit") == 0)
        {
            // Exit the shell
            break;
        }

        execute_command(args);
    }

    return 0;
}
