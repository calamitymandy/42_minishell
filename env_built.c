// ADD HEADER

#include "minishell.h"

int	nb_env_variables(char **env)
{
	int	i;

	i = 0;
	while (env && env[i])
		i++;
	return (i);
}

int	srch_env_i(char **env, char *pwd_or_old)
{
	int		i;
	char	*temp;

	temp = ft_strjoin(pwd_or_old, "=");
	if (!temp)
		return (-1);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(temp, env[i], ft_strlen(temp)) == 0)
		{
			free_star(temp);
			return (i);
		}
		i++;
	}
	free_star(temp);
	return (-1);
}

char	*get_env_value(char **env, char *key)
{
	int		i;
	char	*temp;

	temp = ft_strjoin(key, "=");
	if (!temp)
		return (NULL);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(temp, env[i], ft_strlen(temp)) == 0)
		{
			free_star(temp);
			return (ft_strchr(env[i], '=') + 1);
		}
		i++;
	}
	free_star(temp);
	return (NULL);
}

bool	valid_env_key(char *key)
{
	int	i;
	
	i = 0;
	if (key == NULL || key[0] == '\0')
		return (false);
	if (ft_isalpha(key[i]) == 0 && key[i] != '_')
		return (false);
	i++;
	while (key[i] && key[i] != '=')
	{
		if (ft_isalnum(key[i]) == 0 && key[i] != '_')
			return (false);
		i++;
	}
	return (true);
}