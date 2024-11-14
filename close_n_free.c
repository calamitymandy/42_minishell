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
bool	reset_io(t_fds *io)
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

void	io_free(t_fds *io)
{
	if (!io)
		return ;
	reset_io(io);
	if (io->del_heredoc)
	{
		unlink(io->infile);
		free_star(io->del_heredoc);
	}
	if (io->infile)
		free_star(io->infile);
	if (io->outfile)
		free_star(io->outfile);
	if (io)
		free_star(io);
}

void	del_one_node_cmd(t_command *lst, void (*del)(void *))
{
	if (lst->cmd)
		(*del)(lst->cmd);
	if (lst->args)
		free_two_stars(lst->args);
	if (lst->pipe_fd)
		(*del)(lst->pipe_fd);
	if (lst->fds)
		io_free(lst->fds);
	(*del)(lst);
}

void	del_all_nodes_cmd(t_command **lst, void (*del)(void *))
{
	t_command	*temp;

	temp = NULL;
	while (*lst != NULL)
	{
		temp = (*lst)->next;
		del_one_node_cmd(*lst, del);
		*lst = temp;
	}
}


void	data_free(t_minishell *ms, bool clearhistory)
{
	if (ms && ms->line)
	{
		free_star(ms->line);
		ms->line = NULL;
	}
	if (ms && ms->token)
		del_all_nodes_tkn(&ms->token, &free_star);
	if (ms && ms->command)
		del_all_nodes_cmd(&ms->command, &free_star);
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

