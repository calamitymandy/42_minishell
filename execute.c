/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amdemuyn <amdemuyn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 18:44:33 by amdemuyn          #+#    #+#             */
/*   Updated: 2025/02/04 19:25:52 by amdemuyn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	prep_the_cmd(t_minishell *mini)
{
	if (!mini || !mini->command || !mini->command->cmd
		|| (mini->command->cmd[0] == '\0' && mini->token->has_quotes == false))
		return (EXIT_SUCCESS);
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
