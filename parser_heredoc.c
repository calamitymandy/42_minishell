/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: algalian <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 19:48:31 by algalian          #+#    #+#             */
/*   Updated: 2025/02/04 19:48:33 by algalian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*put_name_tmp(void)
{
	static int	i;
	char		*name;
	char		*number;

	number = ft_itoa(i);
	if (!number)
		return (NULL);
	name = ft_strjoin("/tmp/.minishell_heredoc", number);
	free(number);
	i++;
	return (name);
}

char	*quit_heredoc_quot(char *delim, bool *heredoc_quotes)
{
	int	len;

	len = ft_strlen(delim) - 1;
	if ((delim[0] == '\"' && delim[len] == '\"')
		|| (delim[0] == '\'' && delim[len] == '\''))
	{
		*heredoc_quotes = true;
		return (ft_strtrim(delim, "\'\""));
	}
	return (ft_strdup(delim));
}

bool	fds_error(t_fds *fds)
{
	if (fds->infile)
	{
		if (fds->fd_infile == -1 || (fds->outfile && fds->fd_outfile == -1))
		{
			return (true);
		}
		if (fds->del_heredoc)
		{
			free_star(fds->del_heredoc);
			fds->del_heredoc = NULL;
			unlink(fds->infile);
		}
		free_star(fds->infile);
		close(fds->fd_infile);
	}
	return (false);
}

void	heredoc_main(t_minishell *ms, t_token **aux)
{
	t_token		*pre_delim;
	t_command	*last_cmd;
	t_fds		*fds;	

	pre_delim = *aux;
	last_cmd = scroll_lstcmd(ms->command);
	if (!set_fd_struct(last_cmd))
		exit_msg(ms, ERR_ALLOC, EXIT_FAILURE);
	fds = last_cmd->fds;
	fds->infile = put_name_tmp();
	if (!fds->infile)
		exit_msg(ms, ERR_ALLOC, EXIT_FAILURE);
	fds->del_heredoc = quit_heredoc_quot(pre_delim->next->content, \
	& (fds->heredoc_quotes));
	if (create_tmp(ms, fds))
		fds->fd_infile = open(fds->infile, O_RDONLY);
	else
		fds->fd_infile = -1;
	skip_next_token(aux);
}
