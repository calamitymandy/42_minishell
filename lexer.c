#include "minishell.h"

void	ft_putendl_fd(char *s, int fd)
{
	ft_putstr_fd(s, fd);
	write(fd, "\n", 1);
}



void	ms_exit_ms(t_minishell *ms, int exit_code)
{
	if (ms)
	{
		if (ms->cmd && ms->cmd->fds)
			ms_close_fds(ms->cmd, true);
		ms_data_free(ms, true);
	}
	exit(exit_code);
}


bool	ms_is_line_empty(char *line)
{
	int	i;

	i = -1;
	while (line[++i])
		if (!ft_isblank(line[++i]))
			return (false);
	return (true);
}

void	exit_minig(t_minishell *ms, char *msg, int exit_code)
{
	ft_putendl_fd(msg, 2);
	exit_mini(ms, exit_code);
}

bool	ms_stx_error_cases(t_token *token)
{
	/*
	SPACES = 1,
	HEREDOC = 2,
	VAR = 3,
	INPUT = 4,
	WORD = 5,
	PIPE = 6,
	TRUNC = 7,
	APPEND = 8,
	FAILURE = 9,
	END = 10,
	*/
	if (token->prev)
	{
		if (token->type == PIPE && token->prev->type >= PIPE)
		{
			g_status = ERR_PIPE_STX;
			return (true);
		}
		if (token->type > PIPE && token->prev->type > PIPE)
		{
			g_status = ERR_PIPE_STX;
			return (true);
		}
		if (token->type == END && token->prev->type >= PIPE)
		{
			g_status = ERR_PIPE_STX;
			return (true);
		}
	}
	return (false);
}

t_token	*ms_tkn_create(char *content, char *cntnt_cpy, int type, int qs)
{
	t_token	*new_node;

	new_node = malloc(sizeof(t_token) * 1);
	if (!new_node)
		return (NULL);
	new_node->content = content;
	new_node->token_type = cntnt_cpy;
	new_node->is_env_var = false;
	new_node->type = type;
	new_node->var_q_stat = qs;
	new_node->has_quotes = false;
	new_node->index = 0;
	new_node->prev = NULL;
	new_node->next = NULL;
	return (new_node);
}


void	ms_add_tkn_lst(t_token **lst, t_token *new_node)
{
	t_token	*aux;

	aux = *lst;
	if (!aux)
	{
		*lst = new_node;
		return ;
	}
	if (lst && *lst && new_node)
	{
		while (aux->next)
			aux = aux->next;
		aux->next = new_node;
		new_node->prev = aux;
	}
}

int	ms_quote_stat(int quote_stat, char *line, int scan)
{
	if (line[scan] == '\'' && quote_stat == OK_Q)
		quote_stat = OPN_SQ;
	else if (line[scan] == '\"' && quote_stat == OK_Q)
		quote_stat = OPN_DQ;
	else if (line[scan] == '\'' && quote_stat == OPN_SQ)
		quote_stat = OK_Q;
	else if (line[scan] == '\"' && quote_stat == OPN_DQ)
		quote_stat = OK_Q;
	return (quote_stat);
}



bool	ms_oprtr_to_tkn(t_token **tkns, char *line, int scan, int oprtr_type)
{
	int		i;
	char	*oprtr;

	i = 0;
	if (oprtr_type == APPEND || oprtr_type == HEREDOC)
	{
		oprtr = malloc(sizeof(char) * 3);
		if (!oprtr)
			return (false);
		while (i < 2)
			oprtr[i++] = line[scan++];
		oprtr[i] = '\0';
		ms_add_tkn_lst(tkns, ms_tkn_create(oprtr, NULL, oprtr_type, OK_Q));
	}
	else
	{
		oprtr = malloc(sizeof(char) * 2);
		if (!oprtr)
			return (false);
		while (i < 1)
			oprtr[i++] = line[scan++];
		oprtr[i] = '\0';
		ms_add_tkn_lst(tkns, ms_tkn_create(oprtr, NULL, oprtr_type, OK_Q));
	}
	return (true);
}
bool	ms_word_to_tkn(t_token **tkns, char *line, int scan, int start_word)
{
	int		i;
	char	*word;

	word = malloc(sizeof(char) * (scan - start_word + 1));
	if (!word)
		return (false);
	i = 0;
	while (start_word < scan)
	{
		word[i] = line[start_word];
		start_word++;
		i++;
	}
	word[i] = '\0';
	ms_add_tkn_lst(tkns, ms_tkn_create (word, ft_strdup(word), WORD, OK_Q));
	return (true);
}


void	free_star(void *ptr)
{
	if (ptr != NULL)
	{
		free(ptr);
		ptr = NULL;
	}
}


char	*ms_strjoin(char *str1, char *str2)
{
	char	*aux;

	if (!str2)
		return (str1);
	if (!str1)
		return (ft_strdup(str2));
	aux = str1;
	str1 = ft_strjoin(aux, str2);
	free_star(aux);
	return (str1);
}

void	ms_err_stx_out(char *message, char *quote, int in_quote)
{
	char	*out;

	out = ft_strdup("minishell: ");
	out = ms_strjoin(out, message);
	if (in_quote)
		out = ms_strjoin(out, " `");
	else
		out = ms_strjoin(out, ": ");
	out = ms_strjoin(out, quote);
	if (in_quote)
		out = ms_strjoin(out, "'");
	ft_putendl_fd(out, STDERR_FILENO);
	free_star(out);
}

int	ms_chck_oprtr_type(char *line, int scan)
{
	int	size;

	size = ft_strlen(line + scan);
	if (((line[scan] > 8 && line[scan] < 14) || line[scan] == 32))
		return (SPACES);
	else if (line[scan] == '|')
		return (PIPE);
	else if (size >= 3 && line[scan] == '<' && line[scan + 1] == '<')
		return (HEREDOC);
	else if (size >= 3 && line[scan] == '>' && line[scan + 1] == '>')
		return (APPEND);
	else if (size >= 2 && line[scan] == '<')
		return (INPUT);
	else if (size >= 2 && line[scan] == '>')
		return (TRUNC);
	else if (line[scan] == '<' || line[scan] == '>')
		return (FAILURE);
	else if (line[scan] == '\0')
		return (END);
	else
		return (0);
}

int	ms_chunk_reader(int *scan, char *line, int start_word, t_minishell *ms)
{
	int	oprtr;

	oprtr = ms_chck_oprtr_type(line, (*scan));
	if (oprtr)
	{
		if ((*scan) != 0 && !ms_chck_oprtr_type(line, (*scan) - 1))
			if (!ms_word_to_tkn(&ms->token, line, (*scan), start_word))
				exit_minig(ms, ERR_ALLOC, EXIT_FAILURE);
		if (oprtr && oprtr != SPACES)
		{
			if (!ms_oprtr_to_tkn(&ms->token, line, (*scan), oprtr))
				exit_minig(ms, ERR_ALLOC, EXIT_FAILURE);
			if (oprtr == APPEND || oprtr == HEREDOC)
				(*scan)++;
		}
		start_word = (*scan) + 1;
	}
	return (start_word);
}

bool	ms_quotes_err_n_read(t_minishell *ms, char *line)
{
	int	start_word;
	int	end_line;
	int	quote_stat;
	int	scan;

	start_word = 0;
	end_line = ft_strlen(line);
	quote_stat = OK_Q;
	scan = -1;
	while (++scan <= end_line)
	{
		quote_stat = ms_quote_stat(quote_stat, line, scan);
		if (quote_stat == OK_Q)
			start_word = ms_chunk_reader(&scan, line, start_word, ms);
	}
	if (quote_stat != OK_Q)
	{
		if (quote_stat == OPN_DQ)
			ms_err_stx_out(ERR_SYNTX_QUO, "\"", true);
		else if (quote_stat == OPN_SQ)
			ms_err_stx_out(ERR_SYNTX_QUO, "\'", true);
		return (true);
	}
	return (false);
}


bool	ms_stx_err_iterator(t_token **token_list)
{
	t_token	*aux;

	aux = *token_list;
	while (aux)
	{
		if (ms_stx_error_cases(aux))
		{
			if (aux->type == END && aux->prev && aux->prev->type > PIPE)
				ms_err_stx_out(ERR_SYNTX_TKN, "newline", true);
			else if (aux->type == END && aux->prev && aux->prev->type == PIPE)
				ms_err_stx_out(ERR_SYNTX_TKN, aux->prev->content, true);
			else
				ms_err_stx_out(ERR_SYNTX_TKN, aux->content, true);
			return (true);
		}
		aux = aux->next;
	}
	return (false);
}

bool	ms_stx_err(t_token **token_list)
{
	t_token	*aux;

	aux = *token_list;
	if (aux->type == PIPE)
	{
		ms_err_stx_out(ERR_SYNTX_TKN, aux->content, true);
		g_status = ERR_PIPE_STX;
		return (true);
	}
	while (aux)
	{
		if (ms_stx_err_iterator(&aux))
			return (true);
		aux = aux->next;
	}
	return (false);
}


bool	ms_is_between_d_quot(char *content, int scan)
{
	if (scan > 0)
	{
		if (content[scan - 1] == '\"' && content[scan + 1] == '\"')
			return (true);
		else
			return (false);
	}
	return (false);
}

bool	ms_is_bad_char_next(char next)
{
	if (next == '$' || next == ' ' || next == '=' || next == '\0')
		return (true);
	else
		return (false);
}

bool	ms_dollar_error(char *content, int scan)
{
	if ((ms_is_bad_char_next(content[scan + 1])) \
		|| (ms_is_between_d_quot(content, scan)))
		return (true);
	else
		return (false);
}

void	ms_quote_stat_expndr(t_token **node, char scan)
{
	if (scan == '\'' && (*node)->var_q_stat == OK_Q)
		(*node)->var_q_stat = OPN_SQ;
	else if (scan == '\"' && (*node)->var_q_stat == OK_Q)
		(*node)->var_q_stat = OPN_DQ;
	else if (scan == '\'' && (*node)->var_q_stat == OPN_SQ)
		(*node)->var_q_stat = OK_Q;
	else if (scan == '\"' && (*node)->var_q_stat == OPN_DQ)
		(*node)->var_q_stat = OK_Q;
}

bool	ms_isalphanum_or__(char c)
{
	if (!ft_isalnum(c) && c != '_')
		return (false);
	else
		return (true);
}


int	ms_var_name_len(char *content)
{
	int		i;
	int		len;

	i = 0;
	while (content[i] != '$')
		i++;
	i++;
	if ((content[i] >= '0' && content[i] <= '9') || content[i] == '?')
		return (1);
	len = 0;
	while (content[i])
	{
		if (!ms_isalphanum_or__(content[i]))
			break ;
		len++;
		i++;
	}
	return (len);
}

void	ms_val_cpy(char *new, char *value, int *j)
{
	int	i;

	i = 0;
	while (value[i])
	{
		new[*j] = value[i];
		i++;
		(*j)++;
	}
}

char	*ms_get_var_str(char *content, char *value, int trim_len, int scan)
{
	int		i;
	int		j;
	char	*new;

	i = 0;
	j = 0;
	new = malloc(sizeof(char) * trim_len);
	if (!new)
		return (NULL);
	while (content[i])
	{
		if (content[i] == '$' && i == scan)
		{
			ms_val_cpy(new, value, &j);
			i = i + ms_var_name_len(content + scan) + 1;
			if (content[i] == '\0')
				break ;
		}
		new[j++] = content[i++];
	}
	new[j] = '\0';
	return (new);
}

char	*ms_replace_for_xpanded(t_token **aux, char *content, char *value, int scan)
{
	int		trim_len;
	char	*trim_cntnt;

	trim_len = (ft_strlen(content) - ms_var_name_len(content + scan)
			+ ft_strlen(value));
	trim_cntnt = ms_get_var_str(content, value, trim_len, scan);
	if (aux && *aux)
	{
		free_star((*aux)->content);
		(*aux)->content = trim_cntnt;
	}
	return (trim_cntnt);
}

char	*replace_str_heredoc(char *str, char *var_value, int index)
{
	char	*tmp;

	tmp = NULL;
	if (var_value == NULL)
		*str = '\0';
	else
	{
		tmp = str;
		str = ms_replace_for_xpanded(NULL, str, var_value, index);
		free_star(tmp);
	}
	free_star(var_value);
	return (str);
}


bool	ms_xpand_if_null(t_token **aux, char *content, int scan)
{
	int		i;
	int		j;
	int		trim_len;
	char	*trim_cntnt;

	trim_len = ft_strlen(content) - ms_var_name_len(content + scan);
	trim_cntnt = (char *)malloc(sizeof(char) * trim_len + 1);
	if (!trim_cntnt)
		return (false);
	i = 0;
	j = 0;
	while (content[i])
	{
		if (content[i] == '$' && i == scan)
		{
			i = i + ms_var_name_len(content + scan) + 1;
			if (content[i] == '\0')
				break ;
		}
		trim_cntnt[j++] = content[i++];
	}
	trim_cntnt[j] = '\0';
	free_star((*aux)->content);
	(*aux)->content = trim_cntnt;
	return (true);
}

void	ms_process_variables(char *value, t_token **aux, int scan, t_minishell *ms)
{
	if (!value)
	{
		if (!ms_xpand_if_null(aux, (*aux)->content, scan))
		{
			free_star(value);
			exit_minig(ms, ERR_ALLOC, EXIT_FAILURE);
		}
	}
	else
	{
		if (!ms_replace_for_xpanded(aux, (*aux)->content, value, scan))
		{
			free_star(value);
			exit_minig(ms, ERR_ALLOC, EXIT_FAILURE);
		}
	}
	free_star(value);
}

char	*ms_dup_env_var_value(t_minishell *ms, char *var_nme)
{
	char	*value;
	int		i;
	int		len;

	i = -1;
	len = ft_strlen(var_nme);
	while (ms->env[++i])
		if (ft_strncmp(ms->env[i], var_nme, len) == 0)
			break ;
	value = ft_strdup(ms->env[i] + len);
	return (value);
}





char	*ms_xtract_var_name(t_minishell *ms, char *content)
{
	char	*var_name;
	char	*tmp;
	int		start;
	int		len;
	int		i;

	i = -1;
	start = 0;
	while (content[++i])
	{
		if (content[i] == '$')
		{
			start = i + 1;
			break ;
		}
	}
	len = ms_var_name_len(content);
	var_name = ft_substr(content, start, len);
	if (!var_name)
		exit_minig(ms, ERR_ALLOC, EXIT_FAILURE);
	tmp = ft_strjoin(var_name, "=");
	free_star(var_name);
	var_name = tmp;
	return (var_name);
}

bool	ms_is_env_var(t_minishell *ms, char *var_nme)
{
	int		i;
	int		len;

	i = -1;
	len = ft_strlen(var_nme);
	while (ms->env[++i])
		if (ft_strncmp(ms->env[i], var_nme, len) == 0)
			return (true);
	return (false);
}

char	*ms_xtract_var_value(t_token *token, char *content, t_minishell *ms)
{
	char	*value;
	char	*var_nme;

	var_nme = ms_xtract_var_name(ms, content);
	if (var_nme && ms_is_env_var(ms, var_nme))
	{
		if (token)
			token->is_env_var = true;
		value = ms_dup_env_var_value(ms, var_nme);
	}
	else if (var_nme && var_nme[0] == '?' && var_nme[1] == '=')
		value = ft_itoa(g_status);
	else
		value = NULL;
	free_star(var_nme);
	return (value);
}

void	ms_scan_variables(t_minishell *ms)
{
	t_token	*aux;
	int		scan;

	aux = ms->token;
	while (aux)
	{
		if (aux->type == VAR)
		{
			scan = 0;
			while (aux->content[scan])
			{
				ms_quote_stat_expndr(&aux, aux->content[scan]);
				if ((aux->content[scan] == '$' \
				&& !ms_dollar_error(aux->content, scan)) \
				&& (aux->var_q_stat == OK_Q || aux->var_q_stat == OPN_DQ))
					ms_process_variables(ms_xtract_var_value \
					(aux, aux->content + scan, ms), &aux, scan, ms);
				else
					scan++;
			}
		}
		aux = aux->next;
	}
}

bool	ms_quote_detector(char *str)
{
	int	i;

	i = -1;
	while (str[++i])
		if (str[i] == '\'' || str[i] == '\"')
			return (true);
	return (false);
}

bool	ms_skip_quot_n_close(t_token **aux, int *pos)
{
	if (((*aux)->content[*pos] == '\'' && (*aux)->var_q_stat == OPN_SQ)
		|| ((*aux)->content[*pos] == '\"' \
		&& (*aux)->var_q_stat == OPN_DQ))
	{
		(*aux)->var_q_stat = OK_Q;
		(*pos)++;
		return (true);
	}
	else
		return (false);
}

void	ms_skip_quot_n_open(t_token **aux, int *pos)
{
	if ((*aux)->content[*pos] == '\'')
		(*aux)->var_q_stat = OPN_SQ;
	if ((*aux)->content[*pos] == '\"')
		(*aux)->var_q_stat = OPN_DQ;
	(*pos)++;
}

bool	ms_is_quote_here(t_token **aux, int pos)
{
	if (((*aux)->content[pos] == '\'' || (*aux)->content[pos] == '\"')
		&& (*aux)->var_q_stat == OK_Q)
		return (true);
	else
		return (false);
}

int	ms_len_wthout_quot(char *content, int len)
{
	int	status;
	int	i;

	status = OK_Q;
	i = 0;
	while (content[i])
	{
		if ((content[i] == '\'' || content[i] == '\"') && status == OK_Q)
		{
			if (content[i] == '\'')
				status = OPN_SQ;
			if (content[i] == '\"')
				status = OPN_DQ;
			i++;
		}
		else if ((content[i] == '\'' && status == OPN_SQ)
			|| (content[i] == '\"' && status == OPN_DQ))
		{
			status = OK_Q;
			i++;
		}
		len++;
		i++;
	}
	return (len);	
}

void	ms_quote_eraser(t_token **aux)
{
	int		i;
	int		j;
	char	*noq_str;

	i = 0;
	j = 0;
	noq_str = malloc(sizeof(char) * ms_len_wthout_quot((*aux)->content, i) + 1);
	while ((*aux)->content[i])
	{
		if (ms_is_quote_here(aux, i))
			ms_skip_quot_n_open(aux, &i);
		else if (ms_skip_quot_n_close(aux, &i))
			continue ;
		else
			noq_str[j++] = (*aux)->content[i++];
	}
	noq_str[j] = '\0';
	free_star((*aux)->content);
	(*aux)->content = noq_str;
	(*aux)->has_quotes = true;
}

int	ms_process_quotes(t_minishell *ms)
{
	t_token	*aux;

	aux = ms->token;
	while (aux)
	{
		if (ms_quote_detector(aux->content) \
			&& (aux->prev == NULL || (aux->prev && aux->prev->type != HEREDOC)))
			ms_quote_eraser(&aux);
		aux = aux->next;
	}
	return (0);
}

void	ms_mark_variables(t_minishell *ms)
{
	t_token	*aux;
	int		scan;

	aux = ms->token;
	while (aux)
	{
		scan = -1;
		while ((aux)->content[++scan])
		{
			if ((aux)->content[scan] == '$')
			{
				if ((aux)->prev && (aux)->prev->type == HEREDOC)
					break ;
				(aux)->type = VAR;
			}
		}
		aux = aux->next;
	}
}


void	ms_expander_main(t_minishell *ms)
{
	ms_mark_variables(ms);
	ms_scan_variables(ms);
	ms_process_quotes(ms);
}

void	ms_token_indx(t_mshl *ms)
{
	t_token			*aux;
	unsigned int	i;

	aux = ms->token;
	i = 0;
	while (aux->next)
	{
		aux->index += i; // why "+="? where are the previous indexes coming from?
		i++;
		aux = aux->next;
	}
}

void	ms_addlst_cmd_container(t_mshl *ms, t_command **cmd_list)
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

bool	ms_lexer_main(t_minishell *ms)
{
	if (!ms->line)
		exit_mini(ms, 0); // TODO: Use built-in exit instead?? // Error code?
	else if (ms_is_line_empty(ms->line))
		return (true);
	add_history(ms->line);
	if (ms_quotes_err_n_read(ms, ms->line))
		return (false);
	if (ms_stx_err(&ms->token))
		return (false);
	ms_expander_main(ms);
	ms_token_indx(ms);
	ms_parser_main(ms);
	return (true);
}
