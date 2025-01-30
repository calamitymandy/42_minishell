/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_export_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amdemuyn <amdemuyn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 17:26:40 by amdemuyn          #+#    #+#             */
/*   Updated: 2025/01/28 17:27:28 by amdemuyn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*add_env_quotes(char *env_var)
{
	char	**split;
	char	*quoted_env_var;
	int		i;

	i = 1;
	split = ft_split(env_var, '=');
	quoted_env_var = ft_strjoin(split[0], "=\"");
	while (split[i] && split[i + 1])
	{
		quoted_env_var = strjoin_n_free(quoted_env_var, split[i++]);
		quoted_env_var = strjoin_n_free(quoted_env_var, "=");
	}
	if (split[i])
		quoted_env_var = strjoin_n_free(quoted_env_var, split[i]);
	quoted_env_var = strjoin_n_free(quoted_env_var, "\"");
	free_two_stars(split);
	return (quoted_env_var);
}

/* Used to sort environment variables in the minishell, 
where the array of strings represents the environment variables 
in the format KEY=VALUE. Sorting can make it easier to display 
or manage these variables.*/

void	qsort_env_vars(char **env, int nb_env_var)
{
	int		i;
	int		j;
	char	*aux;

	i = 0;
	while (i < nb_env_var - 1)
	{
		j = i + 1;
		while (j < nb_env_var)
		{
			if (ft_strcmp(env[i], env[j]) > 0)
			{
				aux = env[i];
				env[i] = env[j];
				env[j] = aux;
			}
			j++;
		}
		i++;
	}
}
