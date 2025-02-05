#include "minishell.h"

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

void	quit_newline_char(char *line)
{
	size_t	len;

	len = ft_strlen(line);
	if (len > 0 && line[len - 1] == '\n')
		line[len - 1] = '\0';
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
