//ADD HEADER

#include "minishell.h"

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
		return (error_msg(command->cmd, NULL, " Is a directory", 126));
	if (access(command->cmd, F_OK | X_OK) != 0)
		return (error_msg(command->cmd, NULL, strerror(errno), 126));
	if (execve(command->cmd, command->args, mini->env) == -1)
		return (error_msg("execve", NULL, strerror(errno), errno));
	return (EXIT_FAILURE);
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