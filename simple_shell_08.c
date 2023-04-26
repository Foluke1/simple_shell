#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_INPUT_LENGTH 256

void display_prompt()
{
    printf("simple_shell$ ");
}

void execute_command(char *command)
{
    pid_t pid;
    int status;

    pid = fork();

    if (pid == 0)
    {
        // Child process
        if (execlp(command, command, NULL) == -1)
        {
            perror("execlp");
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

int main()
{
    char input[MAX_INPUT_LENGTH];
    char command[MAX_INPUT_LENGTH];

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

        // Check for built-in exit command
        if (strcmp(input, "exit") == 0)
        {
            // Exit command
            int status = 0;
            if (sscanf(command, "exit %d", &status) == 1)
            {
                exit(status);
            }
            else
            {
                exit(0);
            }
        }

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

        if (command[0] == '\0')
        {
            // Empty command, display prompt again
            continue;
        }

        execute_command(command);
    }

    return 0;
}
