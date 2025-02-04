/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_mark.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: algalian <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 19:20:30 by algalian          #+#    #+#             */
/*   Updated: 2025/02/04 19:20:32 by algalian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	mark_variables(t_minishell *ms)
{
	t_token	*aux;
	int		scan;

	aux = ms->token;
	while (aux)
	{
		scan = -1;
		while ((aux)->content[++scan])
		{
			if ((aux)->content[scan] == '$')
			{
				if ((aux)->prev && (aux)->prev->type == HEREDOC)
					break ;
				(aux)->type = VAR;
			}
		}
		aux = aux->next;
	}
}
