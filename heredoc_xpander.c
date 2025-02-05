#include "minishell.h"

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
