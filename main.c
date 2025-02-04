/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amdemuyn <amdemuyn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 20:24:50 by amdemuyn          #+#    #+#             */
/*   Updated: 2025/02/04 18:01:31 by amdemuyn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*compile with gcc main.c -lreadline*/

int	g_status;

/* This function executes the processed command and returns a status code 
 * that updates g_status. 
 * 
 * The 2nd if block checks if the command should be executed directly, 
 * without piping or chaining to other commands:
 * 1 - Ensures that command is not sending output through a pipe.
 * 2 - Ensures that command is not part of a pipeline or a sequence of commands.
 * 3 - checks if fds input & output are valid, meaning no pb with redirections.
 * 
 * config_in_and_out and reset_fds_in_and_out are necessary to handle input/output 
 * redirections (ex: echo "Hello" > output.txt) they save & restore the 
 * original fds (STDIN & STDOUT) when needed, and so ensure each command starts
 * with a clean input/output state.
 * 
 * 127 = command unknown
 */
int	 exec_main(t_minishell *mini)
{
	int	result;

	result = prep_the_cmd(mini);
	if (result != 127)
		return (result);
	if (!mini->command->pipe_output && !mini->command->prev
		&& check_in_and_out(mini->command->fds))
	{
		config_in_and_out(mini->command->fds);
		result = exec_builtin(mini, mini->command);
		reset_fds_in_and_out(mini->command->fds);
	}
	if (result != 127)
		return (result);
	return (create_children(mini));
}

void	main_loop(t_minishell *mini)
{
	while (1)
	{
		listening_interact_sig();
		mini->line = readline("$-> ");
		listening_no_interact_sig();
		if(lexer_main(mini))
			g_status = exec_main(mini);
		clean_data(mini, false);
	}
}

int	main(int argc, char **argv, char **env)
{
	t_minishell	mini;

	(void)argv;
	if (argc != 1)
	{
		ft_putendl_fd("Error: execute with ./minishell, nothing more", 2);
		exit_mini(NULL, EXIT_FAILURE);
	}
	if (!init_main_struct(&mini, env))
		exit_mini(NULL, EXIT_FAILURE);
	main_loop(&mini);
	return (0);
}
