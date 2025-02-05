/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_append.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: algalian <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 20:38:29 by algalian          #+#    #+#             */
/*   Updated: 2025/02/04 20:38:31 by algalian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	append_file(t_fds *fds, char *file_name, char *cc)
{
	if (fds_error2(fds))
		return ;
	fds->outfile = ft_strdup(file_name);
	if (fds->outfile && fds->outfile[0] == '\0' && cc)
	{
		error_msg(cc, NULL, "ambiguous redirect", false);
		return ;
	}
	fds->fd_outfile = open(fds->outfile, O_WRONLY | O_CREAT | O_APPEND, 0664);
	if (fds->fd_outfile == -1)
		error_msg(fds->outfile, NULL, strerror(errno), false);
}

void	append_parser(t_minishell *ms, t_token **aux)
{
	t_token		*tkn_process;
	t_command	*last_cmd;

	tkn_process = *aux;
	last_cmd = scroll_lstcmd(ms->command);
	if (!set_fd_struct(last_cmd))
		exit_msg(ms, ERR_ALLOC, EXIT_FAILURE);
	if (!ms->ctrlcheredoc)
		append_file(last_cmd->fds, tkn_process->next->content, \
	tkn_process->next->cc);
	skip_next_token(aux);
}
