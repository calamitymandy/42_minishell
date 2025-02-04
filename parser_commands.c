/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_commands.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: algalian <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 18:00:24 by algalian          #+#    #+#             */
/*   Updated: 2025/02/04 18:00:26 by algalian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_command	*new_cmd_lst(void)
{
	t_command	*new_node;

	new_node = (t_command *)malloc(sizeof(t_command));
	if (!(new_node))
		return (NULL);
	ft_memset(new_node, 0, sizeof(t_command));
	new_node->cmd = NULL;
	new_node->path = NULL;
	new_node->args = NULL;
	new_node->pipe_output = false;
	new_node->pipe_fd = 0;
	new_node->prev = NULL;
	new_node->next = NULL;
	return (new_node);
}

t_command	*scroll_lstcmd(t_command *aux)
{
	while (aux->next != NULL)
		aux = aux->next;
	return (aux);
}

void	addlst_cmd_container(t_minishell *ms, t_command **cmd_list)
{
	t_command	*new_node;
	t_command	*aux;

	new_node = new_cmd_lst();
	if (!new_node)
		exit_msg(ms, ERR_ALLOC, EXIT_FAILURE);
	aux = *cmd_list;
	if (!aux)
	{
		*cmd_list = new_node;
		return ;
	}
	if (new_node)
	{
		scroll_lstcmd(aux);
		aux->next = new_node;
		new_node->prev = aux;
	}
}
