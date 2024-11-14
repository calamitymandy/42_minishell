#include "minishell.h"

void	ft_putendl_fd(char *s, int fd)
{
	ft_putstr_fd(s, fd);
	write(fd, "\n", 1);
}


bool	is_line_empty(char *line)
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

bool	stx_error_cases(t_token *token)
{

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


char	*strjoin(char *str1, char *str2)
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

void	err_stx_out(char *message, char *quote, int in_quote)
{
	char	*out;

	out = ft_strdup("minishell: ");
	out = strjoin(out, message);
	if (in_quote)
		out = strjoin(out, " `");
	else
		out = strjoin(out, ": ");
	out = strjoin(out, quote);
	if (in_quote)
		out = strjoin(out, "'");
	ft_putendl_fd(out, STDERR_FILENO);
	free_star(out);
}

bool	stx_err_iterator(t_token **token_list)
{
	t_token	*aux;

	aux = *token_list;
	while (aux)
	{
		if (stx_error_cases(aux))
		{
			if (aux->type == END && aux->prev && aux->prev->type > PIPE)
				err_stx_out(ERR_SYNTX_TKN, "newline", true);
			else if (aux->type == END && aux->prev && aux->prev->type == PIPE)
				err_stx_out(ERR_SYNTX_TKN, aux->prev->content, true);
			else
				err_stx_out(ERR_SYNTX_TKN, aux->content, true);
			return (true);
		}
		aux = aux->next;
	}
	return (false);
}

bool	stx_err(t_token **token_list)
{
	t_token	*aux;

	aux = *token_list;
	if (aux->type == PIPE)
	{
		err_stx_out(ERR_SYNTX_TKN, aux->content, true);
		g_status = ERR_PIPE_STX;
		return (true);
	}
	while (aux)
	{
		if (stx_err_iterator(&aux))
			return (true);
		aux = aux->next;
	}
	return (false);
}


bool	is_between_d_quot(char *content, int scan)
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

bool	is_bad_char_next(char next)
{
	if (next == '$' || next == ' ' || next == '=' || next == '\0')
		return (true);
	else
		return (false);
}

bool	dollar_error(char *content, int scan)
{
	if ((is_bad_char_next(content[scan + 1])) \
		|| (is_between_d_quot(content, scan)))
		return (true);
	else
		return (false);
}

void	quote_stat_expndr(t_token **node, char scan)
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

bool	isalphanum_or__(char c)
{
	if (!ft_isalnum(c) && c != '_')
		return (false);
	else
		return (true);
}


int	var_name_len(char *content)
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
		if (!isalphanum_or__(content[i]))
			break ;
		len++;
		i++;
	}
	return (len);
}

void	val_cpy(char *new, char *value, int *j)
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

char	*get_var_str(char *content, char *value, int trim_len, int scan)
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
			val_cpy(new, value, &j);
			i = i + var_name_len(content + scan) + 1;
			if (content[i] == '\0')
				break ;
		}
		new[j++] = content[i++];
	}
	new[j] = '\0';
	return (new);
}

char	*replace_for_xpanded(t_token **aux, char *content, char *value, int scan)
{
	int		trim_len;
	char	*trim_cntnt;

	trim_len = (ft_strlen(content) - var_name_len(content + scan)
			+ ft_strlen(value));
	trim_cntnt = get_var_str(content, value, trim_len, scan);
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
		str = replace_for_xpanded(NULL, str, var_value, index);
		free_star(tmp);
	}
	free_star(var_value);
	return (str);
}


bool	xpand_if_null(t_token **aux, char *content, int scan)
{
	int		i;
	int		j;
	int		trim_len;
	char	*trim_cntnt;

	trim_len = ft_strlen(content) - var_name_len(content + scan);
	trim_cntnt = (char *)malloc(sizeof(char) * trim_len + 1);
	if (!trim_cntnt)
		return (false);
	i = 0;
	j = 0;
	while (content[i])
	{
		if (content[i] == '$' && i == scan)
		{
			i = i + var_name_len(content + scan) + 1;
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

void	process_variables(char *value, t_token **aux, int scan, t_minishell *ms)
{
	if (!value)
	{
		if (!xpand_if_null(aux, (*aux)->content, scan))
		{
			free_star(value);
			exit_minig(ms, ERR_ALLOC, EXIT_FAILURE);
		}
	}
	else
	{
		if (!replace_for_xpanded(aux, (*aux)->content, value, scan))
		{
			free_star(value);
			exit_minig(ms, ERR_ALLOC, EXIT_FAILURE);
		}
	}
	free_star(value);
}

char	*dup_env_var_value(t_minishell *ms, char *var_nme)
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





char	*xtract_var_name(t_minishell *ms, char *content)
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
	len = var_name_len(content);
	var_name = ft_substr(content, start, len);
	if (!var_name)
		exit_minig(ms, ERR_ALLOC, EXIT_FAILURE);
	tmp = ft_strjoin(var_name, "=");
	free_star(var_name);
	var_name = tmp;
	return (var_name);
}

bool	is_env_var(t_minishell *ms, char *var_nme)
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

char	*xtract_var_value(t_token *token, char *content, t_minishell *ms)
{
	char	*value;
	char	*var_nme;

	var_nme = xtract_var_name(ms, content);
	if (var_nme && is_env_var(ms, var_nme))
	{
		if (token)
			token->is_env_var = true;
		value = dup_env_var_value(ms, var_nme);
	}
	else if (var_nme && var_nme[0] == '?' && var_nme[1] == '=')
		value = ft_itoa(g_status);
	else
		value = NULL;
	free_star(var_nme);
	return (value);
}

void	scan_variables(t_minishell *ms)
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
				quote_stat_expndr(&aux, aux->content[scan]);
				if ((aux->content[scan] == '$' \
				&& !dollar_error(aux->content, scan)) \
				&& (aux->var_q_stat == OK_Q || aux->var_q_stat == OPN_DQ))
					process_variables(xtract_var_value \
					(aux, aux->content + scan, ms), &aux, scan, ms);
				else
					scan++;
			}
		}
		aux = aux->next;
	}
}

bool	quote_detector(char *str)
{
	int	i;

	i = -1;
	while (str[++i])
		if (str[i] == '\'' || str[i] == '\"')
			return (true);
	return (false);
}

bool	skip_quot_n_close(t_token **aux, int *pos)
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

void	skip_quot_n_open(t_token **aux, int *pos)
{
	if ((*aux)->content[*pos] == '\'')
		(*aux)->var_q_stat = OPN_SQ;
	if ((*aux)->content[*pos] == '\"')
		(*aux)->var_q_stat = OPN_DQ;
	(*pos)++;
}

bool	is_quote_here(t_token **aux, int pos)
{
	if (((*aux)->content[pos] == '\'' || (*aux)->content[pos] == '\"')
		&& (*aux)->var_q_stat == OK_Q)
		return (true);
	else
		return (false);
}

int	len_wthout_quot(char *content, int len)
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

void	quote_eraser(t_token **aux)
{
	int		i;
	int		j;
	char	*noq_str;

	i = 0;
	j = 0;
	noq_str = malloc(sizeof(char) * len_wthout_quot((*aux)->content, i) + 1);
	while ((*aux)->content[i])
	{
		if (is_quote_here(aux, i))
			skip_quot_n_open(aux, &i);
		else if (skip_quot_n_close(aux, &i))
			continue ;
		else
			noq_str[j++] = (*aux)->content[i++];
	}
	noq_str[j] = '\0';
	free_star((*aux)->content);
	(*aux)->content = noq_str;
	(*aux)->has_quotes = true;
}

int	process_quotes(t_minishell *ms)
{
	t_token	*aux;

	aux = ms->token;
	while (aux)
	{
		if (quote_detector(aux->content) \
			&& (aux->prev == NULL || (aux->prev && aux->prev->type != HEREDOC)))
			quote_eraser(&aux);
		aux = aux->next;
	}
	return (0);
}

void	mark_variables(t_minishell *ms)
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


void	expander_main(t_minishell *ms)
{
	mark_variables(ms);
	scan_variables(ms);
	process_quotes(ms);
}

void	token_indx(t_minishell *ms)
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


bool	lexer_main(t_minishell *ms)
{
	if (!ms->line)
		exit_ms(ms, 0); // TODO: Exec exit
	else if (is_line_empty(ms->line))
		return (true);
	add_history(ms->line);
	if (quotes_err_n_read(ms, ms->line))
		return (false);
	if (stx_err(&ms->token))
		return (false);
	expander_main(ms);
	token_indx(ms);
	parser_main(ms);
	return (true);
}
