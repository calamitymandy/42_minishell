#include "minishell.h"

void	ft_putendl_fd(char *s, int fd)
{
	ft_putstr_fd(s, fd);
	write(fd, "\n", 1);
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


bool	ms_lexer_main(t_minishell *ms)
{
	if (!ms->line)
		exit_mini(ms, NULL); //Use exit built_in instead??
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
