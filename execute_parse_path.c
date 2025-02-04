/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_parse_path.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amdemuyn <amdemuyn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 18:42:36 by amdemuyn          #+#    #+#             */
/*   Updated: 2025/02/04 19:27:24 by amdemuyn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* mix de get_env_path & get_cmd_path 
 *
 * get_path_cmd function takes a command (such as ls, grep, etc.) and 
 * searches for its location within the directories specified by the PATH 
 * environment variable. If it finds a path where the command is executable,
 * it returns the full path. If an error occurs at any point (for example,
 * if PATH doesn't exist or if no valid path is found), the function frees
 * the used memory and returns NULL.
*/

char	*get_path_cmd(t_minishell *mini, char *str)
{
	char	**env_paths;
	char	*cmd;
	char	*cmd_path;

	if (!str)
		return (NULL);
	if (find_env_index_of_key(mini->env, "PATH") == -1)
		return (NULL);
	env_paths = ft_split(get_env_value(mini->env, "PATH"), ':');
	if (!env_paths)
		return (NULL);
	cmd = ft_strjoin("/", str);
	if (!cmd)
	{
		free_two_stars(env_paths);
		return (NULL);
	}
	cmd_path = find_valid_cmd_path(cmd, env_paths);
	if (!cmd_path)
	{
		free_star(cmd);
		free_two_stars(env_paths);
		return (NULL);
	}
	return (cmd_path);
}

/* The function find_valid_cmd_path is designed to locate the 
 * full path to a valid command by searching through a list of 
 * directories (all_paths). If the command is found in one of 
 * these directories and it is executable, the function returns 
 * the complete path. */
char	*find_valid_cmd_path(char *cmd, char **all_paths)
{
	char	*cmd_path;
	int		i;

	cmd_path = NULL;
	i = 0;
	while (all_paths[i])
	{
		cmd_path = ft_strjoin(all_paths[i], cmd);
		if (!cmd_path)
		{
			error_msg("malloc", NULL, "an unexpected error occured",
				EXIT_FAILURE);
			return (NULL);
		}
		if (access(cmd_path, F_OK | X_OK) == 0)
			return (cmd_path);
		free_star(cmd_path);
		i++;
	}
	return (NULL);
}
