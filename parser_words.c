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

bool	cmd_arg_only_fill(t_token **arg_list, t_command *command, bool is_echo)
{
	int		var_word_amnt;
	t_token	*aux;
	int		args_amnt;
	char	**args_table;

	if (is_echo)
		rm_echo_empty_words(arg_list);
	aux = *arg_list;
	var_word_amnt = 0;
	while (aux && (aux->type == WORD || aux->type == VAR))
	{
		var_word_amnt++;
		aux = aux->next;
	}
	args_amnt = 0;
	while (command->args[args_amnt])
		args_amnt++;
	args_table = malloc(sizeof(char *) * (var_word_amnt + args_amnt + 1));
	if (!args_table)
		return (false);
	args_table = create_table(args_amnt, args_table, command, arg_list);
	free(command->args);
	command->args = args_table;
	*arg_list = aux;
	return (true);
}

bool	cmd_arg_filler(t_token **arg_list, t_command *command)
{
	if (ft_strcmp(command->cmd, "echo") == 0)
	{
		if (!(command->args))
			return (cmd_arg_creat_n_fill(arg_list, command, true));
		else
			return (cmd_arg_only_fill(arg_list, command, true));
	}
	else
	{
		if (!(command->args))
			return (cmd_arg_creat_n_fill(arg_list, command, false));
		else
			return (cmd_arg_only_fill(arg_list, command, false));
	}
	return (true);
}



void	split_in_args(t_command *new_cmd, char *tkn_cntnt, t_minishell *ms)
{
	char		**splited_cntn;
	t_token		*args_list;
	t_token		*free_tkn_ptr;
	int			i;

	splited_cntn = ft_split(tkn_cntnt, ' ');
	if (!splited_cntn)
		return ;
	new_cmd->cmd = ft_strdup(splited_cntn[0]);
	args_list = NULL;
	if (splited_cntn[1])
	{
		i = 0;
		while (splited_cntn[++i])
			add_tkn_lst(&args_list,
				tkn_create(ft_strdup(splited_cntn[i]), NULL, WORD, OK_Q));
	}
	add_tkn_lst(&args_list, tkn_create(NULL, NULL, END, OK_Q));
	if (!cmd_arg_filler(&args_list, new_cmd))
		exit_and_msg(ms, ERR_ALLOC, EXIT_FAILURE);
	free_tkn_ptr = args_list;
	clean_tkn_nodes(&free_tkn_ptr, &free_star);
	free_two_stars(splited_cntn);
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
			exit_and_msg(ms, ERR_ALLOC, EXIT_FAILURE);
	}
	*aux = token;
}
