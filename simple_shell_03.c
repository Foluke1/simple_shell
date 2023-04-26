#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_INPUT_LENGTH 256
#define MAX_ARGS 64

void display_prompt()
{
    printf("simple_shell$ ");
}

void parse_command(char *input, char **command, char **args)
{
    int i = 0;
    char *token = strtok(input, " \t\n");
    while (token != NULL)
    {
        if (i == 0)
            *command = token;
        else
            args[i - 1] = token;
        i++;
        token = strtok(NULL, " \t\n");
    }
    args[i - 1] = NULL;
}

void execute_command(char *command, char **args)
{
    pid_t pid = fork();
    if (pid == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        // Child process
        execvp(command, args);
        perror("execvp");
        exit(EXIT_FAILURE);
    }
    else
    {
        // Parent process
        wait(NULL);
    }
}

int main()
{
    char input[MAX_INPUT_LENGTH];
    char *command;
    char *args[MAX_ARGS];

    while (1)
    {
        display_prompt();
        if (fgets(input, MAX_INPUT_LENGTH, stdin) == NULL)
        {
            printf("\n");
            break; // End of file (Ctrl+D) condition
        }

        // Remove trailing newline character
        input[strcspn(input, "\n")] = '\0';

        // Parse command and arguments
        parse_command(input, &command, args);

        // Check if command exists
        if (access(command, X_OK) == -1)
        {
            printf("Command not found: %s\n", command);
            continue; // Display prompt again
        }

        // Execute command
        execute_command(command, args);
    }

    return 0;
}
