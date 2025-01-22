/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_export_unset.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amdemuyn <amdemuyn@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 17:56:56 by amdemuyn          #+#    #+#             */
/*   Updated: 2025/01/22 22:24:11 by amdemuyn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* For ex, if the command is export VAR=value, 
then args would be something like ["export", "VAR=value", NULL]
1- Initialization: res is set to EXIT_SUCCESS initially & 
i starts at 1 to skip the command name (args[0]).

2-Check for No Arguments:
If args[1] is NULL, it means no arguments were passed si it calls 
export_builtin(mini) to display all exported variables) and returns
its result.

3-Iterating Over Arguments: The while loop processes each argument.

4-Validation of Environment Key:
checks if the current argument (args[i]) is a valid environment 
variable name using valid_env_key().
If invalid: error_msg() & res is updated to EXIT_FAILURE.

5-Handling Key-Value Pairs:
If the argument contains = (detected using ft_strchr()): split it
into a key-value pair using key_value_arr(). The key-value pair 
is added or updated in the environment using add_or_update_env_var().
The temporary array (arr) is freed using free_two_stars().

6-Loop Increment:
i is incremented to process the next argument.

7-Return Value:
After processing all arguments, it returns res, which indicates 
the overall success (EXIT_SUCCESS) or failure (EXIT_FAILURE)
*/

int	exec_export_builtin(t_minishell *mini, char **args)
{
	int		i;
	char	**arr;
	int		res;

	res = EXIT_SUCCESS;
	i = 1;
	if (!args[i])
		return (export_builtin(mini));
	while (args[i])
	{
		if (!valid_env_key(args[i]))
		{
			error_msg("export", args[i], "not a valid identifier", \
			false);
			res = EXIT_FAILURE;
		}
		else if (ft_strchr(args[i], '=') != NULL)
		{
			arr = key_value_arr(args[i]);
			add_or_update_env_var(mini, arr[0], arr[1]);
			free_two_stars(arr);
		}
		i++;
	}
	return (res);
}

char **key_value_arr(char *key)
{
	char	**arr;
	char	*equal;

	equal = ft_strchr(key, '=');
	if (!equal)
		return (NULL);
	arr = malloc(sizeof(char *) * 3);
	arr[0] = ft_substr(key, 0, equal - key);
	arr[1] = ft_substr(equal, 1, ft_strlen(equal));
	arr[2] = NULL;
	return (arr);
}

/* 
1-Check for Null Environment: If mini->env is NULL return EXIT_FAILURE.
2-Count Environment Variables: Total number of environment variables 
in mini->env is found using nb_env_variables(mini->env).
3-Sort Environment Variables: The array of environment variables is 
sorted alphabetically using qsort_env_vars(mini->env, nb_env_var).
This ensures a consistent and organized display of variables.
4-Display Environment Variables: while loop iterates over each 
environment variable.
For each variable:
add_env_quotes() is called to add necessary quotes around the 
variable's value for proper formatting. The variable is then printed 
to the console with the format declare -x <variable>.
ex:
"PATH=/usr/bin" becomes "declare -x PATH=\"/usr/bin\""
"USER=john" becomes "declare -x USER=\"john\""
Prints:
declare -x PATH="/usr/bin"
declare -x USER="john"
5-Return Success: After successfully processing and displaying all 
variables, the function returns EXIT_SUCCESS to indicate success.*/

int	export_builtin(t_minishell *mini)
{
	char 	*env_quotes;
	int		i;
	int		nb_env_var;

	i = 0;
	if (!mini->env)
		return (EXIT_FAILURE);
	nb_env_var = nb_env_variables(mini->env);
	qsort_env_vars(mini->env, nb_env_var);
	i = 0;
	while (i < nb_env_var)
	{
		env_quotes = add_env_quotes(mini->env[i]);
		printf("declare -x %s\n", env_quotes);
		i++;
	}
	return (EXIT_SUCCESS);
}

/*
1-Initialization: res is set to EXIT_SUCCESS & i starts at 1
to skip the command name (args[0]).
2-Iterate Over Arguments from args[1] with while loop
3-Validation: valid_env_key() checks if the current argument 
(args[i]) is a valid environment variable name.
Ensures the argument does not contain an = (invalid for unset)
using ft_strchr(args[i], '=').
If argument is invalid: error_msg() & res updated to EXIT_FAILURE.
4-Search and Delete:
If the argument is valid: Searches for the env var in the array 
using srch_env_i() & returns the index if found, or -1 if not.
If the variable exists (pos != -1) delete_env_var_pos() delete it.
5-Return Value: After processing all arguments, the function 
returns res, indicating success or failure.
*/

int 	exec_unset_builtin(t_minishell *mini, char **args)
{
	int i;
	int pos;
	int	res;

	res = EXIT_SUCCESS;
	i = 1;
	while (args[i])
	{
		if (!valid_env_key(args[i]) || ft_strchr(args[i], '=') != NULL)
		{
			error_msg("unset", args[i], "not a valid identifier", \
				false);
				res = EXIT_FAILURE;
		}
		else
		{
			pos = srch_env_i(mini->env, args[i]);
			if (pos != -1)
				delete_env_var_pos(mini, pos);
		}
		i++;
	}
	return (res);
}
