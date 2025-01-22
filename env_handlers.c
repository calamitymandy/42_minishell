// ADD HEADER

#include "minishell.h"

char	**callocate_env_variables(t_minishell *mini, int size)
{
	char	**new_env;
	int		i;
	
	new_env = ft_calloc(size + 1, sizeof * new_env);
	if (!new_env)
		return (NULL);
	i = 0;
	while (mini->env[i] && i < size)
	{
		new_env[i] = ft_strdup(mini->env[i]);
		free_star(mini->env[i]);
		i++;
	}
	free(mini->env);
	return (new_env);
}

/* Updates an existing environment variable (like PWD or OLDPWD) or adds a 
 * new one if it doesn't exist:
 * 1- Search for the index of environment variable
 * 2- joins the "=" with the value string to form the value part of the 
 * 	environment variable (e.g., =new_path). This will later be used to 
 * 	construct the full environment variable string.
 * 3- IF the environment variable was found (i != -1), it frees mini->env[i]
 * 	Then creates a new string joining the variable name (pwd_or_old) 
 * 	with the temp string (=value) and stores the result in mini->env[i].
 * 4- ELSE (i == -1), it calculates the number of environment variables 
 * 	using nb_env_variables(mini->env). Reallocates memory to accommodate 
 * 	the new environment variable.
 * 	& Adds the new environment variable to the end of the mini->env array
 * 	by joining the variable name (pwd_or_old) with temp and storing the result.
*/
bool	add_or_update_env_var(t_minishell *mini, char *pwd_or_old, char *value)
{
	int 	i;
	char	*temp;

	i = srch_env_i(mini->env, pwd_or_old);
	if (value == NULL)
		value = "";
	temp = ft_strjoin("=", value);
	if (!temp)
		return (false);
	if (i != -1 && mini->env[i])
	{
		free_star(mini->env[i]);
		mini->env[i] = ft_strjoin(pwd_or_old, temp);
	}
	else
	{
		i = nb_env_variables(mini->env);
		mini->env = callocate_env_variables(mini, i + 1);
		if (!mini->env)
			return (false);
		mini->env[i] = ft_strjoin(pwd_or_old, temp);
	}
	free_star(temp);
	return (true);
}