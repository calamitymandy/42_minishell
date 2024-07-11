/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amdemuyn <amdemuyn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 20:24:50 by amdemuyn          #+#    #+#             */
/*   Updated: 2024/07/11 21:03:25 by amdemuyn         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "minishell.h"
/*compile with gcc main.c -lreadline*/

/**
 * The function `error_msg` generates and outputs an error message with 
 * specific formatting based on the input parameters.
 * 
 * @param info The `info` parameter in the `error_msg` function is a string 
 * that contains additional information related to the error message being 
 * displayed. It is used to provide context or details about the error that 
 * occurred.
 * @param msg The `msg` parameter in the `error_msg` function is a string 
 * that represents the error message to be displayed.
 * @param err_nb The `err_nb` parameter in the `error_msg` function is an 
 * integer that represents the error number or code associated with the 
 * error message being displayed. It is used to indicate the specific type 
 * of error that occurred during the execution of a command.
 */
int	error_msg(char *cmd, char *info, char *msg, int err_nb)
{
	char	*output;

	output = ft_strdup("$-> ");
	if (cmd)
	{
		output = ft_strjoin(output, cmd);
		output = ft_strjoin(output, ": ");
	}
	if (info)
	{
		if (ft_strncmp(cmd, "export", 7) == 0
			|| ft_strncmp(cmd, "unset", 6) == 0)
			output = ft_strjoin(output, "`");
		output = ft_strjoin(output, info);
		if (ft_strncmp(cmd, "export", 7) == 0
			|| ft_strncmp(cmd, "unset", 6) == 0)
			output = ft_strjoin(output, "'");
		output = ft_strjoin(output, ": ");
	}
	output = ft_strjoin(output, msg);
	ft_putendl_fd(output, STDERR_FILENO);
	//TODO FREE
	return (err_nb);
}

char	*get_env_var_value(char **env, char *key)
{
	int		i;
	char	*temp;

	temp = ft_strjoin(key, "=");
	if (!temp)
		return (NULL);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(temp, env[i], ft_strlen(temp)) == 0)
		{
			//TODO FREE
			return (ft_strchr(env[i], '=') + 1);
		}
		i++;
	}
	//TODO FREE
	return (NULL);
}

int	srch_env_i(char **env, char *pwd_or_old)
{
	int		i;
	char	*temp;

	temp = ft_strjoin(pwd_or_old, "=");
	if (!temp)
		return (-1);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(temp, env[i], ft_strlen(temp)) == 0)
		{
			//TODO FREE
			return (i);
		}
		i++;
	}
	//TODO FREE
	return (-1);
}

int	nb_env_variables(char **env)
{
	int	i;

	i = 0;
	while (env && env[i])
		i++;
	return (i);
}

char	**callocate_env_variables(t_minishell *mini, int size)
{
	char	**new_env;
	int		i;
	
	new_env = ft_calloc(size + 1, sizeof * new_env);
	if (!new_env)
		return (NULL);
	i = 0;
	while (mini->env[i] && i < size)
	{
		new_env[i] = ft_strdup(mini->env[i]);
		//TODO FREE
		i++;
	}
	//TODO FREE
	return (new_env);
}

bool	add_or_update_env_var(t_minishell *mini, char *pwd_or_old, char *value)
{
	int 	i;
	char	*temp;

	i = srch_env_i(mini->env, pwd_or_old);
	if (value == NULL)
		value = "";
	temp = ft_strjoin("=", value);
	if (!temp)
		return (false);
	if (i != -1 && mini->env[i])
	{
		//TODO FREE
		mini->env[i] = ft_strjoin(pwd_or_old, temp);
	}
	else
	{
		i = nb_env_variables(mini->env);
		mini->env = callocate_env_variables(mini, i + 1);
		if (!mini->env)
			return (false);
		mini->env[i] = ft_strjoin(pwd_or_old, temp);
	}
	//TODO FREE
	return (true);
}

void	update_pwd_n_old(t_minishell *mini, char *buf_of_work_dir_path)
{
	add_or_update_env_var(mini, "OLDPWD", get_env_var_value(mini->env, "PWD"));
	add_or_update_env_var(mini, "PWD", buf_of_work_dir_path);
	if (mini->old_pwd)
	{
		//TODO FREE
		mini->old_pwd =ft_strdup(mini->pwd);
	}
	if (mini->pwd)
	{
		//TODO FREE
		mini->pwd =ft_strdup(buf_of_work_dir_path);
	}
	//TODO FREE
}

/*getcwd: Get the pathname of the current working directory,
and put it in SIZE bytes of BUF. Returns NULL if the
directory couldn't be determined or SIZE was too small.
If successful, returns BUF.
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
		//TODO FREE
	}
	else
		buf_of_work_dir_path = ft_strdup(cwd);
	update_pwd_n_old(mini, buf_of_work_dir_path);
	return (true);
}

int	exec_cd(t_minishell *mini, char **args)
{
	char	*path;

	if (!args[1] || ft_isblank(args[1][0]) || args[1][0] == '\0')
	{
		path = get_env_var_value(mini->env, "HOME");
		if (!path || *path == '\0' || ft_isblank(*path))
			return (error_msg("cd", NULL, "HOME not set", EXIT_FAILURE));
		return (!cd(mini, path));
	}
	if (args[2])
		return (error_msg("cd", NULL, "Error: too many arguments", 1));
	if (ft_strncmp(args[1], "-", 2) == 0)
	{
		path = get_env_var_value(mini->env, "OLDPWD");
		if (!path)
			return (error_msg("cd", NULL, "Error: OLDPWD not set", EXIT_FAILURE));
		ft_putendl_fd(path, 2);
		return (!cd(mini, path));
	}
	return (!cd(mini, args[1]));
}

int	exec_builtin(t_minishell *mini, t_command *cmd)
{
	int	cmd_res;

	cmd_res = 127;
	if (ft_strncmp(cmd->cmd, "cd", 3) == 0)
		cmd_res = exec_cd(mini, cmd->args);
	return (cmd_res);
}

int	g_status;

bool	check_in_and_out(t_fds	*in_n_out)
{
	if (!in_n_out || (!in_n_out->infile && !in_n_out->outfile))
		return (true);
	if ((in_n_out->infile && in_n_out->fd_infile == -1)
		|| (in_n_out->outfile && in_n_out->fd_outfile == -1))
		return (false);
	return (true);
}

bool	config_in_and_out(t_fds	*in_n_out)
{
	int	result;

	result = true;
	if (!in_n_out)
		return (result);
	in_n_out->stdin_ori = dup(STDIN_FILENO);
	in_n_out->stdout_ori = dup(STDOUT_FILENO);
	if (in_n_out->stdin_ori == -1)
		result = printf("TODO stdin_ori MSG ERROR");
	if (in_n_out->stdout_ori == -1)
		result = printf("TODO stdout_ori MSG ERROR");
	if (in_n_out->fd_infile != -1
		&& (dup2(in_n_out->fd_infile, STDIN_FILENO) == -1))
		result = printf("TODO dup2 fd_infile MSG ERROR");
	if (in_n_out->fd_outfile != -1
		&& (dup2(in_n_out->fd_outfile, STDIN_FILENO) == -1))
		result = printf("TODO dup2 fd_outfile MSG ERROR");
	return (result);
}

bool	reset_fds_in_and_out(t_fds *fds_in_and_out)
{
	int	res;

	res = true;
	if (!fds_in_and_out)
		return (res);
	if (fds_in_and_out->stdin_ori != -1)
	{
		if (dup2(fds_in_and_out->stdin_ori, STDIN_FILENO) == -1)
			res = false;
		close(fds_in_and_out->stdin_ori);
		fds_in_and_out->stdin_ori = -1;
	}
	if (fds_in_and_out->stdout_ori != -1)
	{
		if (dup2(fds_in_and_out->stdout_ori, STDOUT_FILENO) == -1)
			res = false;
		close(fds_in_and_out->stdout_ori);
		fds_in_and_out->stdout_ori = -1;
	}
	return (res);
}

void	close_pipe_fd(t_command *cmd, t_command *skip_cmd)
{
	while (cmd)
	{
		if (cmd != skip_cmd && cmd->pipe_fd)
		{
			close(cmd->pipe_fd[0]);
			close(cmd->pipe_fd[1]);
		}
		cmd = cmd->next;
	}
}

void	close_fds(t_command *cmd, bool close_or_not)
{
	if (cmd->fds)
	{
		if (cmd->fds->fd_infile || cmd->fds->fd_outfile)
		{
			if (cmd->fds->fd_infile != -1)
				close (cmd->fds->fd_infile);
			else
				close (cmd->fds->fd_outfile);
		}
		if (close_or_not)
			reset_fds_in_and_out(cmd->fds);
	}
	close_pipe_fd(cmd, NULL);
}

void	exit_mini(t_minishell *mini, int exit_code)
{
	if (mini)
	{
		if (mini->cmd && mini->cmd->fds)
			close_fds(mini->cmd, true);
		//free data
	}
	exit(exit_code);
}

bool	init_main_struct(t_minishell *mini, char **env)
{
	//TODO set_env_var & set_pwd_and aldpwd
	mini->env = env;
	mini->line = NULL;
	//mini->ctrlc_heredoc = false;
	mini->token = NULL;
	mini->cmd = NULL;
	mini->pid = -1;
	g_status = 0;
	return (true);
}

bool	create_pipes(t_minishell *mini)
{
	int			*fd;
	t_command	*temp_cmd;

	temp_cmd = mini->cmd;
	while (temp_cmd)
	{
		if (temp_cmd->pipe_output
			|| (temp_cmd->prev && temp_cmd->prev->pipe_output))
		{
			fd = malloc (sizeof * fd * 2);
			if (!fd || pipe (fd) != 0)
			{
				//free data
				return (false);
			}
			temp_cmd->pipe_fd = fd;
		}
		temp_cmd = temp_cmd->next;
	}
	return (true);
}

int	prep_the_cmd(t_minishell *mini)
{
	printf("here it prints");
	//TODO init mini->cmd->cmd & mini->token->has_quotes somewhere
	if (!mini || !mini->cmd || !mini->cmd->cmd
		|| (mini->cmd->cmd[0] == '\0' && mini->token->has_quotes == false))
		return (EXIT_SUCCESS);
	printf("here it does not print");
	if (mini->cmd && !mini->cmd->cmd)
	{
		if (mini->cmd->fds && !check_in_and_out(mini->cmd->fds))
			return (EXIT_FAILURE);
		return (EXIT_SUCCESS);
	}
	if (!create_pipes(mini))
		return (EXIT_FAILURE);
	return (127);
}

/* This function executes the processed command and returns a status code 
that updates g_status. 
127 = cmd unknown*/
int	exec_main(t_minishell *mini)
{
	int	result;

	result = prep_the_cmd(mini);
	if (result != 127)
		return (result);
	if (!mini->cmd->pipe_output && !mini->cmd->prev
		&& check_in_and_out(mini->cmd->fds))
	{
		config_in_and_out(mini->cmd->fds);
		result = exec_builtin(mini, mini->cmd);
		//TODO reset in out
	}
	if (result != 127)
		return (result);
	//TODO return (create_children(mini));
	return (1);
}

void	main_loop(t_minishell *mini)
{
	while (1)
	{
		//TODO interact sig
		mini->line = readline("$-> ");
		//TODO no interact sig
/*
		if (!mini->line)
			break; // Exit on EOF (Ctrl-D)

        // Remove trailing newline
        mini->line[strcspn(mini->line, "\n")] = '\0';

        // Exit command
        if (strcmp(mini->line, "exit") == 0) {
            free(mini->line);
            break;
        }
*/
		if (strlen(mini->line) > 0)
			add_history(mini->line);
		free(mini->line);
		//process input line
		//TODO if(lexer)
		g_status = exec_main(mini);

		//TODO free data (NOW: free(mini->line);)

		printf("You wrote: %s\n", mini->line);
		//printf("You wrote: %d\n", g_status);
	}
}

int	main(int argc, char **argv, char **env)
{
	t_minishell	mini;

	(void)argv;
	if (argc != 1)
	{
		ft_putendl_fd("Error: execute with ./minishell, nothing more", 2);
		exit_mini(NULL, EXIT_FAILURE);
	}
	if (!init_main_struct(&mini, env))
		exit_mini(NULL, EXIT_FAILURE);
	main_loop(&mini);
	return (0);
}
