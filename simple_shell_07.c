#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_INPUT_LENGTH 256
#define MAX_ARGS 10

void display_prompt()
{
    printf("simple_shell$ ");
}

void execute_command(char *command, char *args[])
{
    pid_t pid;
    int status;

    pid = fork();

    if (pid == 0)
    {
        // Child process
        if (execvp(command, args) == -1)
        {
            perror("execvp");
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

void parse_input(char *input, char *command, char *args[])
{
    int arg_count = 0;
    char *token;

    // Extract command from input
    int i = 0;
    while (input[i] == ' ' || input[i] == '\t')
    {
        i++;
    }

    int j = 0;
    while (input[i] != '\0' && input[i] != ' ' && input[i] != '\t' && input[i] != '\n')
    {
        command[j++] = input[i++];
    }
    command[j] = '\0';

    // Extract arguments from input
    while (input[i] != '\0' && arg_count < MAX_ARGS - 1)
    {
        while (input[i] == ' ' || input[i] == '\t')
        {
            i++;
        }

        if (input[i] == '\0' || input[i] == '\n')
        {
            break;
        }

        j = 0;
        while (input[i] != '\0' && input[i] != ' ' && input[i] != '\t' && input[i] != '\n')
        {
            args[arg_count][j++] = input[i++];
        }
        args[arg_count][j] = '\0';
        arg_count++;
    }

    args[arg_count] = NULL; // Set last argument to NULL
}

int main()
{
    char input[MAX_INPUT_LENGTH];
    char command[MAX_INPUT_LENGTH];
    char args[MAX_ARGS][MAX_INPUT_LENGTH];

    while (1)
    {
        display_prompt();

        if (fgets(input, MAX_INPUT_LENGTH, stdin) == NULL)
        {
            // End of file (Ctrl+D) condition
            printf("\n");
            break;
        }

        // Remove trailing newline
        input[strcspn(input, "\n")] = '\0';

        parse_input(input, command, args);

        if (command[0] == '\0')
        {
            // Empty command, display prompt again
            continue;
        }

        execute_command(command, args);
    }

    return 0;
}
