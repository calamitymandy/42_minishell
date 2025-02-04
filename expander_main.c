/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: algalian <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 18:03:55 by algalian          #+#    #+#             */
/*   Updated: 2025/02/04 18:03:56 by algalian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	val_cpy(char *new, char *value, int *j)
{
	int	i;

	i = 0;
	while (value[i])
	{
		new[*j] = value[i];
		i++;
		(*j)++;
	}
}

void	expander_main(t_minishell *ms)
{
	mark_variables(ms);
	scan_variables(ms);
	process_quotes(ms);
}
