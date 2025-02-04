/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_words.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: algalian <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 17:25:18 by algalian          #+#    #+#             */
/*   Updated: 2025/02/04 17:25:20 by algalian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_there_space(char *str)
{
	int	i;

	i = -1;
	while (str[++i])
		if (str[i] == ' ')
			return (true);
	return (false);
}

int	word_n_var_counter(t_token *aux)
{
	int	i;

	i = 0;
	while (aux && (aux->type == WORD || aux->type == VAR))
	{
		i++;
		aux = aux->next;
	}
	return (i);
}

void	rm_echo_empty_words(t_token **arg_list)
{
	t_token	*aux;

	aux = *arg_list;
	while (aux->type == WORD || aux->type == VAR)
	{
		if (aux->type == VAR && aux->content[0] == '\0' && \
		!aux->has_quotes)
		{
			aux = aux->next;
			if (aux == (*arg_list)->next)
				(*arg_list) = (*arg_list)->next;
			del_one_node_tkn(aux->prev, free_star);
		}
		else
			aux = aux->next;
	}
}

char	**create_table(
	int args_amnt, char **args_table, t_command *command, t_token **arg_list)
{
	int		i;
	t_token	*aux;

	i = 0;
	aux = *arg_list;
	while (i < args_amnt)
	{
		args_table[i] = command->args[i];
		i++;
	}
	while (aux->type == WORD || aux->type == VAR)
	{
		args_table[i] = ft_strdup(aux->content);
		i++;
		aux = aux->next;
	}
	args_table[i] = NULL;
	return (args_table);
}

void	word_n_var_parser(t_minishell *ms, t_token **aux)
{
	t_token		*token;
	t_command	*new_cmd;

	token = *aux;
	while (token->type == WORD || token->type == VAR)
	{
		new_cmd = scroll_lstcmd(ms->command);
		if (token->prev == NULL || (token->prev && token->prev->type == PIPE)
			|| new_cmd->cmd == NULL || new_cmd->cmd[0] == '\0')
		{
			if (token->type == VAR && is_there_space(token->content))
				split_in_args(new_cmd, token->content, ms);
			else
				new_cmd->cmd = ft_strdup(token->content);
			token = token->next;
		}
		else if (!cmd_arg_filler(&token, new_cmd))
			exit_msg(ms, ERR_ALLOC, EXIT_FAILURE);
	}
	*aux = token;
}
