/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   close_n_free.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amdemuyn <amdemuyn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 17:56:56 by amdemuyn          #+#    #+#             */
/*   Updated: 2024/09/03 18:44:33 by amdemuyn         ###   ########.fr       */
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

bool	ms_reset_io(t_fds *io)
{
	int	res;

	res = true;
	if (!io)
		return (res);
	if (io->stdin_ori != -1)
	{
		if (dup2(io->stdin_ori, STDIN_FILENO) == -1)
			res = false;
		close(io->stdin_ori);
		io->stdin_ori = -1;
	}
	if (io->stdout_ori != -1)
	{
		if (dup2(io->stdout_ori, STDOUT_FILENO) == -1)
			res = false;
		close(io->stdout_ori);
		io->stdout_ori = -1;
	}
	return (res);
}

void	ms_close_un_pipes_fd(t_command *cmds, t_command *omit_cmd)
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



void	ms_close_fds(t_command *cmds, bool close_backups)
{
	if (cmds->fds)
	{
		if (cmds->fds->fd_infile != -1)
			close(cmds->fds->fd_infile);
		if (cmds->fds->fd_outfile != -1)
			close(cmds->fds->fd_outfile);
		if (close_backups)
			ms_reset_io(cmds->fds);
	}
	ms_close_un_pipes_fd(cmds, NULL);
}