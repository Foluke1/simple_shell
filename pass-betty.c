#include <stdio.h>
/**
 * factorial - Calculates the factorial of a given number.
 * @n: The number to calculate the factorial for.
 *
 * Return: The factorial of n.
 */
unsigned int factorial(unsigned int n)
{
	if (n == 0)
		return (1);
	else
		return (n * factorial(n - 1));
}

/**
 * main - Entry point of the program.
 *
 * Return: Always 0 (success).
 */
int main(void)
{
	unsigned int num = 5;
	unsigned int result;

	result = factorial(num);

	printf("The factorial of %u is: %u\n", num, result);

	return (0);
}
