#include"minishell.h"



bool	is_export_or_unset(char *cmd)
{
	if (ft_strncmp(cmd, "export", 7) == 0
		|| ft_strncmp(cmd, "unset", 6) == 0)
		return (true);
	return (false);
}

void	exit_msg(t_minishell *ms, char *msg, int exit_code)
{
	ft_putendl_fd(msg, 2);
	exit_mini(ms, exit_code);
}

void	exit_mini(t_minishell *mini, int exit_code)
{
	if (mini)
	{
		if (mini->command && mini->command->fds)
			close_fds(mini->command, true);
		clean_data(mini, true);
	}
	exit(exit_code);
}

int	msg_err(char *cc, char *info, char *msg, int error_code)
{
	char	*out;
	bool	exprt_unset;

	exprt_unset = is_export_or_unset(cc);
	out = ft_strdup("minishell: ");
	if (cc)
	{
		out = strjoin(out, cc);
		out = strjoin(out, ": ");
	}
	if (info)
	{
		if (exprt_unset)
			out = strjoin(out, "`");
		out = strjoin(out, info);
		if (exprt_unset)
			out = strjoin(out, "'");
		out = strjoin(out, ": ");
	}
	out = strjoin(out, msg);
	ft_putendl_fd(out, STDERR_FILENO);
	free_star(out);
	return (error_code);
}