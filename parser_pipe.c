/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_pipe.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: algalian <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 19:45:59 by algalian          #+#    #+#             */
/*   Updated: 2025/02/04 19:46:01 by algalian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	pipe_parser(t_minishell *ms, t_token **token_lst)
{
	t_command	*last_cmd;

	last_cmd = scroll_lstcmd(ms->command);
	last_cmd->pipe_output = true;
	addlst_cmd_container(ms, &last_cmd);
	*token_lst = (*token_lst)->next;
}
