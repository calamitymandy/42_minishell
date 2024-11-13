#include"minishell.h"

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