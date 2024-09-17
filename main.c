/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amdemuyn <amdemuyn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 20:24:50 by amdemuyn          #+#    #+#             */
/*   Updated: 2024/09/17 17:52:35 by amdemuyn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
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
	add_or_update_env_var(mini, "OLDPWD", get_env_value(mini->env, "PWD"));
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

/*!!!!!!! check is_blank !!!!!!!!!!*/
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

int	exec_builtin(t_minishell *mini, t_command *cmd)
{
	int	cmd_res;

	cmd_res = 127;
	if (ft_strncmp(cmd->cmd, "cd", 3) == 0)
		cmd_res = exec_cd(mini, cmd->args);
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
 */
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

/**
 * `close_pipe_fd` closes pipe file descriptors for all commands except 
 * for a specified command.
 * This function iterates through a linked list of `t_command` structures 
 * and closes the file descriptors stored in the `pipe_fd` array for each 
 * command, except for the command pointed to by `skip_cmd`.

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
*/

/**
 * close_fds` closes file descriptors and resets them if specified.
 * If `close_or_not` is true, the function will call `reset_fds_in_and_out` 
 * to reset the file descriptors in the `cmd` structure.
 * 
 * MIXED WITH CLOSE_PIPE_FD (while) to closes pipe file descriptors for all 
 * commands except NULL.
 */
void	close_fds(t_command *cmd, bool close_or_not)
{
	if (cmd->fds)
	{
		if (cmd->fds->fd_infile != -1)
				close (cmd->fds->fd_infile);
		if (cmd->fds->fd_outfile != -1)
				close (cmd->fds->fd_outfile);
		if (close_or_not)
			reset_fds_in_and_out(cmd->fds);
	}
	while (cmd)
	{
		if (cmd != NULL && cmd->pipe_fd)
		{
			close(cmd->pipe_fd[0]);
			close(cmd->pipe_fd[1]);
		}
		cmd = cmd->next;
	}
}

bool	init_main_struct(t_minishell *mini, char **env)
{
	//TODO set_env && set_pwd_and oldpwd
	mini->env = env; //QUIT when TODO is done
	mini->line = NULL;
	//mini->ctrlc_heredoc = false; TODO
	mini->token = NULL;
	mini->cmd = NULL;
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

	temp_cmd = mini->cmd;
	while (temp_cmd)
	{
		if (temp_cmd->pipe_output
			|| (temp_cmd->prev && temp_cmd->prev->pipe_output))
		{
			fd = malloc (sizeof * fd * 2);
			if (!fd || pipe(fd) != 0)
			{
				//free data: ms_data_free(mini, false);
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
	//TODO init mini->cmd->cmd & mini->token->has_quotes somewhere
	if (!mini || !mini->cmd || !mini->cmd->cmd
		|| (mini->cmd->cmd[0] == '\0' && mini->token->has_quotes == false))
		return (EXIT_SUCCESS);
	//printf("here it does not print");
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

int	exec_sys_binary(t_minishell *mini, t_command *cmd)
{
	if (!cmd->cmd || cmd->cmd[0] == '\0')
		return (127);
	if (is_directory(cmd->cmd))
		return (127);
	cmd->path = get_path_cmd(mini, cmd->cmd);
	if (!cmd->path)
		return (127);
	if (execve(cmd->path, cmd->args, mini->env) == -1)
		error_msg("execve", NULL, strerror(errno), errno);
	return (EXIT_FAILURE);
}
/*Mix of exec_local_binary & check_cmd_validity*/
int	exec_local_binary(t_minishell *mini, t_command *cmd)
{
	if (ft_strchr(cmd->cmd, '/') == NULL
		&& find_env_index_of_key(mini->env, "PATH") != -1)
		return (error_msg(cmd->cmd, NULL, "command not found", 127));
	if (access(cmd->cmd, F_OK) != 0)
		return (error_msg(cmd->cmd, NULL, strerror(errno), 127));
	if (is_directory(cmd->cmd))
		return (error_msg(cmd->cmd, NULL, "is a directory", 126));
	if (access(cmd->cmd, F_OK | X_OK) != 0)
		return (error_msg(cmd->cmd, NULL, strerror(errno), 126));
	if (execve(cmd->cmd, cmd->args, mini->env) == -1)
		return (error_msg("execve", NULL, strerror(errno), errno));
	return (EXIT_FAILURE);
}

int	exec_cmd(t_minishell *mini, t_command *cmd)
{
	int	res;

	if (check_in_and_out(cmd->fds))
		exit_mini(mini, EXIT_FAILURE);
	set_n_close_pipes_fds(mini->cmd, cmd); //Mix of two
	config_in_and_out(cmd->fds);
	close_fds(mini->cmd, false);
	if (ft_strchr(cmd->cmd, '/') == NULL)
	{
		res = exec_builtin(mini, cmd);
		if (res != 127)
			exit_mini(mini, res);
		res = exec_sys_binary(mini, cmd);
		if (res != 127)
			exit_mini(mini, res);
	}
	res = exec_local_binary(mini, cmd);
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

	close_fds(mini->cmd, false);
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
	t_command	*cmd;

	cmd = mini->cmd;
	while (mini->pid != 0 && cmd)
	{
		mini->pid = fork();
		if (mini->pid == -1)
			return (error_msg("fork", NULL, strerror(errno), EXIT_FAILURE));
		else if (mini->pid == 0)
			exec_cmd(mini, cmd);
		cmd = cmd->next;
	}
	return (child_status(mini));
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
		reset_fds_in_and_out(mini->cmd->fds);
	}
	if (result != 127)
		return (result);
	return (create_children(mini));
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

void	exit_mini(t_minishell *mini, int exit_code)
{
	if (mini)
	{
		if (mini->cmd && mini->cmd->fds)
			close_fds(mini->cmd, true);
		//free data TODO
	}
	exit(exit_code);
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
