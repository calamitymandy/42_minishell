/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_quotes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: algalian <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 19:39:21 by algalian          #+#    #+#             */
/*   Updated: 2025/02/04 19:39:25 by algalian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	len_wthout_quot(char *content, int len)
{
	int	status;
	int	i;

	status = OK_Q;
	i = 0;
	while (content[i])
	{
		if ((content[i] == '\'' || content[i] == '\"') && status == OK_Q)
		{
			if (content[i] == '\'')
				status = OPN_SQ;
			if (content[i] == '\"')
				status = OPN_DQ;
			i++;
		}
		else if ((content[i] == '\'' && status == OPN_SQ)
			|| (content[i] == '\"' && status == OPN_DQ))
		{
			status = OK_Q;
			i++;
		}
		len++;
		i++;
	}
	return (len);
}

void	skip_quot_n_open(t_token **aux, int *pos)
{
	if ((*aux)->content[*pos] == '\'')
		(*aux)->var_q_stat = OPN_SQ;
	if ((*aux)->content[*pos] == '\"')
		(*aux)->var_q_stat = OPN_DQ;
	(*pos)++;
}

bool	skip_quot_n_close(t_token **aux, int *pos)
{
	if (((*aux)->content[*pos] == '\'' && (*aux)->var_q_stat == OPN_SQ)
		|| ((*aux)->content[*pos] == '\"' \
		&& (*aux)->var_q_stat == OPN_DQ))
	{
		(*aux)->var_q_stat = OK_Q;
		(*pos)++;
		return (true);
	}
	else
		return (false);
}

void	quote_eraser(t_token **aux)
{
	int		i;
	int		j;
	char	*noq_str;

	i = 0;
	j = 0;
	noq_str = malloc(sizeof(char) * len_wthout_quot((*aux)->content, i) + 1);
	while ((*aux)->content[i])
	{
		if (is_quote_here(aux, i))
			skip_quot_n_open(aux, &i);
		else if (skip_quot_n_close(aux, &i))
			continue ;
		else
			noq_str[j++] = (*aux)->content[i++];
	}
	noq_str[j] = '\0';
	free_star((*aux)->content);
	(*aux)->content = noq_str;
	(*aux)->has_quotes = true;
}

int	process_quotes(t_minishell *ms)
{
	t_token	*aux;

	aux = ms->token;
	while (aux)
	{
		if (quote_detector(aux->content) \
			&& (aux->prev == NULL || (aux->prev && aux->prev->type != HEREDOC)))
			quote_eraser(&aux);
		aux = aux->next;
	}
	return (0);
}
