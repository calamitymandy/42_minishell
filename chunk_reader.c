/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunk_reader.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: algalian <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 19:40:36 by algalian          #+#    #+#             */
/*   Updated: 2025/02/04 19:40:38 by algalian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	chck_oprtr_type(char *line, int scan)
{
	int	size;

	size = ft_strlen(line + scan);
	if (((line[scan] > 8 && line[scan] < 14) || line[scan] == 32))
		return (SPACES);
	else if (line[scan] == '|')
		return (PIPE);
	else if (size >= 3 && line[scan] == '<' && line[scan + 1] == '<')
		return (HEREDOC);
	else if (size >= 3 && line[scan] == '>' && line[scan + 1] == '>')
		return (APPEND);
	else if (size >= 2 && line[scan] == '<')
		return (INPUT);
	else if (size >= 2 && line[scan] == '>')
		return (TRUNC);
	else if (line[scan] == '<' || line[scan] == '>')
		return (FAILURE);
	else if (line[scan] == '\0')
		return (END);
	else
		return (0);
}

bool	word_to_tkn(t_token **tkns, char *line, int scan, int start_word)
{
	int		i;
	char	*word;

	word = malloc(sizeof(char) * (scan - start_word + 1));
	if (!word)
	{
		return (false);
	}
	i = 0;
	while (start_word < scan)
	{
		word[i] = line[start_word];
		start_word++;
		i++;
	}
	word[i] = '\0';
	add_tkn_lst(tkns, tkn_create (word, ft_strdup(word), WORD, OK_Q));
	return (true);
}

bool	oprtr_to_tkn(t_token **tkns, char *line, int scan, int oprtr_type)
{
	int		i;
	char	*oprtr;

	i = 0;
	if (oprtr_type == APPEND || oprtr_type == HEREDOC)
	{
		oprtr = malloc(sizeof(char) * 3);
		if (!oprtr)
			return (false);
		while (i < 2)
			oprtr[i++] = line[scan++];
		oprtr[i] = '\0';
		add_tkn_lst(tkns, tkn_create(oprtr, NULL, oprtr_type, OK_Q));
	}
	else
	{
		oprtr = malloc(sizeof(char) * 2);
		if (!oprtr)
			return (false);
		while (i < 1)
			oprtr[i++] = line[scan++];
		oprtr[i] = '\0';
		add_tkn_lst(tkns, tkn_create(oprtr, NULL, oprtr_type, OK_Q));
	}
	return (true);
}

int	chunk_reader(int *scan, char *line, int start_word, t_minishell *ms)
{
	int	oprtr;

	oprtr = chck_oprtr_type(line, (*scan));
	if (oprtr)
	{
		if ((*scan) != 0 && !chck_oprtr_type(line, (*scan) - 1))
			if (!word_to_tkn(&ms->token, line, (*scan), start_word))
				exit_and_msg(ms, ERR_ALLOC, EXIT_FAILURE);
		if (oprtr && oprtr != SPACES)
		{
			if (!oprtr_to_tkn(&ms->token, line, (*scan), oprtr))
				exit_and_msg(ms, ERR_ALLOC, EXIT_FAILURE);
			if (oprtr == APPEND || oprtr == HEREDOC)
				(*scan)++;
		}
		start_word = (*scan) + 1;
	}
	return (start_word);
}
