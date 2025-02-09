/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_lexer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amdemuyn <amdemuyn@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 20:59:51 by algalian          #+#    #+#             */
/*   Updated: 2025/02/09 12:42:44 by amdemuyn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
