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

void	ms_io_free(t_fds *io)
{
	if (!io)
		return ;
	ms_reset_io(io);
	if (io->del_heredoc)
	{
		unlink(io->infile);
		ms_ptr_free(io->del_heredoc);
	}
	if (io->infile)
		ms_ptr_free(io->infile);
	if (io->outfile)
		ms_ptr_free(io->outfile);
	if (io)
		ms_ptr_free(io);
}

void	ms_del_one_node_cmd(t_command *lst, void (*del)(void *))
{
	if (lst->command)
		(*del)(lst->command);
	if (lst->args)
		ms_ptr_free_arr(lst->args);
	if (lst->pipe_fd)
		(*del)(lst->pipe_fd);
	if (lst->fds)
		ms_io_free(lst->fds);
	(*del)(lst);
}

void	ms_del_all_nodes_cmd(t_command **lst, void (*del)(void *))
{
	t_command	*temp;

	temp = NULL;
	while (*lst != NULL)
	{
		temp = (*lst)->next;
		ms_del_one_node_cmd(*lst, del);
		*lst = temp;
	}
}


void	ms_data_free(t_minishell *ms, bool clearhistory)
{
	if (ms && ms->line)
	{
		ms_ptr_free(ms->line);
		ms->line = NULL;
	}
	if (ms && ms->token)
		ms_del_all_nodes_tkn(&ms->token, &ms_ptr_free);
	if (ms && ms->cmd)
		ms_del_all_nodes_cmd(&ms->cmd, &ms_ptr_free);
	if (clearhistory == true)
	{
		if (ms && ms->pwd)
			ms_ptr_free(ms->pwd);
		if (ms && ms->old_pwd)
			ms_ptr_free(ms->old_pwd);
		if (ms && ms->env)
			ms_ptr_free_arr(ms->env);
		clear_history();
	}
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