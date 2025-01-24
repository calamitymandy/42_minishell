/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amdemuyn <amdemuyn@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 20:24:50 by amdemuyn          #+#    #+#             */
/*   Updated: 2025/01/23 20:27:55 by amdemuyn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*compile with gcc main.c -lreadline*/

int	g_status;

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
		cmd_res = exec_export_builtin(mini, command->args);
	else if (ft_strncmp(command->cmd, "unset", 6) == 0)
		cmd_res = exec_unset_builtin(mini, command->args);
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

bool	create_env(t_minishell *ms)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		free(pwd);
		return (false);
	}
	ms->env = malloc(sizeof(char *) * 4);
	if (!ms->env)
		return (false);
	ms->env[0] = ft_strjoin("PWD=", pwd);
	ms->env[1] = ft_strdup("SHLVL=1");
	ms->env[2] = ft_strdup("_=/usr/bin/env");
	if (!ms->env[0] || !ms->env[1] || !ms->env[2])
		return (false);
	ms->env[3] = NULL;
	free(pwd);
	return (true);
}

char	*search_env(t_minishell *ms, char *env_key)
{
	int		i;
	char	**key_value;

	i = 0;
	while (ms->env[i])
	{
		key_value = ft_split(ms->env[i], '=');
		if (ft_strcmp(key_value[0], env_key) == 0)
		{
			free_array(key_value);
			break ;
		}
		else
			i++;
		free_array(key_value);
	}
	if (!ms->env[i])
		return (NULL);
	return (ms->env[i]);
}

char	**arr_append(char **arr, char *line)
{
	int		i;
	char	**new_arr;

	i = 0;
	while (arr[i])
		i++;
	new_arr = malloc(sizeof(char *) * (i + 2));
	if (!new_arr)
		return (NULL);
	i = 0;
	while (arr[i])
	{
		arr[i] = ft_strdup(arr[i]);
		i++;
	}
	new_arr[i++] = ft_strdup(line);
	new_arr[i] = NULL;
	free_array(arr);
	return (new_arr);
}

void	modify_or_add_env(t_minishell *ms, char *line)
{
	int		i;
	char	**split_line;
	char	**split_env;

	split_line = ft_split(line, '=');
	i = 0;
	while (ms->env[i])
	{
		split_env = ft_split(ms->env[i], '=');
		if (strcmp(split_line[0], split_env[0]) == 0)
		{
			free_array(split_env);
			free_array(split_line);
			free(ms->env[i]);
			ms->env[i] = ft_strdup(line);
			return ;
		}
		i++;
		free_array(split_env);
	}
	free_array(split_line);
	ms->env[i] = ft_strdup(line);
}


void	modify_mslvl(t_minishell *ms)
{
	char	*shlvl;
	char	*new_shlvl;
	int		value;
	char	**split;

	shlvl = search_env(ms, "SHLVL");
	if (!shlvl)
	{
		ms->env = arr_append(ms->env, "SHLVL=1");
		return ;
	}
	split = ft_split(shlvl, '=');
	value = ft_atoi(split[1]);
	value++;
	shlvl = ft_itoa(value);
	new_shlvl = ft_strjoin("SHLVL=", shlvl);
	modify_or_add_env(ms, new_shlvl);
	free(shlvl);
	free(new_shlvl);
	free_array(split);
}

void	refresh_pwd_env(t_minishell *ms)
{
	char	*pwd;
	char	*str;

	pwd = search_env(ms, "PWD");
	if (!pwd)
	{
		str = getcwd(NULL, 0);
		ms->env = arr_append(ms->env, str);
		free(str);
		return ;
	}
}

bool copy_environment(t_minishell *ms, char **envp)
{

	int		i;

	if (*envp)
		ms->env = ft_arrdup(envp);
	modify_mslvl(ms);
	refresh_pwd_env(ms);
	if (!ms->env)
		return (false);
	i = 0;
	while (ms->env[i])
	{
		if (!ms->env[i])
			return (false);
		i++;
	}
	return (true);
}


bool set_env_var(t_minishell *ms, char **env)
{
	if (!*env)
	{
		if (!create_env(ms))
			return (false);
	}
	else
	{
		if (!copy_environment(ms, env))
			return (false);
	}
	return (true);
}

bool	init_main_struct(t_minishell *mini, char **env)
{

	if (!set_env_var(mini, env))
	{
		error_msg("Error", NULL, \
		"Failed to initialize environment variables", errno);
		return (false);
	}
	if (!set_pwd_n_oldpwd(mini))
	{
		error_msg("Error", NULL, \
		"Failed to initialize working directories", errno);
		return (false);
	}
	
	mini->line = NULL;
	mini->ctrlcheredoc = false;
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
