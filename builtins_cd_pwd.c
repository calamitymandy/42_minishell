/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_cd_pwd.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amdemuyn <amdemuyn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 17:18:14 by amdemuyn          #+#    #+#             */
/*   Updated: 2025/01/28 17:21:12 by amdemuyn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* The function update_pwd_n_old is responsible for updating the current 
 * working directory (PWD) and the previous working directory (OLDPWD) 
 * environment variables when a directory change occurs in the shell. 
 * 
 * 1- Retrieves the current value of the PWD (current working directory)
 *  using get_env_value() and sets this value as the new OLDPWD 
 * 2- Sets the current working directory (PWD) to the new path stored 
 * 	in buf_of_work_dir_path.
 * 3- Free and update mini->old_pwd
 * 4- Free and update mini->pwd
*/
void	update_pwd_n_old(t_minishell *mini, char *buf_of_work_dir_path)
{
	add_or_update_env_var(mini, "OLDPWD", get_env_value(mini->env, "PWD"));
	add_or_update_env_var(mini, "PWD", buf_of_work_dir_path);
	if (mini->old_pwd)
	{
		free_star(mini->old_pwd);
		mini->old_pwd = ft_strdup(mini->pwd);
	}
	if (mini->pwd)
	{
		free_star(mini->pwd);
		mini->pwd = ft_strdup(buf_of_work_dir_path);
	}
	free_star(buf_of_work_dir_path);
}

/* getcwd: Get the pathname of the current working directory, and put it 
 * in SIZE bytes of BUF. Returns NULL if the directory couldn't be determined 
 * or SIZE was too small.
 * 
 * If successful, returns BUF.
*/
bool	cd(t_minishell *mini, char *path)
{
	char	*buf_of_work_dir_path;
	char	*temp;
	char	cwd[4096];

	buf_of_work_dir_path = NULL;
	if (chdir(path) != 0)
	{
		error_msg("cd", path, "Error: No such file or directory", 2);
		return (false);
	}
	buf_of_work_dir_path = getcwd(cwd, 4096);
	if (!buf_of_work_dir_path)
	{
		error_msg("cd: error retrieving current directory",
			"getcwd: cannot access parent directories", strerror(errno), errno);
		buf_of_work_dir_path = ft_strjoin(mini->pwd, "/");
		temp = buf_of_work_dir_path;
		buf_of_work_dir_path = ft_strjoin(temp, path);
		free_star(temp);
	}
	else
		buf_of_work_dir_path = ft_strdup(cwd);
	update_pwd_n_old(mini, buf_of_work_dir_path);
	return (true);
}

/**
 * The exec_cd function handles the logic for the cd command in a shell.
 * 
 * If no arg or no valid arg (no valid path) was passed to cd, the function 
 * tries to retrieve the value of the HOME environment variable, which 
 * is the default directory to go to when running cd without arguments.
 * 
 * Then handles the special case of cd -, which changes to the previous 
 * directory (OLDPWD).
 * 
 * It handles errors for missing arguments, HOME, or OLDPWD not being set,
 * and too many arguments being passed.
 * 
 * Finally, it changes the directory using cd(mini, path) when appropriate.
 */
int	exec_cd(t_minishell *mini, char **args)
{
	char	*path;

	if (!args[1] || ft_isblank(args[1][0]) || args[1][0] == '\0')
	{
		path = get_env_value(mini->env, "HOME");
		if (!path || *path == '\0' || ft_isblank(*path))
			return (error_msg("cd", NULL, "HOME not set", EXIT_FAILURE));
		return (!cd(mini, path));
	}
	if (args[2])
		return (error_msg("cd", NULL, "Error: too many arguments", 1));
	if (ft_strncmp(args[1], "-", 2) == 0)
	{
		path = get_env_value(mini->env, "OLDPWD");
		if (!path)
			return (error_msg("cd", NULL,
					"Error: OLDPWD not set", EXIT_FAILURE));
		ft_putendl_fd(path, 2);
		return (!cd(mini, path));
	}
	return (!cd(mini, args[1]));
}

/*  * 4096 is path max *
 * 1- IF cd was used, mini->pwd is set, so it prints it.
 * 2- if not: get cwd whith getcwd()
 * 3- IF cwd prints it.
 * 4- if not -> error_msg
 *
 * getcwd: Get the pathname of the current working directory, and put it 
 * in SIZE bytes of BUF. Returns NULL if the directory couldn't be determined 
 * or SIZE was too small.
 * If successful, returns BUF.
*/
int	exec_pwd_builtin(t_minishell *mini, char **args)
{
	char	buffer[4096];
	char	*cwd;

	(void)args;
	if (mini->pwd)
	{
		ft_putendl_fd(mini->pwd, STDOUT_FILENO);
		return (EXIT_SUCCESS);
	}
	cwd = getcwd(buffer, 4096);
	if (cwd)
	{
		ft_putendl_fd(cwd, STDOUT_FILENO);
		return (EXIT_SUCCESS);
	}
	error_msg("pwd", NULL, strerror(errno), errno);
	return (EXIT_FAILURE);
}
