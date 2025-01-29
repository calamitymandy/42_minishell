#include "minishell.h"

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

t_token	*tkn_create(char *content, char *cntnt_cpy, int type, int qs)
{
	t_token	*new_node;

	new_node = malloc(sizeof(t_token) * 1);
	if (!new_node)
		return (NULL);
	new_node->content = content;
	new_node->cc = cntnt_cpy;
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
			start_word = chunk_reader(&scan, line, start_word, ms);
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
