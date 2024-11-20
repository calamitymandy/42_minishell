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

void	exit_and_msg(t_minishell *ms, char *msg, int exit_code)
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
		exec_exit_builtin(ms, NULL);
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
