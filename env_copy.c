/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_copy.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amdemuyn <amdemuyn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 18:40:11 by amdemuyn          #+#    #+#             */
/*   Updated: 2025/02/04 19:24:46 by amdemuyn         ###   ########.fr       */
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
