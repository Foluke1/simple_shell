#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_COMMAND_LENGTH 1024
#define MAX_ARGUMENTS 64

// Function to parse the command line and split it into arguments
int parse_command(char *command, char **args)
{
    int num_args = 0;
    char *arg = strtok(command, " \t\n"); // Split command by space, tab, or newline

    while (arg != NULL)
    {
        args[num_args++] = arg;
        arg = strtok(NULL, " \t\n");
    }

    args[num_args] = NULL; // Set last argument to NULL for execve

    return num_args;
}

// Function to handle the exit built-in command
void exit_shell()
{
    printf("Exiting the shell...\n");
    exit(0);
}

// Function to handle the env built-in command
void print_environment()
{
    extern char **environ;
    for (int i = 0; environ[i] != NULL; i++)
    {
        printf("%s\n", environ[i]);
    }
}

int main(int argc, char *argv[])
{
    char command[MAX_COMMAND_LENGTH];
    char *args[MAX_ARGUMENTS];
    int num_args;

    FILE *file = NULL;
    if (argc > 1)
    {
        file = fopen(argv[1], "r"); // Open file for reading
        if (file == NULL)
        {
            perror("Error:");
            exit(1);
        }
    }

    while (1)
    {
        if (file == NULL)
        {
            printf("simple_shell$ "); // Display the prompt if not reading from a file
            fgets(command, MAX_COMMAND_LENGTH, stdin); // Read the command from the user
        }
        else
        {
            if (fgets(command, MAX_COMMAND_LENGTH, file) == NULL)
            {
                // End of file reached, exit the shell
                fclose(file);
                exit_shell();
            }
            printf("%s", command); // Print command from file
        }

        num_args = parse_command(command, args); // Parse the command into arguments

        if (num_args == 0) // If no command is entered, continue to next iteration
        {
            continue;
        }

        // Handle built-in commands
        if (strcmp(args[0], "exit") == 0)
        {
            exit_shell();
        }
        else if (strcmp(args[0], "env") == 0)
        {
            print_environment();
            continue;
        }

        pid_t pid = fork();
        if (pid == 0)
        {
            // Child process
            execvp(args[0], args); // Execute the command
            perror("Error:"); // Print error message if execvp fails
            exit(1);
        }
        else if (pid > 0)
        {
            // Parent process
            wait(NULL); // Wait for child process to finish
        }
        else
        {
            // Fork failed
            perror("Error:");
            exit(1);
        }
    }

    return 0;
}
