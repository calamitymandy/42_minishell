/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   close_n_free.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amdemuyn <amdemuyn@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 17:56:56 by amdemuyn          #+#    #+#             */
/*   Updated: 2025/01/29 19:44:30 by amdemuyn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void	free_star(void *ptr)
{
	if (ptr != NULL)
	{
		free(ptr);
		ptr = NULL;
	}
}

void	free_two_stars(char **arr)
{
	int	i;

	i = 0;
	if (arr)
	{
		while (arr[i])
		{
			if (arr[i])
			{
				free_star(arr[i]);
				arr[i] = NULL;
			}
			i++;
		}
		free (arr);
		arr = NULL;
	}
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
/* mix of 2 with: ms_del_one_node_cmd*/ 
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


void	clean_data(t_minishell *ms, bool clearhistory)
{
	if (ms && ms->line)
	{
		free_star(ms->line);
		ms->line = NULL;
	}
	if (ms && ms->token)
		del_all_nodes_tkn(&ms->token, &free_star);
	if (ms && ms->command)
		clean_cmd_nodes(&ms->command, &free_star);
	if (clearhistory == true)
	{
		if (ms && ms->pwd)
			free_star(ms->pwd);
		if (ms && ms->old_pwd)
			free_star(ms->old_pwd);
		if (ms && ms->env)
			free_two_stars(ms->env);
		clear_history();
	}
}



void	close_un_pipes_fd(t_command *cmds, t_command *omit_cmd)
{
	while (cmds)
	{
		if (cmds != omit_cmd && cmds->pipe_fd)
		{
			close(cmds->pipe_fd[0]);
			close(cmds->pipe_fd[1]);
		}
		cmds = cmds->next;
	}
}

