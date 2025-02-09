/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_parser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amdemuyn <amdemuyn@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 17:56:56 by amdemuyn          #+#    #+#             */
/*   Updated: 2025/02/09 12:30:25 by amdemuyn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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