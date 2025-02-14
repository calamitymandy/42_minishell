/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_args.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: algalian <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 17:58:13 by algalian          #+#    #+#             */
/*   Updated: 2025/02/04 17:58:35 by algalian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	cmd_arg_creat_n_fill(t_token **arg_list, \
	t_command *command, bool is_echo)
{
	int		arg_amnt;
	t_token	*aux;
	int		i;

	if (is_echo)
		rm_echo_empty_words(arg_list);
	aux = *arg_list;
	arg_amnt = word_n_var_counter(aux);
	command->args = malloc(sizeof(char *) * (arg_amnt + 2));
	if (!command->args)
		return (false);
	i = 0;
	command->args[i] = ft_strdup(command->cmd);
	i++;
	while (aux->type == WORD || aux->type == VAR)
	{
		command->args[i] = ft_strdup(aux->content);
		i++;
		aux = aux->next;
	}
	command->args[i] = NULL;
	*arg_list = aux;
	return (true);
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
		exit_msg(ms, ERR_ALLOC, EXIT_FAILURE);
	free_tkn_ptr = args_list;
	clean_tkn_nodes(&free_tkn_ptr, &free_star);
	free_two_stars(splited_cntn);
}
