#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_INPUT_LENGTH 256

void display_prompt()
{
	printf("simple_shell$ ");
}

void read_input(char *input)
{
	fgets(input, MAX_INPUT_LENGTH, stdin);
	input[strcspn(input, "\n")] = '\0'; // Remove trailing newline character
}

void execute_command(char *command)
{
	pid_t pid;
	int status;

	pid = fork();
	if (pid == 0)
	{
	       	// Child process
		char *args[] = {command, NULL};
		if (execve(command, args, environ) == -1)
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
	char command[MAX_INPUT_LENGTH];
	
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

		sscanf(input, "%s", command);

		if (strcmp(command, "exit") == 0)
		{
			// Exit the shell
			break;
		}

	execute_command(command);
    }

    return 0;
}
