#include"minishell.h"

bool	ms_g_status_handler(char **line, t_mshl *ms)
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
void	ms_quit_newline_char(char *line)
{
	size_t	len;

	len = ft_strlen(line);
	if (len > 0 && line[len - 1] == '\n')
		line[len - 1] = '\0';
}

char	*ms_put_name_tmp(void)
{
	static int	i;
	char		*name;
	char		*number;

	number = ft_itoa(i);
	if (!number)
		return (NULL);
	name = ft_strjoin("/tmp/.anshellmo_heredoc", number);
	free(number);
	i++;
	return (name);
}


void	ms_ptr_free(void *ptr)
{
	if (ptr != NULL)
	{
		free(ptr);
		ptr = NULL;
	}
}

void	ms_ptr_free_arr(char **tab)
{
	int	i;

	i = 0;
	if (tab)
	{
		while (tab[i])
		{
			if (tab[i])
			{
				ms_ptr_free(tab[i]);
				tab[i] = NULL;
			}
			i++;
		}
		free(tab);
		tab = NULL;
	}
}

bool	ms_fds_error2(t_fds *fds)
{
	if (fds->outfile)
	{
		if (fds->fd_out == -1 || (fds->infile && fds->fd_in == -1))
			return (true);
		ms_ptr_free(fds->outfile);
		close(fds->fd_out);
	}
	return (false);
}

void	ms_skip_next_token(t_token **aux)
{
	if ((*aux)->next->next)
		*aux = (*aux)->next->next;
	else
		*aux = (*aux)->next;
}

bool	ms_fds_error(t_fds *fds)
{
	if (fds->infile)
	{
		if (fds->fd_in == -1 || (fds->outfile && fds->fd_out == -1))
			return (true);
		if (fds->heredoc_del)
		{
			ms_ptr_free(fds->heredoc_del);
			fds->heredoc_del = NULL;
			unlink(fds->infile);
		}
		ms_ptr_free(fds->infile);
		close(fds->fd_in);
	}
	return (false);
}
bool	ms_set_fd_struct(t_command *cmd)
{
	if (!cmd->fds)
	{
		cmd->fds = malloc(sizeof * cmd->fds);
		if (!cmd->fds)
			return (false);
		cmd->fds->infile = NULL;
		cmd->fds->outfile = NULL;
		cmd->fds->heredoc_del = NULL;
		cmd->fds->heredoc_quotes = false;
		cmd->fds->fd_in = -1;
		cmd->fds->fd_out = -1;
		cmd->fds->stdin_backup = -1;
		cmd->fds->stdout_backup = -1;
		cmd->fds->error_msg = false;
	}
	return (true);
}



void	ms_del_all_nodes_tkn(t_token **lst, void (*del)(void *))
{
	t_token	*tmp;

	tmp = NULL;
	while (*lst != NULL)
	{
		tmp = (*lst)->next;
		ms_del_one_node_tkn(*lst, del);
		*lst = tmp;
	}
}


t_command	*ms_scroll_lstcmd(t_command *aux)
{
	while (aux->next != NULL)
		aux = aux->next;
	return (aux);
}

void	ms_cmd_no_arg_prccs(t_minishell *ms)
{
	t_command	*new_cmd;

	if (!ms || !ms->cmd)
		return ;
	new_cmd = ms->cmd;
	while (new_cmd && new_cmd->command)
	{
		if (!new_cmd->args)
		{
			new_cmd->args = malloc(sizeof * new_cmd->args * 2);
			new_cmd->args[0] = ft_strdup(new_cmd->command);
			new_cmd->args[1] = NULL;
		}
		new_cmd = new_cmd->next;
	}
	new_cmd = ms_scroll_lstcmd(ms->cmd);
}

t_command	*ms_new_cmd_lst(void)
{
	t_command	*new_node;

	new_node = (t_command *)malloc(sizeof(t_command));
	if (!(new_node))
		return (NULL);
	ft_memset(new_node, 0, sizeof(t_command));
	new_node->command = NULL;
	new_node->path = NULL;
	new_node->args = NULL;
	new_node->pipe_output = false;
	new_node->pipe_fd = 0;
	new_node->prev = NULL;
	new_node->next = NULL;
	return (new_node);
}




void	ms_addlst_cmd_container(t_minishell *ms, t_command **cmd_list)
{
	t_command	*new_node;
	t_command	*aux;

	new_node = ms_new_cmd_lst();
	if (!new_node)
		ms_exit_msg(ms, ERR_ALLOC, EXIT_FAILURE);
	aux = *cmd_list;
	if (!aux)
	{
		*cmd_list = new_node;
		return ;
	}
	if (new_node)
	{
		ms_scroll_lstcmd(aux);
		aux->next = new_node;
		new_node->prev = aux;
	}
}

bool	ms_is_there_space(char *str)
{
	int	i;

	i = -1;
	while (str[++i])
		if (str[i] == ' ')
			return (true);
	return (false);
}

void	ms_del_one_node_tkn(t_token *lst, void (*del)(void *))
{
	if (del && lst && lst->content)
	{
		(*del)(lst->content);
		lst->content = NULL;
	}
	if (del && lst && lst->cc)
	{
		(*del)(lst->cc);
		lst->cc = NULL;
	}
	if (lst->prev)
		lst->prev->next = lst->next;
	if (lst->next)
		lst->next->prev = lst->prev;
	ms_ptr_free(lst);
}

void	ms_rm_echo_empty_words(t_token **arg_list)
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
			ms_del_one_node_tkn(aux->prev, ms_ptr_free);
		}
		else
			aux = aux->next;
	}
}


int	ms_word_n_var_counter(t_token *aux)
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

bool	ms_cmd_arg_creat_n_fill(t_token **arg_list, \
	t_command *cmd, bool is_echo)
{
	int		arg_amnt;
	t_token	*aux;
	int		i;

	if (is_echo)
		ms_rm_echo_empty_words(arg_list);
	aux = *arg_list;
	arg_amnt = ms_word_n_var_counter(aux);
	cmd->args = malloc(sizeof(char *) * (arg_amnt + 2));
	if (!cmd->args)
		return (false);
	i = 0;
	cmd->args[i] = ft_strdup(cmd->command);
	i++;
	while (aux->type == WORD || aux->type == VAR)
	{
		cmd->args[i] = ft_strdup(aux->content);
		i++;
		aux = aux->next;
	}
	cmd->args[i] = NULL;
	*arg_list = aux;
	return (true);
}

char	**ms_create_table(
	int args_amnt, char **args_table, t_command *cmd, t_token **arg_list)
{
	int		i;
	t_token	*aux;

	i = 0;
	aux = *arg_list;
	while (i < args_amnt)
	{
		args_table[i] = cmd->args[i];
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

bool	ms_cmd_arg_only_fill(t_token **arg_list, t_command *cmd, bool is_echo)
{
	int		var_word_amnt;
	t_token	*aux;
	int		args_amnt;
	char	**args_table;

	if (is_echo)
		ms_rm_echo_empty_words(arg_list);
	aux = *arg_list;
	var_word_amnt = 0;
	while (aux && (aux->type == WORD || aux->type == VAR))
	{
		var_word_amnt++;
		aux = aux->next;
	}
	args_amnt = 0;
	while (cmd->args[args_amnt])
		args_amnt++;
	args_table = malloc(sizeof(char *) * (var_word_amnt + args_amnt + 1));
	if (!args_table)
		return (false);
	args_table = ms_create_table(args_amnt, args_table, cmd, arg_list);
	free(cmd->args);
	cmd->args = args_table;
	*arg_list = aux;
	return (true);
}

bool	ms_cmd_arg_filler(t_token **arg_list, t_command *cmd)
{
	if (ft_strcmp(cmd->command, "echo") == 0)
	{
		if (!(cmd->args))
			return (ms_cmd_arg_creat_n_fill(arg_list, cmd, true));
		else
			return (ms_cmd_arg_only_fill(arg_list, cmd, true));
	}
	else
	{
		if (!(cmd->args))
			return (ms_cmd_arg_creat_n_fill(arg_list, cmd, false));
		else
			return (ms_cmd_arg_only_fill(arg_list, cmd, false));
	}
	return (true);
}

void	ms_split_in_args(t_command *new_cmd, char *tkn_cntnt, t_minishell *ms)
{
	char		**splited_cntn;
	t_token		*args_list;
	t_token		*free_tkn_ptr;
	int			i;

	splited_cntn = ft_split(tkn_cntnt, ' ');
	if (!splited_cntn)
		return ;
	new_cmd->command = ft_strdup(splited_cntn[0]);
	args_list = NULL;
	if (splited_cntn[1])
	{
		i = 0;
		while (splited_cntn[++i])
			ms_add_tkn_lst(&args_list,
				ms_tkn_create(ft_strdup(splited_cntn[i]), NULL, WORD, OK_Q));
	}
	ms_add_tkn_lst(&args_list, ms_tkn_creat(NULL, NULL, END, OK_Q));
	if (!ms_cmd_arg_filler(&args_list, new_cmd))
		ms_exit_msg(ms, ERR_ALLOC, EXIT_FAILURE);
	free_tkn_ptr = args_list;
	ms_del_all_nodes_tkn(&free_tkn_ptr, &ms_ptr_free);
	ms_ptr_free_arr(splited_cntn);
}

void	ms_word_n_var_parser(t_minishell *ms, t_token **aux)
{
	t_token		*token;
	t_command	*new_cmd;

	token = *aux;
	while (token->type == WORD || token->type == VAR)
	{
		new_cmd = ms_scroll_lstcmd(ms->cmd);
		if (token->prev == NULL || (token->prev && token->prev->type == PIPE)
			|| new_cmd->command == NULL || new_cmd->command[0] == '\0')
		{
			if (token->type == VAR && ms_is_there_space(token->content))
				ms_split_in_args(new_cmd, token->content, ms);
			else
				new_cmd->command = ft_strdup(token->content);
			token = token->next;
		}
		else if (!ms_cmd_arg_filler(&token, new_cmd))
			ms_exit_msg(ms, ERR_ALLOC, EXIT_FAILURE);
	}
	*aux = token;
}



void	ms_infile_open(t_fds *fds, char *infile_name, char *cc)
{
	if (ms_fds_error(fds))
		return ;
	fds->infile = ft_strdup(infile_name);
	if (fds->infile && fds->infile[0] == '\0')
	{
		ms_msg_err(cc, NULL, "ambiguous redirect", false);
		fds->error_msg = true;
		return ;
	}
	fds->fd_in = open(fds->infile, O_RDONLY);
	if (fds->fd_in == -1 && fds->error_msg == false)
	{
		ms_msg_err(fds->infile, NULL, strerror(errno), false);
		fds->error_msg = true;
	}
}



void	ms_infile_parser(t_minishell *ms, t_token **aux)
{
	t_token		*aux_aux;
	t_command	*last_cmd;

	aux_aux = *aux;
	last_cmd = ms_scroll_lstcmd(ms->cmd);
	if (last_cmd->fds && last_cmd->fds->error_msg)
	{
		ms_skip_next_token(aux);
		return ;
	}
	if (!ms_set_fd_struct(last_cmd))
		ms_exit_msg(ms, ERR_ALLOC, EXIT_FAILURE);
	ms_infile_open(last_cmd->fds, aux_aux->next->content, aux_aux->next->cc);
	ms_skip_next_token(aux);
}

char	*ms_quit_heredoc_quot(char *delim, bool *heredoc_quotes)
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

char	*ms_heredoc_var_xpndr(t_mshl *ms, char *var)
{
	int	i;

	i = 0;
	while (var[i])
	{
		if (var[i] == '$'
			&& !ms_dollar_error(var, i))
			var = replace_str_heredoc(var, \
			ms_xtract_var_value(NULL, var + i, ms), i);
		else
			i++;
	}
	return (var);
}

char	*ms_heredoc_xpndr_main(t_mshl *ms, char *line)
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
			words[i] = ms_heredoc_var_xpndr(ms, words[i]);
			if (!words[i])
				return (NULL);
		}
		i++;
	}
	return (ms_addspace_btwn_words(words));
}

bool	ms_loop_breaker(t_mshl *ms, char **line, t_fds *fds, bool *success)
{
	if ((*line == NULL && g_status == 1) || *line == NULL)
	{
		*success = ms_g_status_handler(line, ms);
		return (BREAK);
	}
	if (ft_strcmp(*line, fds->heredoc_del) == 0)
	{
		*success = true;
		return (BREAK);
	}
	if (fds->heredoc_quotes == false && ft_strchr(*line, '$'))
	{
		*line = ms_heredoc_xpndr_main(ms, *line);
		if (!(*line))
		{
			ms_ptr_free(*line);
			*success = false;
			return (BREAK);
		}
	}
	return (GO);
}

bool	ms_heredoc_loop(t_minishell *ms, t_fds *fds, int tmp_fd)
{
	char	*line;
	bool	success;

	success = false;
	line = NULL;
	while (1)
	{
		ms_listening_hdoc_input_sig();
		ft_putstr_fd("> ", 1);
		line = get_next_line(STDIN_FILENO); 
		ms_listening_no_interact_sig();
		if (line)
			ms_quit_newline_char(line);
		if (ms_loop_breaker(ms, &line, fds, &success))
			break ;
		ft_putendl_fd(line, tmp_fd);
		ms_ptr_free(line);
	}
	ms_ptr_free(line);
	return (success);
}

bool	ms_create_tmp(t_minishell *ms, t_fds *fds)
{
	int		tmp_fd;
	bool	success;

	success = true;
	tmp_fd = open(fds->infile, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (tmp_fd == -1)
		success = false;
	else
	{
		success = ms_heredoc_loop(ms, fds, tmp_fd);
		close(tmp_fd);
	}
	return (success);
}

void	ms_heredoc_main(t_minishell *ms, t_token **aux)
{
	t_token		*pre_delim;
	t_command	*last_cmd;
	t_fds		*fds;

	pre_delim = *aux;
	last_cmd = ms_scroll_lstcmd(ms->cmd);
	if (!ms_set_fd_struct(last_cmd))
		ms_exit_msg(ms, ERR_ALLOC, EXIT_FAILURE);
	fds = last_cmd->fds;
	if (ms_fds_error(fds))
		return ;
	fds->infile = ms_put_name_tmp();
	if (!fds->infile)
		ms_exit_msg(ms, ERR_ALLOC, EXIT_FAILURE);
	fds->heredoc_del = ms_quit_heredoc_quot(pre_delim->next->content, \
	& (fds->heredoc_quotes));
	if (ms_create_tmp(ms, fds))
		fds->fd_in = open(fds->infile, O_RDONLY);
	else
	{
		fds->fd_in = -1;
	}
	ms_skip_next_token(aux);
}

void	ms_create_trunc(t_fds *fds, char *file_name, char *cc)
{
	if (ms_fds_error2(fds))
		return ;
	fds->outfile = ft_strdup(file_name);
	if (fds->outfile && fds->outfile[0] == '\0' && fds->error_msg == false)
	{
		ms_msg_err(cc, NULL, "ambiguous redirect", false);
		fds->error_msg = true;
		return ;
	}
	fds->fd_out = open(fds->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0664);
	if (fds->fd_out == -1 && fds->error_msg == false)
	{
		ms_msg_err(fds->outfile, NULL, strerror(errno), false);
		fds->error_msg = true;
	}
}

void	ms_trunc_parser(t_minishell *ms, t_token **aux)
{
	t_token		*tkn_process;
	t_command	*last_cmd;

	tkn_process = *aux;
	last_cmd = ms_scroll_lstcmd(ms->cmd);
	if (last_cmd->fds && last_cmd->fds->error_msg)
	{
		ms_skip_next_token(aux); // why just skip? why not ms_exit_msg altoghether?
		return ;
	}
	if (!ms_set_fd_struct(last_cmd))
		ms_exit_msg(ms, ERR_ALLOC, EXIT_FAILURE);
	if (!ms->ctrlcheredoc) // why this condition?
		ms_create_trunc(last_cmd->fds, tkn_process->next->content, \
	tkn_process->next->cc);
	ms_skip_next_token(aux);
}

void	ms_append_file(t_fds *fds, char *file_name, char *cc)
{
	if (ms_fds_error2(fds))
		return ;
	fds->outfile = ft_strdup(file_name);
	if (fds->outfile && fds->outfile[0] == '\0' && cc)
	{
		ms_msg_err(cc, NULL, "ambiguous redirect", false);
		return ;
	}
	fds->fd_out = open(fds->outfile, O_WRONLY | O_CREAT | O_APPEND, 0664);
	if (fds->fd_out == -1)
		ms_msg_err(fds->outfile, NULL, strerror(errno), false);
}

void	ms_append_parser(t_mshl *ms, t_token **aux)
{
	t_token		*tkn_process;
	t_command	*last_cmd;

	tkn_process = *aux;
	last_cmd = ms_scroll_lstcmd(ms->cmd);
	if (!ms_set_fd_struct(last_cmd))
		ms_exit_msg(ms, ERR_ALLOC, EXIT_FAILURE);
	if (!ms->ctrlcheredoc)
		ms_append_file(last_cmd->fds, tkn_process->next->content, \
	tkn_process->next->cc);
	ms_skip_next_token(aux);
}

void	ms_pipe_parser(t_minishell *ms, t_token **token_lst)
{
	t_command	*last_cmd;

	last_cmd = ms_scroll_lstcmd(ms->cmd);
	last_cmd->pipe_output = true;
	ms_addlst_cmd_container(ms, &last_cmd);
	*token_lst = (*token_lst)->next;
}

void	ms_parser_main(t_minishell *ms)
{
	t_token	*aux;

	aux = ms->token;
	if (aux->type == END)
		return ;
	while (aux->next)
	{
		if (aux == ms->token)
			ms_addlst_cmd_container(ms, &ms->cmd);
		if (aux->type == WORD || aux->type == VAR)
			ms_word_n_var_parser(ms, &aux);
		else if (aux->type == INPUT)
			ms_infile_parser(ms, &aux);
		else if (aux->type == HEREDOC)
			ms_heredoc_main(ms, &aux);
		else if (aux->type == TRUNC)
			ms_trunc_parser(ms, &aux);
		else if (aux->type == APPEND)
			ms_append_parser(ms, &aux);
		else if (aux->type == PIPE)
			ms_pipe_parser(ms, &aux);
		else if (aux->type == END)
			break ;
	}
	ms_cmd_no_arg_prccs(ms);
}