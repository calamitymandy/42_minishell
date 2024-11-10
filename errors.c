#include"minishell.h"



bool	ms_is_export_or_unset(char *command)
{
	if (ft_strncmp(command, "export", 7) == 0
		|| ft_strncmp(command, "unset", 6) == 0)
		return (true);
	return (false);
}

void	ms_exit_msg(t_minishell *ms, char *msg, int exit_code)
{
	ft_putendl_fd(msg, 2);
	ms_exit_ms(ms, exit_code);
}

void	ms_exit_ms(t_minishell *ms, int exit_code)
{
	if (ms)
	{
		if (ms->command && ms->command->fds)
			ms_close_fds(ms->command, true);
		ms_data_free(ms, true);
	}
	exit(exit_code);
}

int	ms_msg_err(char *cc, char *info, char *msg, int error_code)
{
	char	*out;
	bool	exprt_unset;

	exprt_unset = ms_is_export_or_unset(cc);
	out = ft_strdup("AnShellmo: ");
	if (cc)
	{
		out = ms_strjoin(out, cc);
		out = ms_strjoin(out, ": ");
	}
	if (info)
	{
		if (exprt_unset)
			out = ms_strjoin(out, "`");
		out = ms_strjoin(out, info);
		if (exprt_unset)
			out = ms_strjoin(out, "'");
		out = ms_strjoin(out, ": ");
	}
	out = ms_strjoin(out, msg);
	ft_putendl_fd(out, STDERR_FILENO);
	free_star(out);
	return (error_code);
}