#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

//strcmp retourne 0 si les deux chaînes sont identiques.

int main(int argc, char **argv, char **envp)
{
	int	status = 0;
	int i = 0;

	if (argc > 1)
	{
		while (argv[i] && argv[++i])
		{
			argv += i;

			i = 0;
			while (argv[i] && strcmp(argv[i], "|") && strcmp(argv[i], ";"))
				i++;
			if (strcmp(*argv, "cd") == 0)
				status = cd(argv, i);
			else if (i)
				status = exec(argv, envp, i);
		}
	}
	return (status);
}