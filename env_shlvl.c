// ADD HEADER

#include "minishell.h"

void	modify_shlvl(t_minishell *mini)
{
	char	*shlvl;
	char	*new_shlvl;
	int		value;
	char	**split;

	shlvl = search_env(mini, "SHLVL");
	if (!shlvl)
	{
		mini->env = arr_append(mini->env, "SHLVL=1");
		return ;
	}
	split = ft_split(shlvl, '=');
	value = ft_atoi(split[1]);
	value++;
	shlvl = ft_itoa(value);
	new_shlvl = ft_strjoin("SHLVL=", shlvl);
	modify_or_add_env(mini, new_shlvl);
	free(shlvl);
	free(new_shlvl);
	free_two_stars(split);
}

char	*search_env(t_minishell *ms, char *env_key)
{
	int		i;
	char	**key_value;

	i = 0;
	while (ms->env[i])
	{
		key_value = ft_split(ms->env[i], '=');
		if (ft_strcmp(key_value[0], env_key) == 0)
		{
			free_two_stars(key_value);
			break ;
		}
		else
			i++;
		free_two_stars(key_value);
	}
	if (!ms->env[i])
		return (NULL);
	return (ms->env[i]);
}

char	**arr_append(char **arr, char *line)
{
	int		i;
	char	**new_arr;

	i = 0;
	while (arr[i])
		i++;
	new_arr = malloc(sizeof(char *) * (i + 2));
	if (!new_arr)
		return (NULL);
	i = 0;
	while (arr[i])
	{
		arr[i] = ft_strdup(arr[i]);
		i++;
	}
	new_arr[i++] = ft_strdup(line);
	new_arr[i] = NULL;
	free_two_stars(arr);
	return (new_arr);
}

void	modify_or_add_env(t_minishell *ms, char *line)
{
	int		i;
	char	**split_line;
	char	**split_env;

	split_line = ft_split(line, '=');
	i = 0;
	while (ms->env[i])
	{
		split_env = ft_split(ms->env[i], '=');
		if (strcmp(split_line[0], split_env[0]) == 0)
		{
			free_two_stars(split_env);
			free_two_stars(split_line);
			free(ms->env[i]);
			ms->env[i] = ft_strdup(line);
			return ;
		}
		i++;
		free_two_stars(split_env);
	}
	free_two_stars(split_line);
	ms->env[i] = ft_strdup(line);
}