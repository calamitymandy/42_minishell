//ADD HEADER

#include "minishell.h"

/**
 * The function `is_directory` checks if a given path is a directory or not.
 * returns a boolean value indicating whether the given `command` path
 * is a directory or not. It uses the `stat` function to retrieve information 
 * about the file specified by `command` and then checks if it is a directory 
 * using the `S_ISDIR` macro.
 */
bool	is_directory(char *cmd)
{
	struct stat	cmd_stat;

	ft_memset(&cmd_stat, 0, sizeof(cmd_stat));
	stat(cmd, &cmd_stat);
	return (S_ISDIR(cmd_stat.st_mode));
}