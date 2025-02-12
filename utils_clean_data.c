/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_clean_data.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amdemuyn <amdemuyn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 20:59:51 by algalian          #+#    #+#             */
/*   Updated: 2025/02/12 17:17:40 by amdemuyn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

bool	is_line_empty(char *line)
{
	int	i;

	i = -1;
	while (line[++i])
		if (!ft_isblank(line[++i]))
			return (false);
	return (true);
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
