/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_errors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amdemuyn <amdemuyn@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 17:33:31 by amdemuyn          #+#    #+#             */
/*   Updated: 2025/01/30 19:15:59 by amdemuyn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

char	*str_append(char *base, const char *to_append)
{
	size_t	new_len;
	char	*new_str;
	static int	i;

	new_len = ft_strlen(base) + ft_strlen(to_append) + 1;
	new_str = malloc(new_len);
	if (!new_str)
		return (NULL);
	ft_strlcpy(new_str, base, ft_strlen(base) + 1);
	ft_strlcat(new_str, to_append, ft_strlen(base) + ft_strlen(to_append) + 1);
	if(base)
		free(base);
	i++;
	return (new_str);
}

char	*info_error(char *cmd, char *info, char *s)
{
	char	*output;

	output = ft_strdup(s);
	if (cmd && (ft_strncmp(cmd, "export", 7) == 0
			|| ft_strncmp(cmd, "unset", 6) == 0))
		output = str_append(output, "`");
	output = str_append(output, info);
	if (cmd && (ft_strncmp(cmd, "export", 7) == 0
			|| ft_strncmp(cmd, "unset", 6) == 0))
		output = str_append(output, "'");
	output = str_append(output, ": ");
	free(s);
	return (output);
}

int	error_msg(char *cmd, char *info, char *msg, int err_nb)
{
	char	*output;

	output = ft_strdup("$-> ");
	if (!output)
		return (err_nb);
	if (cmd)
	{	
		output = str_append(output, cmd);
		output = str_append(output, ": ");

	}
	if (info)
	{
		output = info_error(cmd, info, output);
	}
	output = str_append(output, msg);
	write(STDERR_FILENO, output, ft_strlen(output));
	write(STDERR_FILENO, "\n", 1);
	free(output);
	return (err_nb);
}
