#include"minishell.h"

int	quote_stat(int quote_stat, char *line, int scan)
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

int	chck_oprtr_type(char *line, int scan)
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

t_token	*tkn_create(char *content, char *cntnt_cpy, int type, int qs)
{
	t_token	*new_node;

	new_node = malloc(sizeof(t_token) * 1);
	if (!new_node)
		return (NULL);
	new_node->content = content;
	new_node->cc = cntnt_cpy;
	//new_node->token_type = NULL;
	new_node->is_env_var = false;
	new_node->type = type;
	new_node->var_q_stat = qs;
	new_node->has_quotes = false;
	new_node->index = 0;
	new_node->prev = NULL;
	new_node->next = NULL;
	return (new_node);
}
void	add_tkn_lst(t_token **lst, t_token *new_node)
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
bool	word_to_tkn(t_token **tkns, char *line, int scan, int start_word)
{
	int		i;
	char	*word;

	word = malloc(sizeof(char) * (scan - start_word + 1));
	if (!word)
	{
		return (false);
	}
	i = 0;
	while (start_word < scan)
	{
		word[i] = line[start_word];
		start_word++;
		i++;
	}
	word[i] = '\0';
	add_tkn_lst(tkns, tkn_create (word, ft_strdup(word), WORD, OK_Q));
	return (true);
}

bool	oprtr_to_tkn(t_token **tkns, char *line, int scan, int oprtr_type)
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
		add_tkn_lst(tkns, tkn_create(oprtr, NULL, oprtr_type, OK_Q));
	}
	else
	{
		oprtr = malloc(sizeof(char) * 2);
		if (!oprtr)
			return (false);
		while (i < 1)
			oprtr[i++] = line[scan++];
		oprtr[i] = '\0';
		add_tkn_lst(tkns, tkn_create(oprtr, NULL, oprtr_type, OK_Q));
	}
	return (true);
}

int	chunk_reader(int *scan, char *line, int start_word, t_minishell *ms)
{
	int	oprtr;

	oprtr = chck_oprtr_type(line, (*scan));
	if (oprtr)
	{
		if ((*scan) != 0 && !chck_oprtr_type(line, (*scan) - 1))
			if (!word_to_tkn(&ms->token, line, (*scan), start_word))
				exit_and_msg(ms, ERR_ALLOC, EXIT_FAILURE);
		if (oprtr && oprtr != SPACES)
		{
			if (!oprtr_to_tkn(&ms->token, line, (*scan), oprtr))
				exit_and_msg(ms, ERR_ALLOC, EXIT_FAILURE);
			if (oprtr == APPEND || oprtr == HEREDOC)
				(*scan)++;
		}
		start_word = (*scan) + 1;
	}
	return (start_word);
}



bool	quotes_err_n_read(t_minishell *ms, char *line)
{
	int	start_word;
	int	end_line;
	int	quote_status;
	int	scan;

	start_word = 0;
	end_line = ft_strlen(line);
	quote_status = OK_Q;
	scan = -1;
	while (++scan <= end_line) 
	{
		quote_status = quote_stat(quote_status, line, scan);
		if (quote_status == OK_Q)
		{	
			start_word = chunk_reader(&scan, line, start_word, ms);
		}
	}
	if (quote_status != OK_Q)
	{
		if (quote_status == OPN_DQ)
			err_stx_out(ERR_SYNTX_QUO, "\"", true);
		else if (quote_status == OPN_SQ)
			err_stx_out(ERR_SYNTX_QUO, "\'", true);
		return (true);
	}
	return (false);
}