/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_env_exit.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amdemuyn <amdemuyn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 17:23:10 by amdemuyn          #+#    #+#             */
/*   Updated: 2025/01/28 17:27:15 by amdemuyn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Implements the env built-in command, which prints the current 
 * environment variables.
 * args: An array of strings representing the arguments passed to the 
 * env command.
 * 1- Check for too many arguments. The env command does not accept 
 * any arguments. If args[1] exists (more than one argument was passed)
 * the function returns an error message and 2, a standard error code 
 * for invalid usage in shell commands.
 * 2- Check if the environment exists, if not it returns EXIT_FAILURE.
 * 3- A loop iterates through the mini->env array (which contains the
 *  environment variables).For each environment variable, prints the 
 * variable with a newline to the standard output (STDOUT_FILENO).
 */

int	exec_env_builtin(t_minishell *mini, char **args)
{
	int	i;

	if (args && args[1])
		return (error_msg("env", NULL, "Error: too many arguments", 2));
	i = 0;
	if (!mini->env)
		return (EXIT_FAILURE);
	while (mini->env[i])
		ft_putendl_fd(mini->env[i++], STDOUT_FILENO);
	return (EXIT_SUCCESS);
}

int	get_exit_code(char *arg, bool *error)
{
	unsigned long long	i;

	if (!arg)
		return (g_status);
	i = 0;
	while (ft_isblank(arg[i]))
		i++;
	if (arg[i] == '\0')
		*error = true;
	if (arg[i] == '-' || arg[i] == '+')
		i++;
	if (!ft_isdigit(arg[i]))
		*error = true;
	while (arg[i])
	{
		if (!ft_isdigit(arg[i]) && !ft_isblank(arg[i]))
			*error = true;
		i++;
	}
	i = ft_atoi_long(arg, error);
	return (i % 256);
}

int	exec_exit_builtin(t_minishell *mini, char **args)
{
	int		exit_code;
	bool	error;

	error = false;
	if (!args || !args[1])
		exit_code = g_status;
	else
	{
		exit_code = get_exit_code(args[1], &error);
		if (error)
			exit_code = error_msg("exit", args[1],
					"numeric argument required", 2);
		else if (args[2])
			return (error_msg("exit", NULL, "too many arguments", 1));
	}
	exit_mini(mini, exit_code);
	return (2);
}
