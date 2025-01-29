/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_echo.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amdemuyn <amdemuyn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 17:45:22 by amdemuyn          #+#    #+#             */
/*   Updated: 2025/01/28 17:46:35 by amdemuyn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Handle the special -n flag, which suppress the newline at the end of the output.
 * Ensures that a string like -n, -nn, -nnn, etc., is valid but not something 
 * like -nX or -nx
 * Mix of 2 with n_flag
*/
int	exec_echo(t_minishell *mini, char **args)
{
	int		i;
	int		j;
	bool	minus_n_flag;

	minus_n_flag = false;
	i = 1;
	while (args[i])
	{
		j = 0;
		if (args[i][j] != '-' || (args[i][j] == '-' && args[i][j + 1] != 'n'))
			break ;
		j++;
		while (args[i][j] && args[i][j] == 'n')
			j++;
		if (args[i][j] != '\0')
			break ;
		minus_n_flag = true;
		i++;
	}
	print_echo(args, minus_n_flag, i, mini);
	return (EXIT_SUCCESS);
}
