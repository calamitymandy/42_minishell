//ADD HEADER

#include "minishell.h"

/**
 * The function `error_msg` generates and outputs an error message with 
 * specific formatting based on the input parameters.
 * 
 * 1. Initializes the output string with the PROMPT prefix.
 * 2. Appends the command, if provided.
 * 3. Appends additional information with special handling for specific 
 * 	commands export or unset.
 * 4. Appends the error message.
 * 5. Prints the formatted message to standard error.
 * 6. Returns the provided error number.
 */

int	error_msg(char *cmd, char *info, char *msg, int err_nb)
{
	char	*output;
	char	*temp;

	output = ft_strdup("$-> ");  // Initial allocation
	if (cmd)
	{
		temp = output;
		output = ft_strjoin(output, cmd);  // Concatenate cmd
		free(temp);  // Free the previous allocation
		temp = output;
		output = ft_strjoin(output, ": ");  // Concatenate ": "
		free(temp);
	}
	if (info)
	{
		if (ft_strncmp(cmd, "export", 7) == 0
			|| ft_strncmp(cmd, "unset", 6) == 0)
		{
			temp = output;
			output = ft_strjoin(output, "`");  // Concatenate "`"
			free(temp);
		}
		temp = output;
		output = ft_strjoin(output, info);  // Concatenate info
		free(temp);
		if (ft_strncmp(cmd, "export", 7) == 0
			|| ft_strncmp(cmd, "unset", 6) == 0)
		{
			temp = output;
			output = ft_strjoin(output, "'");  // Concatenate "'"
			free(temp);
		}
		temp = output;
		output = ft_strjoin(output, ": ");  // Concatenate ": "
		free(temp);
	}
	temp = output;
	output = ft_strjoin(output, msg);  // Concatenate the final message
	free(temp);
	ft_putendl_fd(output, STDERR_FILENO);  // Print the error message
	free_star(output);  // Free the final string
	return (err_nb);
}