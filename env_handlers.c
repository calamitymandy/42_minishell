/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_handlers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amdemuyn <amdemuyn@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 17:31:14 by amdemuyn          #+#    #+#             */
/*   Updated: 2025/02/10 18:13:36 by amdemuyn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	copy_environment(t_minishell *ms, char **envp)
{
	int		i;

	if (*envp)
		ms->env = ft_arrdup(envp);
	modify_shlvl(ms);
	refresh_pwd_env(ms);
	if (!ms->env)
		return (false);
	i = 0;
	while (ms->env[i])
	{
		if (!ms->env[i])
			return (false);
		i++;
	}
	return (true);
}

void	refresh_pwd_env(t_minishell *mini)
{
	char	*pwd;
	char	*str;

	pwd = search_env(mini, "PWD");
	if (!pwd)
	{
		str = getcwd(NULL, 0);
		mini->env = arr_append(mini->env, str);
		free(str);
		return ;
	}
}

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
	int		i;
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

/* 
1-Invalid Position Check: If pos is greater than the total number 
of environment variables (nb_env_variables(ms->env)), the 
function returns false. This ensures the position is valid.
2-Free the Target Variable: Frees the memory of the environment 
variable at position pos using free_star().
3-Shift Remaining Variables: Starting from pos, the function 
shifts all subsequent variables one position to the left:
Copies the value from ms->env[i + 1] to ms->env[i] using ft_strdup().
Frees the memory of the old ms->env[i + 1] after copying it.
4-Update the Environment Array: After all variables are shifted,
the function resizes the environment array to remove the extra, 
unused space: Calls callocate_env_variables() to reallocate 
memory for the updated array. If this call fails 
(ms->env becomes NULL), the function returns false.
5-Return Success: If all steps are successful, the function 
returns true.
*/

bool	delete_env_var_pos(t_minishell *mini, int pos)
{
	int	i;
	int	count;

	if (pos > nb_env_variables(mini->env))
		return (false);
	i = pos;
	count = pos;
	free_star(mini->env[pos]);
	while (mini->env[i + 1])
	{
		mini->env[i] = ft_strdup(mini->env[i + 1]);
		free_star(mini->env[i + 1]);
		count++;
		i++;
	}
	mini->env = callocate_env_variables(mini, count);
	if (!mini->env)
		return (false);
	return (true);
}
