#include"minishell.h"




bool	fds_error2(t_fds *fds)
{
	if (fds->outfile)
	{
		if (fds->fd_outfile == -1 || (fds->infile && fds->fd_infile == -1))
			return (true);
		free_star(fds->outfile);
		close(fds->fd_outfile);
	}
	return (false);
}


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

void	create_trunc(t_fds *fds, char *file_name, char *cc)
{
	if (fds_error2(fds))
		return ;
	fds->outfile = ft_strdup(file_name);
	if (fds->outfile && fds->outfile[0] == '\0' && fds->error_msg == false)
	{
		msg_err(cc, NULL, "ambiguous redirect", false);
		fds->error_msg = true;
		return ;
	}
	fds->fd_outfile = open(fds->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0664);
	if (fds->fd_outfile == -1 && fds->error_msg == false)
	{
		msg_err(fds->outfile, NULL, strerror(errno), false);
		fds->error_msg = true;
	}
}

void	trunc_parser(t_minishell *ms, t_token **aux)
{
	t_token		*tkn_process;
	t_command	*last_cmd;

	tkn_process = *aux;
	last_cmd = scroll_lstcmd(ms->command);
	if (last_cmd->fds && last_cmd->fds->error_msg)
	{
		skip_next_token(aux); 
		return ;
	}
	if (!set_fd_struct(last_cmd))
		exit_msg(ms, ERR_ALLOC, EXIT_FAILURE);
	if (!ms->ctrlcheredoc) 
		create_trunc(last_cmd->fds, tkn_process->next->content, \
	tkn_process->next->cc);
	skip_next_token(aux);
}

void	append_file(t_fds *fds, char *file_name, char *cc)
{
	if (fds_error2(fds))
		return ;
	fds->outfile = ft_strdup(file_name);
	if (fds->outfile && fds->outfile[0] == '\0' && cc)
	{
		msg_err(cc, NULL, "ambiguous redirect", false);
		return ;
	}
	fds->fd_outfile = open(fds->outfile, O_WRONLY | O_CREAT | O_APPEND, 0664);
	if (fds->fd_outfile == -1)
		msg_err(fds->outfile, NULL, strerror(errno), false);
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

void	pipe_parser(t_minishell *ms, t_token **token_lst)
{
	t_command	*last_cmd;

	last_cmd = scroll_lstcmd(ms->command);
	last_cmd->pipe_output = true;
	addlst_cmd_container(ms, &last_cmd);
	*token_lst = (*token_lst)->next;
}

void	parser_main(t_minishell *ms)
{
	t_token	*aux;

	aux = ms->token;
	if (aux->type == END)
		return ;
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