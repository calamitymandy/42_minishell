/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_init_data.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amdemuyn <amdemuyn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 17:34:24 by amdemuyn          #+#    #+#             */
/*   Updated: 2025/02/04 19:31:16 by amdemuyn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	init_main_struct(t_minishell *mini, char **env)
{
	if (!set_env_var(mini, env))
	{
		error_msg("Error", NULL, \
		"Failed to initialize environment variables", errno);
		return (false);
	}
	if (!set_pwd_n_oldpwd(mini))
	{
		error_msg("Error", NULL, \
		"Failed to initialize working directories", errno);
		return (false);
	}
	mini->line = NULL;
	mini->ctrlcheredoc = false;
	mini->token = NULL;
	mini->command = NULL;
	mini->pid = -1;
	g_status = 0;
	return (true);
}

bool	set_env_var(t_minishell *mini, char **env)
{
	if (!*env)
	{
		if (!create_env(mini))
			return (false);
	}
	else
	{
		if (!copy_environment(mini, env))
			return (false);
	}
	return (true);
}

bool	create_env(t_minishell *mini)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		free(pwd);
		return (false);
	}
	mini->env = malloc(sizeof(char *) * 4);
	if (!mini->env)
		return (false);
	mini->env[0] = ft_strjoin("PWD=", pwd);
	mini->env[1] = ft_strdup("SHLVL=1");
	mini->env[2] = ft_strdup("_=/usr/bin/env");
	if (!mini->env[0] || !mini->env[1] || !mini->env[2])
		return (false);
	mini->env[3] = NULL;
	free(pwd);
	return (true);
}

bool	set_pwd_n_oldpwd(t_minishell *mini)
{
	char	buff[1024];
	char	*working_dir;

	working_dir = getcwd(buff, 1024);
	mini->pwd = ft_strdup(working_dir);
	if (!mini->pwd)
		return (false);
	if (srch_env_i(mini->env, "OLDPWD") != -1)
	{
		mini->old_pwd = ft_strdup(get_env_value(mini->env,
					"OLDPWD"));
		if (!mini->old_pwd)
			return (false);
	}
	else
	{
		mini->old_pwd = ft_strdup(working_dir);
		if (!mini->old_pwd)
			return (false);
	}
	return (true);
}
