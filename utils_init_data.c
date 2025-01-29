/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_init_data.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amdemuyn <amdemuyn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 17:34:24 by amdemuyn          #+#    #+#             */
/*   Updated: 2025/01/28 17:36:37 by amdemuyn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
