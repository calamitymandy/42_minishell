/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_infile.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: algalian <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 19:49:13 by algalian          #+#    #+#             */
/*   Updated: 2025/02/04 19:49:15 by algalian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"minishell.h"

void	skip_next_token(t_token **aux)
{
	if ((*aux)->next->next)
		*aux = (*aux)->next->next;
	else
		*aux = (*aux)->next;
}

bool	set_fd_struct(t_command *command)
{
	if (!command->fds)
	{
		command->fds = malloc(sizeof * command->fds);
		if (!command->fds)
			return (false);
		command->fds->infile = NULL;
		command->fds->outfile = NULL;
		command->fds->del_heredoc = NULL;
		command->fds->heredoc_quotes = false;
		command->fds->fd_infile = -1;
		command->fds->fd_outfile = -1;
		command->fds->stdin_ori = -1;
		command->fds->stdout_ori = -1;
		command->fds->error_msg = false;
	}
	return (true);
}

void	infile_open(t_fds *fds, char *infile_name, char *cc)
{
	if (fds_error(fds))
		return ;
	fds->infile = ft_strdup(infile_name);
	if (fds->infile && fds->infile[0] == '\0')
	{
		error_msg(cc, NULL, "ambiguous redirect", false);
		fds->error_msg = true;
		return ;
	}
	fds->fd_infile = open(fds->infile, O_RDONLY);
	if (fds->fd_infile == -1 && fds->error_msg == false)
	{
		error_msg(fds->infile, NULL, strerror(errno), false);
		fds->error_msg = true;
	}
}


void	infile_parser(t_minishell *ms, t_token **aux)
{
	t_token		*aux_aux;
	t_command	*last_cmd;

	aux_aux = *aux;
	last_cmd = scroll_lstcmd(ms->command);
	if (last_cmd->fds && last_cmd->fds->error_msg)
	{
		skip_next_token(aux);
		return ;
	}
	if (!set_fd_struct(last_cmd))
		exit_and_msg(ms, ERR_ALLOC, EXIT_FAILURE);
	infile_open(last_cmd->fds, aux_aux->next->content, aux_aux->next->cc);
	skip_next_token(aux);
}
