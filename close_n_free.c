/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   close_n_free.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amdemuyn <amdemuyn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 17:56:56 by amdemuyn          #+#    #+#             */
/*   Updated: 2025/02/04 18:31:21 by amdemuyn         ###   ########.fr       */
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

void	clean_data(t_minishell *mini, bool clear_hist_or_not)
{
	if (mini && mini->line)
	{
		free_star(mini->line);
		mini->line = NULL;
	}
	if (mini && mini->token)
		clean_tkn_nodes(&mini->token, &free_star);
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

void	exit_mini(t_minishell *mini, int exit_code)
{
	if (mini)
	{
		if (mini->command && mini->command->fds)
			close_fds(mini->command, true);
		clean_data(mini, true);
	}
	exit(exit_code);
}