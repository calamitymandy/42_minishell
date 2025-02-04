/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_scan.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: algalian <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 18:46:51 by algalian          #+#    #+#             */
/*   Updated: 2025/02/04 18:46:53 by algalian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	quote_stat_expndr(t_token **node, char scan)
{
	if (scan == '\'' && (*node)->var_q_stat == OK_Q)
		(*node)->var_q_stat = OPN_SQ;
	else if (scan == '\"' && (*node)->var_q_stat == OK_Q)
		(*node)->var_q_stat = OPN_DQ;
	else if (scan == '\'' && (*node)->var_q_stat == OPN_SQ)
		(*node)->var_q_stat = OK_Q;
	else if (scan == '\"' && (*node)->var_q_stat == OPN_DQ)
		(*node)->var_q_stat = OK_Q;
}

bool	is_between_d_quot(char *content, int scan)
{
	if (scan > 0)
	{
		if (content[scan - 1] == '\"' && content[scan + 1] == '\"')
			return (true);
		else
			return (false);
	}
	return (false);
}

bool	dollar_error(char *content, int scan)
{
	if ((is_bad_char_next(content[scan + 1])) \
		|| (is_between_d_quot(content, scan)))
		return (true);
	else
		return (false);
}

bool	is_env_var(t_minishell *ms, char *var_nme)
{
	int		i;
	int		len;

	i = -1;
	len = ft_strlen(var_nme);
	while (ms->env[++i])
		if (ft_strncmp(ms->env[i], var_nme, len) == 0)
			return (true);
	return (false);
}

void	scan_variables(t_minishell *ms)
{
	t_token	*aux;
	int		scan;

	aux = ms->token;
	while (aux)
	{
		if (aux->type == VAR)
		{
			scan = 0;
			while (aux->content[scan])
			{
				quote_stat_expndr(&aux, aux->content[scan]);
				if ((aux->content[scan] == '$' \
				&& !dollar_error(aux->content, scan)) \
				&& (aux->var_q_stat == OK_Q || aux->var_q_stat == OPN_DQ))
					process_variables(xtract_var_value \
					(aux, aux->content + scan, ms), &aux, scan, ms);
				else
					scan++;
			}
		}
		aux = aux->next;
	}
}
