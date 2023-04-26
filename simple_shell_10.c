#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_INPUT_LENGTH 1024 // Maximum length of user input
#define MAX_ARG_LENGTH 256    // Maximum length of each argument
#define MAX_NUM_ARGS 64       // Maximum number of arguments

// Function declarations
void display_prompt();
void read_input(char *input);
int parse_input(char *input, char *args[]);
void execute_command(char *args[]);

int main()
{
    char input[MAX_INPUT_LENGTH]; // Buffer to store user input
    char *args[MAX_NUM_ARGS];     // Array to store parsed arguments

    while (1)
    {
        display_prompt();   // Display prompt
        read_input(input); // Read user input
        if (strlen(input) == 0)
            continue; // Skip if input is empty

        int num_args = parse_input(input, args); // Parse input into arguments
        if (num_args == -1)
        {
            printf("Error: Too many arguments\n");
            continue;
        }

        execute_command(args); // Execute the command with arguments
    }

    return 0;
}

// Function to display the prompt
void display_prompt()
{
    printf("simple_shell> ");
}

// Function to read user input
void read_input(char *input)
{
    fgets(input, MAX_INPUT_LENGTH, stdin);
    input[strcspn(input, "\n")] = '\0'; // Remove trailing newline character
}

// Function to parse input into arguments
int parse_input(char *input, char *args[])
{
    int num_args = 0;
    char *arg = strtok(input, " ");
    while (arg != NULL)
    {
        args[num_args++] = arg;
        if (num_args >= MAX_NUM_ARGS)
            return -1; // Return error if too many arguments
        arg = strtok(NULL, " ");
    }
    args[num_args] = NULL; // Set last argument to NULL for execvp() to work
    return num_args;
}

// Function to execute the command with arguments
void execute_command(char *args[])
{
    pid_t pid = fork(); // Fork a child process
    if (pid < 0)
    {
        printf("Error: Fork failed\n");
        exit(1);
    }
    else if (pid == 0)
    {
        // Child process
        if (execvp(args[0], args) == -1)
        {
            printf("Error: Command not found\n");
            exit(1);
        }
    }
    else
    {
        // Parent process
        wait(NULL); // Wait for child process to complete
    }
}
