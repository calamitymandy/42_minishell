/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   close_n_free.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amdemuyn <amdemuyn@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 17:56:56 by amdemuyn          #+#    #+#             */
/*   Updated: 2025/02/03 22:35:32 by amdemuyn         ###   ########.fr       */
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
			free_two_stars((*lst)->args);
		if ((*lst)->pipe_fd)
			(*del)((*lst)->pipe_fd);
		if ((*lst)->fds)
			free_in_and_out_fds((*lst)->fds);
		(*del)(*lst);
		*lst = temp;
	}
	*lst = NULL;
}


void	del_one_node_tkn(t_token *lst, void (*del)(void *))
{
	if (del && lst && lst->content)
	{
		(*del)(lst->content);
		lst->content = NULL;
	}
	if (del && lst && lst->cc)
	{
		(*del)(lst->cc);
		lst->cc = NULL;
	}
	if (lst->prev)
		lst->prev->next = lst->next;
	if (lst->next)
		lst->next->prev = lst->prev;
	free_star(lst);
}

void	clean_tkn_nodes(t_token **lst, void (*del)(void *))
{
	t_token	*tmp;

	tmp = NULL;
	while (*lst != NULL)
	{
		tmp = (*lst)->next;
		del_one_node_tkn(*lst, del);
		*lst = tmp;
	}
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
		free_in_and_out_fds(lst->fds);
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


void	clean_data(t_minishell *ms, bool clearhistory)
{
	if (ms && ms->line)
	{
		free_star(ms->line);
		ms->line = NULL;
	}
	if (ms && ms->token)
		clean_tkn_nodes(&ms->token, &free_star);
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