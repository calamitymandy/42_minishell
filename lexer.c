/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: algalian <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 20:41:03 by algalian          #+#    #+#             */
/*   Updated: 2025/02/04 20:41:05 by algalian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	stx_error_cases(t_token *token)
{
	if (token->prev)
	{
		if (token->type == PIPE && token->prev->type >= PIPE)
		{
			g_status = ERR_PIPE_STX;
			return (true);
		}
		if (token->type > PIPE && token->prev->type > PIPE)
		{
			g_status = ERR_PIPE_STX;
			return (true);
		}
		if (token->type == END && token->prev->type >= PIPE)
		{
			g_status = ERR_PIPE_STX;
			return (true);
		}
	}
	return (false);
}

void	token_indx(t_minishell *ms)
{
	t_token			*aux;
	unsigned int	i;

	aux = ms->token;
	i = 0;
	while (aux->next)
	{
		aux->index += i;
		i++;
		aux = aux->next;
	}
}

bool	lexer_main(t_minishell *ms)
{
	if (!ms->line)
		exec_exit_builtin(ms, NULL);
	else if (is_line_empty(ms->line))
		return (true);
	add_history(ms->line);
	if (quotes_err_n_read(ms, ms->line))
		return (false);
	if (stx_err(&ms->token))
		return (false);
	expander_main(ms);
	token_indx(ms);
	parser_main(ms);
	return (true);
}
