#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_INPUT_LENGTH 256

static char buffer[MAX_INPUT_LENGTH];
static int buffer_index = 0;

char *my_getline()
{
    int c;
    while ((c = getchar()) != EOF && c != '\n')
    {
        buffer[buffer_index++] = c;
    }
    buffer[buffer_index] = '\0';
    buffer_index = 0;

    if (c == EOF)
    {
        return NULL;
    }

    return buffer;
}

void execute_command(char *command)
{
    if (fork() == 0)
    {
        execlp(command, command, (char *)NULL);
        perror("execlp");
        exit(EXIT_FAILURE);
    }
    else
    {
        wait(NULL);
    }
}

int main()
{
    char *input;

    while (1)
    {
        printf("simple_shell$ ");
        input = my_getline();

        if (input == NULL)
        {
            printf("\n");
            break; // End of file (Ctrl+D) condition
        }

        // Execute command
        execute_command(input);
    }

    return 0;
}
