/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: algalian <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 20:59:16 by algalian          #+#    #+#             */
/*   Updated: 2025/02/04 20:59:17 by algalian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*mini_strjoin(char *str1, char *str2)
{
	char	*aux;

	if (!str2)
		return (str1);
	if (!str1)
		return (ft_strdup(str2));
	aux = str1;
	str1 = ft_strjoin(aux, str2);
	free_star(aux);
	return (str1);
}

void	ft_putendl_fd(char *s, int fd)
{
	ft_putstr_fd(s, fd);
	write(fd, "\n", 1);
}

void	err_stx_out(char *message, char *quote, int in_quote)
{
	char	*out;

	out = ft_strdup("minishell: ");
	out = mini_strjoin(out, message);
	if (in_quote)
		out = mini_strjoin(out, " `");
	else
		out = mini_strjoin(out, ": ");
	out = mini_strjoin(out, quote);
	if (in_quote)
		out = mini_strjoin(out, "'");
	ft_putendl_fd(out, STDERR_FILENO);
	free_star(out);
}

bool	stx_err_iterator(t_token **token_list)
{
	t_token	*aux;

	aux = *token_list;
	while (aux)
	{
		if (stx_error_cases(aux))
		{
			if (aux->type == END && aux->prev && aux->prev->type > PIPE)
				err_stx_out(ERR_SYNTX_TKN, "newline", true);
			else if (aux->type == END && aux->prev && aux->prev->type == PIPE)
				err_stx_out(ERR_SYNTX_TKN, aux->prev->content, true);
			else
				err_stx_out(ERR_SYNTX_TKN, aux->content, true);
			return (true);
		}
		aux = aux->next;
	}
	return (false);
}

bool	stx_err(t_token **token_list)
{
	t_token	*aux;

	aux = *token_list;
	if (aux->type == PIPE)
	{
		err_stx_out(ERR_SYNTX_TKN, aux->content, true);
		g_status = ERR_PIPE_STX;
		return (true);
	}
	while (aux)
	{
		if (stx_err_iterator(&aux))
			return (true);
		aux = aux->next;
	}
	return (false);
}
