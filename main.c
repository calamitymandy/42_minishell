/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amdemuyn <amdemuyn@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 20:24:50 by amdemuyn          #+#    #+#             */
/*   Updated: 2024/11/28 20:26:26 by amdemuyn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void clean_data(t_minishell *mini, bool clear_hist_or_not);

/*compile with gcc main.c -lreadline*/

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

int	g_status;

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
	free_star(output);
	return (err_nb);
}

char	*get_env_value(char **env, char *key)
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
			free_star(temp);
			return (ft_strchr(env[i], '=') + 1);
		}
		i++;
	}
	free_star(temp);
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
			free_star(temp);
			return (i);
		}
		i++;
	}
	free_star(temp);
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
		free_star(mini->env[i]);
		i++;
	}
	free(mini->env);
	return (new_env);
}

/* Updates an existing environment variable (like PWD or OLDPWD) or adds a 
 * new one if it doesn't exist:
 * 1- Search for the index of environment variable
 * 2- joins the "=" with the value string to form the value part of the 
 * 	environment variable (e.g., =new_path). This will later be used to 
 * 	construct the full environment variable string.
 * 3- IF the environment variable was found (i != -1), it frees mini->env[i]
 * 	Then creates a new string joining the variable name (pwd_or_old) 
 * 	with the temp string (=value) and stores the result in mini->env[i].
 * 4- ELSE (i == -1), it calculates the number of environment variables 
 * 	using nb_env_variables(mini->env). Reallocates memory to accommodate 
 * 	the new environment variable.
 * 	& Adds the new environment variable to the end of the mini->env array
 * 	by joining the variable name (pwd_or_old) with temp and storing the result.
*/
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
		free_star(mini->env[i]);
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
	free_star(temp);
	return (true);
}
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
		mini->old_pwd =ft_strdup(mini->pwd);
	}
	if (mini->pwd)
	{
		free_star(mini->pwd);
		mini->pwd =ft_strdup(buf_of_work_dir_path);
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
			return (error_msg("cd", NULL, "Error: OLDPWD not set", EXIT_FAILURE));
		ft_putendl_fd(path, 2);
		return (!cd(mini, path));
	}
	return (!cd(mini, args[1]));
}

/*Mix of 2 with quotes checker*/
bool	is_var_no_quotes(t_token *tkns, int index)
{
	t_token	*lst;
	int i;

	i = 0;
	lst = tkns;
	while (lst)
	{
		if (lst->index == index && lst->type == VAR)
		{
			while (lst->token_type[i])
			{
				if (lst->token_type[i] == '"' || lst->token_type[i] == '\'')
					return (false);
				i++;
			}
			return (true);
		}
		lst = lst->next;
	}
	return (false);
}
/* Mix of 2 with count_no_white_space
*/
char	*remove_extra_spaces(const char *str)
{
	char	*new_string;
	int		i;
	int		j;

	i = 0;
	j = 0;
	new_string = malloc((ft_strlen(str) + 1) * sizeof(char));
	if (!new_string)
		return (NULL);
	i = 0;
	while (str[i])
	{
		if (!ft_isblank(str[i]) || (i > 0 && !ft_isblank(str[i - 1])))
			new_string[j++] = str[i];
		i++;
	}
	while (j > 0 && ft_isblank(new_string[j - 1]))
		j--;
	new_string[j] = '\0';
	return (new_string);
}

void	print_echo(char **args, bool minus_n_flag, int i, t_minishell *mini)
{
	char	*clean_arg;

	if (!args[i])
	{
		if (!minus_n_flag)
			ft_putchar_fd('\n', STDOUT_FILENO);
		return ;
	}
	while (args[i])
	{
		if (is_var_no_quotes(mini->token, i))
		{
			clean_arg = remove_extra_spaces(args[i]);
			ft_putstr_fd(clean_arg, STDOUT_FILENO);
			free(clean_arg);
		}
		else
			ft_putstr_fd(args[i], STDOUT_FILENO);
		if (args[i +1])
			ft_putchar_fd(' ', STDOUT_FILENO);
		else if (!args[i + 1] && !minus_n_flag)
			ft_putchar_fd('\n', STDOUT_FILENO);
		i++;
	}
}

/* Handle the special -n flag, which suppress the newline at the end of the output.
 * Ensures that a string like -n, -nn, -nnn, etc., is valid but not something 
 * like -nX or -nx
 * Mix of 2 with n_flag
*/
int	exec_echo(t_minishell *mini, char **args)
{
	int		i;
	int		j;
	bool	minus_n_flag;

	minus_n_flag = false;
	i = 1;
	while (args[i])
	{
		j = 0;
		if (args[i][j] != '-' || (args[i][j] == '-' && args[i][j + 1] != 'n'))
			break ;
		j++;
		while (args[i][j] && args[i][j] == 'n')
			j++;
		if (args[i][j] != '\0')
			break ;
		minus_n_flag = true;
		i++;
	}
	print_echo(args, minus_n_flag, i, mini);
	return (EXIT_SUCCESS);
}

/* Implements the env built-in command, which prints the current 
 * environment variables.
 * args: An array of strings representing the arguments passed to the 
 * env command.
 * 1- Check for too many arguments. The env command does not accept 
 * any arguments. If args[1] exists (more than one argument was passed)
 * the function returns an error message and 2, a standard error code 
 * for invalid usage in shell commands.
 * 2- Check if the environment exists, if not it returns EXIT_FAILURE.
 * 3- A loop iterates through the mini->env array (which contains the
 *  environment variables).For each environment variable, prints the 
 * variable with a newline to the standard output (STDOUT_FILENO).
 */

int	exec_env_builtin(t_minishell *mini, char **args)
{
	int	i;
	
	if (args && args[1])
		return (error_msg("env", NULL, "Error: too many arguments", 2));
	i = 0;
	if (!mini->env)
		return (EXIT_FAILURE);
	while (mini->env[i])
		ft_putendl_fd(mini->env[i++], STDOUT_FILENO);
	return (EXIT_SUCCESS);
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

int	get_exit_code(char *arg, bool *error)
{
	unsigned long long	i;
	
	if (!arg)
		return (g_status);
	i = 0;
	while (ft_isblank(arg[i]))
		i++;
	if (arg[i] == '\0')
		*error = true;
	if (arg[i] == '-' || arg[i] == '+')
		i++;
	if (!ft_isdigit(arg[i]))
		*error = true;
	while (arg[i])
	{
		if (!ft_isdigit(arg[i]) && !ft_isblank(arg[i]))
			*error = true;
		i++;
	}
	i = ft_atoi_long(arg, error);
	return (i % 256);
}

int	exec_exit_builtin(t_minishell *mini, char **args)
{
	int		exit_code;
	bool	error;

	error = false;
	if (!args || !args[1])
		exit_code = g_status;
	else
	{
		exit_code = get_exit_code(args[1], &error);
		if (error)
			exit_code = error_msg("exit", args[1],
					"numeric argument required", 255);
		else if (args[2])
			return (error_msg("exit", NULL, "too many arguments", 1));
	}
	exit_mini(mini, exit_code);
	return (2);
}

bool	valid_env_key(char *key)
{
	int	i;
	
	i = 0;
	if (key == NULL || key[0] == '\0')
		return (false);
	if (ft_isalpha(key[i]) == 0 && key[i] != '_')
		return (false);
	i++;
	while (key[i] && key[i] != '=')
	{
		if (ft_isalnum(key[i]) == 0 && key[i] != '_')
			return (false);
		i++;
	}
	return (true);
}

static char **key_value_arr(char *key)
{
	char	**arr;
	char	*equal;

	equal = ft_strchr(key, '=');
	if (!equal)
		return (NULL);
	arr = malloc(sizeof(char *) * 3);
	arr[0] = ft_substr(key, 0, equal - key);
	arr[1] = ft_substr(equal, 1, ft_strlen(equal));
	arr[2] = NULL;
	return (arr);
}

int	export_builtin(t_minishell *mini)
{
	char 	*env_quotes;
	int		i;
	int		n;

	i = 0;
	if (!mini->env)
		return (EXIT_FAILURE);
	n = nb_env_variables(mini->env);
	qsort_env(mini->env, n); //TODO
	i = 0;
	while (i < n)
	{
		env_quotes = add_env_quotes(mini->env[i]); //TODO
		printf("declare -x %s\n", env_quotes);
		i++;
	}
	return (EXIT_SUCCESS);
}


int	exec_export_builtin(t_minishell *mini, char **args)
{
	int		i;
	char	**arr;
	int		res;

	res = EXIT_SUCCESS;
	i = 1;
	if (!args[i])
		return (export_builtin(mini)); //IN PROGRESS
	while (args[i])
	{
		if (!valid_env_key(args[i]))
		{
			error_msg("export", args[i], "not a valid identifier", \
			false);
			res = EXIT_FAILURE;
		}
		else if (ft_strchr(args[i], '=') != NULL)
		{
			arr = key_value_arr(args[i]);
			add_or_update_env_var(mini, arr[0], arr[1]);
			free_two_stars(arr);
		}
		i++;
	}
	return (res);
}

int	exec_builtin(t_minishell *mini, t_command *command)
{
	int	cmd_res;

	cmd_res = 127;
	if (ft_strncmp(command->cmd, "cd", 3) == 0)
		cmd_res = exec_cd(mini, command->args);
	else if (ft_strncmp(command->cmd, "echo", 5) == 0)
		cmd_res = exec_echo(mini, command->args);
	else if (ft_strncmp(command->cmd, "env", 4) == 0)
		cmd_res = exec_env_builtin(mini, command->args);
	else if (ft_strncmp(command->cmd, "pwd", 4) == 0)
		cmd_res = exec_pwd_builtin(mini, command->args);
	else if (ft_strncmp(command->cmd, "exit", 5) == 0)
		cmd_res = exec_exit_builtin(mini, command->args);
	else if (ft_strncmp(command->cmd, "export", 7) == 0)
		cmd_res = exec_export_builtin(mini, command->args); // IN PROGRESS
	// TODO AMANDINE:
	// exec_unset_builtin
	return (cmd_res);
}

/**
 * Checks if input and output file descriptors are valid in a given structure.
 * If the file descriptors are valid, it returns true. 
 * Otherwise, it returns false.
 */
bool	check_in_and_out(t_fds	*in_n_out)
{
	if (!in_n_out || (!in_n_out->infile && !in_n_out->outfile))
		return (true);
	if ((in_n_out->infile && in_n_out->fd_infile == -1)
		|| (in_n_out->outfile && in_n_out->fd_outfile == -1))
		return (false);
	return (true);
}

/**
 * SLIGHT MODIF
 * configures the standard input and output file descriptors for a process.
 * It makes duplicates of the current standard input and output, then 
 * redirects them to specified files if provided. 
 * 
 * 1. Checks if the in_n_out structure is not NULL.
 * 2. Duplicates the current standard input and output file descriptors and
 * stores them in the stdin_ori and stdout_ori fields of the in_n_out structure.
 * 3. Redirects the standard input to fd_infile if it is provided.
 * 4. Redirects the standard output to fd_outfile if it is provided.
 * 5. Handles any errors during these operations and logs them using the 
 * error_msg function.
 * 6. Returns true if all operations succeed and false if any operation fails.
 */
bool	config_in_and_out(t_fds	*in_n_out)
{
	int	result;

	result = true;
	if (!in_n_out)
		return (result);
	in_n_out->stdin_ori = dup(STDIN_FILENO);
	in_n_out->stdout_ori = dup(STDOUT_FILENO);
	if (in_n_out->stdin_ori == -1)
		result = error_msg("dup", "stdin_ori", strerror(errno), false);
	if (in_n_out->stdout_ori == -1)
		result = error_msg("dup", "stdout_ori", strerror(errno), false);
	if (in_n_out->fd_infile != -1
		&& (dup2(in_n_out->fd_infile, STDIN_FILENO) == -1))
		result = error_msg("dup2", in_n_out->infile, strerror(errno), false);
	if (in_n_out->fd_outfile != -1
		&& (dup2(in_n_out->fd_outfile, STDIN_FILENO) == -1))
		result = error_msg("dup2", in_n_out->outfile, strerror(errno), false);
	return (result);
}

/**
 * Restores the original stdin & stdout fds if they were saved:
 * Uses dup2 to restore the original stdin & stdout.
 * Closes the temporary file descriptor.
 * Resets stdin_ori to -1.
 * returns a boolean value indicating whether the fds have been successfully reset.

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
 */

/**
 * close_fds` closes file descriptors and resets them if specified.
 * If `close_or_not` is true, the function will call `reset_fds_in_and_out` 
 * to reset the file descriptors in the `command` structure.
 * 
 * MIXED WITH CLOSE_PIPE_FD (while) to closes pipe file descriptors for all 
 * commands except NULL.
 */
void	close_fds(t_command *command, bool close_or_not)
{
	if (command->fds)
	{
		if (command->fds->fd_infile != -1)
				close (command->fds->fd_infile);
		if (command->fds->fd_outfile != -1)
				close (command->fds->fd_outfile);
		if (close_or_not)
			reset_fds_in_and_out(command->fds);
	}
	while (command)
	{
		if (command != NULL && command->pipe_fd)
		{
			close(command->pipe_fd[0]);
			close(command->pipe_fd[1]);
		}
		command = command->next;
	}
}

bool	init_main_struct(t_minishell *mini, char **env)
{
	//TODO set_env && set_pwd_and oldpwd
	mini->env = env; //QUIT when TODO is done
	mini->line = NULL;
	//mini->ctrlc_heredoc = false; TODO
	mini->token = NULL;
	mini->command = NULL;
	mini->pid = -1;
	g_status = 0;
	return (true);
}

/**
 * The function creates pipes for inter-process communication in a shell program
 * whith: pipe(fd).
 * It returns `true` if the pipes are successfully created for the commands in 
 * the struct, or `false` if there is an error during pipe creation 
 * or memory allocation.
 */
bool	create_pipes(t_minishell *mini)
{
	int			*fd;
	t_command	*temp_cmd;

	temp_cmd = mini->command;
	while (temp_cmd)
	{
		if (temp_cmd->pipe_output
			|| (temp_cmd->prev && temp_cmd->prev->pipe_output))
		{
			fd = malloc (sizeof * fd * 2);
			if (!fd || pipe(fd) != 0)
			{
				clean_data(mini, false);
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
	//TODO init mini->command->cmd & mini->token->has_quotes somewhere
	if (!mini || !mini->command || !mini->command->cmd
		|| (mini->command->cmd[0] == '\0' && mini->token->has_quotes == false))
		return (EXIT_SUCCESS);
	//printf("here it does not print");
	if (mini->command && !mini->command->cmd)
	{
		if (mini->command->fds && !check_in_and_out(mini->command->fds))
			return (EXIT_FAILURE);
		return (EXIT_SUCCESS);
	}
	if (!create_pipes(mini))
		return (EXIT_FAILURE);
	return (127);
}
 /*
 * MIXED WITH CLOSE_PIPE_FD (while) to closes pipe file descriptors for all 
 * commands except current_cmd.
 */

bool	set_n_close_pipes_fds(t_command *cmd_list, t_command *current_cmd)
{
	if (!current_cmd)
		return(false);
	if (current_cmd->prev && current_cmd->prev->pipe_output)
		dup2(current_cmd->prev->pipe_fd[0], STDIN_FILENO);
	if (current_cmd->pipe_output)
		dup2(current_cmd->pipe_fd[1], STDOUT_FILENO);
	while (cmd_list)
	{
		if (cmd_list != current_cmd && cmd_list->pipe_fd)
		{
			close(cmd_list->pipe_fd[0]);
			close(cmd_list->pipe_fd[1]);
		}
		cmd_list = cmd_list->next;
	}
	return (true);
}

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

int	find_env_index_of_key(char **env, char *key)
{
	int		i;
	char *aux;

	aux = ft_strjoin(key, "=");
	if (!aux)
		return (-1);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(aux, env[i], ft_strlen(aux)) == 0)
		{
			free_star(aux);
			return (i);
		}
		i++;
	}
	free_star(aux);
	return (-1);
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
			error_msg("malloc", NULL, "an unexpected error occured", EXIT_FAILURE);
			return (NULL);
		}
		if (access(cmd_path, F_OK | X_OK) == 0)
			return (cmd_path);
		free_star(cmd_path);
		i++;
	}
	return (NULL);
}

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

/**
 * The function executes a system binary cmd in a minishell environment.
 * 
 * A system binary cmd is an executable program that is stored in binary 
 * form on the system, typically located in standard directories like 
 * /bin, /usr/bin, /sbin. These commands are essential for interacting with
 * the operating system and performing various tasks.
 * 
 * Examples of basic Commands: ls, cat, cp(Copies files or directories), 
 * mv(Moves or renames files or directories), rm.
 */
int	exec_sys_binary(t_minishell *mini, t_command *command)
{
	if (!command->cmd || command->cmd[0] == '\0')
		return (127);
	if (is_directory(command->cmd))
		return (127);
	command->path = get_path_cmd(mini, command->cmd);
	if (!command->path)
		return (127);
	if (execve(command->path, command->args, mini->env) == -1)
		error_msg("execve", NULL, strerror(errno), errno);
	return (EXIT_FAILURE);
}
/*Mix of exec_local_binary & check_cmd_validity
 * Function designed to execute a command that is specified as a local binary. 
 * A local binary command refers to a program or script that is located in a 
 * specific directory on the file system and is executed by specifying either a 
 * relative or absolute path to the file. 
 * ex: ./hello.sh or /home/user/bin/my_program
 * 
 * This function is used to handle the execution of commands that are
 * specified with a relative or absolute path (i.e., commands with / in them).
 * It checks whether the command exists, is not a directory, and is executable.
 * If all checks pass, it attempts to execute the command using execve. 
 * If execution fails or any conditions aren't met, appropriate error messages 
 * are returned with relevant exit codes (127 for command not found, 
 * 126 for permission errors, and errno for other errors).
*/
int	exec_local_binary(t_minishell *mini, t_command *command)
{
	if (ft_strchr(command->cmd, '/') == NULL
		&& find_env_index_of_key(mini->env, "PATH") != -1)
		return (error_msg(command->cmd, NULL, "command not found", 127));
	if (access(command->cmd, F_OK) != 0)
		return (error_msg(command->cmd, NULL, strerror(errno), 127));
	if (is_directory(command->cmd))
		return (error_msg(command->cmd, NULL, "is a directory", 126));
	if (access(command->cmd, F_OK | X_OK) != 0)
		return (error_msg(command->cmd, NULL, strerror(errno), 126));
	if (execve(command->cmd, command->args, mini->env) == -1)
		return (error_msg("execve", NULL, strerror(errno), errno));
	return (EXIT_FAILURE);
}

int	exec_cmd(t_minishell *mini, t_command *command)
{
	int	res;

	if (!check_in_and_out(command->fds))
		exit_mini(mini, EXIT_FAILURE);
	set_n_close_pipes_fds(mini->command, command); //Mix of two
	config_in_and_out(command->fds);
	close_fds(mini->command, false);
	if (ft_strchr(command->cmd, '/') == NULL)
	{
		res = exec_builtin(mini, command);
		if (res != 127)
			exit_mini(mini, res);
		res = exec_sys_binary(mini, command);
		if (res != 127)
			exit_mini(mini, res);
	}
	res = exec_local_binary(mini, command);
	exit_mini(mini, res);
	return (res);
}
/* waits for all child processes to finish and gathers the exit status
* of the shell's main process (identified by mini->pid):
* 1- Closes file descriptors.
* 2- Waits for child processes using waitpid.
* 3- Stores the status of the main child process.
*	After all child processes have finished and the loop ends,
*	the function evaluates how the main child process
*	(stored in save_status) terminated:
* WIFSIGNALED(save_status) checks if child was terminated by 
* a signal (SIGKILL, SIGINT).
* If the child was terminated by a signal, the exit status is calculated
* as 128 + WTERMSIG(save_status), where WTERMSIG extracts the signal number.
* WIFEXITED(save_status) checks if the child terminated normally via a 
* call to exit() or by returning from main(). If so, the exit status is 
* set to the exit code of the child, extracted using WEXITSTATUS(save_status).
* else : If the child didn’t terminate in either of the above two conditions,
* it returns save_status as-is, which might handle other rare termination cases.
* 4- Checks if the child was terminated by a signal or exited normally 
* and returns the appropriate status.
* 	The function is crucial for correctly handling the termination of child
* 	processes and communicating their exit status to the parent shell 
* 	process, which affects how the shell responds after a command is executed.
*/
int	child_status(t_minishell *mini)
{
	pid_t	pid_from_waitpid;
	int		status;
	int		save_status;

	close_fds(mini->command, false);
	save_status = 0;
	pid_from_waitpid = 0;
	while (pid_from_waitpid != -1 || errno != ECHILD)
	{
		pid_from_waitpid = waitpid(-1, &status, 0);
		if (pid_from_waitpid == mini->pid)
			save_status = status;
		continue ;
	}
	if (WIFSIGNALED(save_status))
		status = 128 + WTERMSIG(save_status);
	else if (WIFEXITED(save_status))
		status = WEXITSTATUS(save_status);
	else
		status = save_status;
	return (status);	
}

int	create_children(t_minishell *mini)
{
	t_command	*command;

	command = mini->command;
	while (mini->pid != 0 && command)
	{
		mini->pid = fork();
		if (mini->pid == -1)
			return (error_msg("fork", NULL, strerror(errno), EXIT_FAILURE));
		else if (mini->pid == 0)
			exec_cmd(mini, command);
		command = command->next;
	}
	return (child_status(mini));
}

/* This function executes the processed command and returns a status code 
 * that updates g_status. 
 * 
 * The 2nd if block checks if the command should be executed directly, 
 * without piping or chaining to other commands:
 * 1 - Ensures that command is not sending output through a pipe.
 * 2 - Ensures that command is not part of a pipeline or a sequence of commands.
 * 3 - checks if fds input & output are valid, meaning no pb with redirections.
 * 
 * config_in_and_out and reset_fds_in_and_out are necessary to handle input/output 
 * redirections (ex: echo "Hello" > output.txt) they save & restore the 
 * original fds (STDIN & STDOUT) when needed, and so ensure each command starts
 * with a clean input/output state.
 * 
 * 127 = command unknown
 */
int	exec_main(t_minishell *mini)
{
	int	result;

	result = prep_the_cmd(mini);
	if (result != 127)
		return (result);
	if (!mini->command->pipe_output && !mini->command->prev
		&& check_in_and_out(mini->command->fds))
	{
		config_in_and_out(mini->command->fds);
		result = exec_builtin(mini, mini->command);
		reset_fds_in_and_out(mini->command->fds);
	}
	if (result != 127)
		return (result);
	return (create_children(mini));
}

/* mix of 2: delete all token node && delete one token node
 * ADDED: delete_all, flag to delete one or all nodes in the list, 
 * depending on the flag being on 1 or 0.
*/
void	clean_token_nodes(t_token **lst, void (*del)(void *), int delete_all)
{
	t_token	*temp;

	while (*lst != NULL)
	{
		temp = (*lst)->next;
		if (del && (*lst))
		{
			if ((*lst)->content)
				(*del)((*lst)->content);
			if ((*lst)->token_type)
				(*del)((*lst)->token_type);
		}
		if ((*lst)->prev)
			(*lst)->prev->next = (*lst)->next;
		if ((*lst)->next)
			(*lst)->next->prev = (*lst)->prev;
		free_star(*lst);
		if (!delete_all)
			break ;
		*lst = temp;
	}
	if (delete_all)
		*lst = NULL;
}
/*
void	free_in_and_out_fds(t_fds *in_and_out)
{
	if (!in_and_out)
		return ;
	reset_fds_in_and_out(in_and_out);
	if (in_and_out->del_heredoc)
	{
		unlink(in_and_out->del_heredoc);
		free_star(in_and_out->del_heredoc);
	}
	if (in_and_out->infile)
		free_star(in_and_out->infile);
	if (in_and_out->outfile)
		free_star(in_and_out->outfile);
	if (in_and_out)
		free_star(in_and_out);
}
*/

/*mix of 2 clean all nodes and clean one node
void	clean_cmd_nodes(t_command **lst, void (*del)(void *))
{
	t_command	*temp;
	
	while (*lst != NULL)
	{
		temp = (*lst)->next;
		if ((*lst)->cmd)
			(*del)((*lst)->cmd);
		if ((*lst)->args)
			(*del)((*lst)->args);
		if ((*lst)->pipe_fd)
			(*del)((*lst)->pipe_fd);
		if ((*lst)->fds)
			free_in_and_out_fds((*lst)->fds);
		(*del)(*lst);
		*lst = temp;
	}
	*lst = NULL;
}
*/

/*void	clean_data(t_minishell *mini, bool clear_hist_or_not)
{
	if (mini && mini->line)
	{
		free_star(mini->line);
		mini->line = NULL;
	}
	if (mini && mini->token)
		clean_token_nodes(&mini->token, &free_star, 1);
	if (mini && mini->command)
		clean_cmd_nodes(&mini->command, &free_star);
	if (clear_hist_or_not == true)
	{
		if (mini && mini->pwd)
			free_star(mini->pwd);
		if (mini && mini->old_pwd)
			free_star(mini->old_pwd);
		if (mini && mini->env)
			free_two_stars(mini->env);
		clear_history();
	}
}
*/

void	main_loop(t_minishell *mini)
{
	while (1)
	{
		listening_interact_sig();
		mini->line = readline("$-> ");
		listening_no_interact_sig();
		if(lexer_main(mini))
			g_status = exec_main(mini);
		clean_data(mini, false);
		//printf("You wrote: %s\n", mini->line);
		//printf("g_status: %d\n", g_status);
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
