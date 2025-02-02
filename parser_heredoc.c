#include"minishell.h"

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

void	quit_newline_char(char *line)
{
	size_t	len;

	len = ft_strlen(line);
	if (len > 0 && line[len - 1] == '\n')
		line[len - 1] = '\0';
}

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

	/*if (tmp_fd == -1)//🚨 Added: HEREDOC should not run if fd_infile is invalid
	{
		return(false);
	}*/
	success = false;
	line = NULL;
	while (1)
	{
		listening_hdoc_input_sig();
		ft_putstr_fd("> ", 1);
		line = get_next_line(STDIN_FILENO); 
		listening_no_interact_sig();
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


bool	fds_error(t_fds *fds)
{
	if (fds->infile)
	{
		if (fds->fd_infile == -1 || (fds->outfile && fds->fd_outfile == -1))
		{
			return (true);
		}
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
	/*printf("del_heredoc: %s\n",fds->del_heredoc);
	printf("fd infile %i\n",fds->fd_infile);
	printf("fd_outfile %i\n",fds->fd_outfile);
	printf("infile %s\n", fds->infile);
	printf("outfile %s\n", fds->outfile);
	printf("sdin_ori %i\n",fds->stdin_ori);
	printf("stdout ori %i\n",fds->stdout_ori);*/
	/*if (fds_error(fds))
	{
		skip_next_token(aux);
		return;
	}*/
	fds->infile = put_name_tmp();
	if (!fds->infile)
		exit_msg(ms, ERR_ALLOC, EXIT_FAILURE);
	fds->del_heredoc = quit_heredoc_quot(pre_delim->next->content, \
	& (fds->heredoc_quotes));
	if (create_tmp(ms, fds))
	{	
		fds->fd_infile = open(fds->infile, O_RDONLY);
	}
	else
	{
		fds->fd_infile = -1;
	}
	skip_next_token(aux);
}
