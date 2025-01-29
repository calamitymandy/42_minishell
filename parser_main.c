#include"minishell.h"

bool	g_status_handler(char **line, t_minishell *ms)
{
	(void)ms;
	if (*line == NULL && g_status == 1)
		g_status = 1;
	else if (*line == NULL)
	{
		g_status = 0;
		ms->ctrlcheredoc = true;
	}
	return (false);
}
void	quit_newline_char(char *line)
{
	size_t	len;

	len = ft_strlen(line);
	if (len > 0 && line[len - 1] == '\n')
		line[len - 1] = '\0';
}

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

void	skip_next_token(t_token **aux)
{
	if ((*aux)->next->next)
		*aux = (*aux)->next->next;
	else
		*aux = (*aux)->next;
}

bool	fds_error(t_fds *fds)
{
	if (fds->infile)
	{
		if (fds->fd_infile == -1 || (fds->outfile && fds->fd_outfile == -1))
			return (true);
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

void	infile_open(t_fds *fds, char *infile_name, char *cc)
{
	if (fds_error(fds))
		return ;
	fds->infile = ft_strdup(infile_name);
	if (fds->infile && fds->infile[0] == '\0')
	{
		msg_err(cc, NULL, "ambiguous redirect", false);
		fds->error_msg = true;
		return ;
	}
	fds->fd_infile = open(fds->infile, O_RDONLY);
	if (fds->fd_infile == -1 && fds->error_msg == false)
	{
		msg_err(fds->infile, NULL, strerror(errno), false);
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
		exit_msg(ms, ERR_ALLOC, EXIT_FAILURE);
	infile_open(last_cmd->fds, aux_aux->next->content, aux_aux->next->cc);
	skip_next_token(aux);
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

char	*heredoc_var_xpndr(t_minishell *ms, char *var)
{
	int	i;

	i = 0;
	while (var[i])
	{
		if (var[i] == '$'
			&& !dollar_error(var, i))
			var = replace_str_heredoc(var, \
			xtract_var_value(NULL, var + i, ms), i);
		else
			i++;
	}
	return (var);
}

char	*addspace_btwn_words(char **words)
{
	char	*str;
	char	*tmp;
	int		i;

	i = -1;
	while (words[++i])
	{
		tmp = str;
		if (i == 0)
			str = ft_strdup(words[0]);
		else
		{
			str = ft_strjoin(tmp, words[i]);
			free_star(tmp);
		}
		if (words[i + 1])
		{
			tmp = str;
			str = ft_strjoin(tmp, " ");
			free_star(tmp);
		}
	}
	free_two_stars(words);
	return (str);
}
char	*heredoc_xpndr_main(t_minishell *ms, char *line)
{
	char	**words;
	int		i;

	words = ft_split(line, ' ');
	if (!words)
		return (NULL);
	i = 0;
	while (words[i])
	{
		if (ft_strchr(words[i], '$'))
		{
			words[i] = heredoc_var_xpndr(ms, words[i]);
			if (!words[i])
				return (NULL);
		}
		i++;
	}
	return (addspace_btwn_words(words));
}

bool	loop_breaker(t_minishell *ms, char **line, t_fds *fds, bool *success)
{
	if ((*line == NULL && g_status == 1) || *line == NULL)
	{
		*success = g_status_handler(line, ms);
		return (BREAK);
	}
	if (ft_strcmp(*line, fds->del_heredoc) == 0)
	{
		*success = true;
		return (BREAK);
	}
	if (fds->heredoc_quotes == false && ft_strchr(*line, '$'))
	{
		*line = heredoc_xpndr_main(ms, *line);
		if (!(*line))
		{
			free_star(*line);
			*success = false;
			return (BREAK);
		}
	}
	return (GO);
}

bool	heredoc_loop(t_minishell *ms, t_fds *fds, int tmp_fd)
{
	char	*line;
	bool	success;

	success = false;
	line = NULL;
	while (1)
	{
		listening_hdoc_input_sig();
		ft_putstr_fd("> ", 1);
		line = get_next_line(STDIN_FILENO); 
		listening_no_interact_sig(); //ambiguous name
		if (line)
			quit_newline_char(line);
		if (loop_breaker(ms, &line, fds, &success))
			break ;
		ft_putendl_fd(line, tmp_fd);
		free_star(line);
	}
	free_star(line);
	return (success);
}

bool	create_tmp(t_minishell *ms, t_fds *fds)
{
	int		tmp_fd;
	bool	success;

	success = true;
	tmp_fd = open(fds->infile, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (tmp_fd == -1)
		success = false;
	else
	{
		success = heredoc_loop(ms, fds, tmp_fd);
		close(tmp_fd);
	}
	return (success);
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
	if (fds_error(fds))
		return ;
	fds->infile = put_name_tmp();
	if (!fds->infile)
		exit_msg(ms, ERR_ALLOC, EXIT_FAILURE);
	fds->del_heredoc = quit_heredoc_quot(pre_delim->next->content, \
	& (fds->heredoc_quotes));
	if (create_tmp(ms, fds))
		fds->fd_infile = open(fds->infile, O_RDONLY);
	else
	{
		fds->fd_infile = -1;
	}
	skip_next_token(aux);
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