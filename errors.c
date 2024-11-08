#include"minishell.h"


char	*ms_strjoin(char *str1, char *str2)
{
	char	*aux;

	if (!str2)
		return (str1);
	if (!str1)
		return (ft_strdup(str2));
	aux = str1;
	str1 = ft_strjoin(aux, str2);
	ms_ptr_free(aux);
	return (str1);
}

bool	ms_is_export_or_unset(char *command)
{
	if (ft_strncmp(command, "export", 7) == 0
		|| ft_strncmp(command, "unset", 6) == 0)
		return (true);
	return (false);
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
	ms_ptr_free(out);
	return (error_code);
}