/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_main.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: algalian <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 17:20:47 by algalian          #+#    #+#             */
/*   Updated: 2025/02/04 17:20:50 by algalian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cmd_no_arg_prccs(t_minishell *ms)
{
	t_command	*new_cmd;

	if (!ms || !ms->command)
		return ;
	new_cmd = ms->command;
	while (new_cmd && new_cmd->cmd)
	{
		if (!new_cmd->args)
		{
			new_cmd->args = malloc(sizeof * new_cmd->args * 2);
			new_cmd->args[0] = ft_strdup(new_cmd->cmd);
			new_cmd->args[1] = NULL;
		}
		new_cmd = new_cmd->next;
	}
	new_cmd = scroll_lstcmd(ms->command);
}

void	parser_main(t_minishell *ms)
{
	t_token	*aux;

	aux = ms->token;
	if (aux->type == END)
		return;
	while (aux->next)
	{
		if (aux == ms->token)
			addlst_cmd_container(ms, &ms->command);
		if (aux->type == WORD || aux->type == VAR)
			word_n_var_parser(ms, &aux);
		else if (aux->type == INPUT)
			infile_parser(ms, &aux);
		else if (aux->type == HEREDOC)
			heredoc_main(ms, &aux);
		else if (aux->type == TRUNC)
			trunc_parser(ms, &aux);
		else if (aux->type == APPEND)
			append_parser(ms, &aux);
		else if (aux->type == PIPE)
			pipe_parser(ms, &aux);
		else if (aux->type == END)
			break ;
	}
	cmd_no_arg_prccs(ms);
}
