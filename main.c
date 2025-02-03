/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amdemuyn <amdemuyn@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 20:24:50 by amdemuyn          #+#    #+#             */
/*   Updated: 2025/02/03 22:43:46 by amdemuyn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*compile with gcc main.c -lreadline*/

int	g_status;


/**
 * Restores the original stdin & stdout fds if they were saved:
 * Uses dup2 to restore the original stdin & stdout.
 * Closes the temporary file descriptor.
 * Resets stdin_ori to -1.
 * returns a boolean value indicating whether the fds have been successfully reset.

bool	reset_fds_in_and_out(t_fds *fds_in_and_out)
{
	int	res;

	res = true;
	if (!fds_in_and_out)
		return (res);
	if (fds_in_and_out->stdin_ori != -1)
	{
		if (dup2(fds_in_and_out->stdin_ori, STDIN_FILENO) == -1)
			res = false;
		close(fds_in_and_out->stdin_ori);
		fds_in_and_out->stdin_ori = -1;
	}
	if (fds_in_and_out->stdout_ori != -1)
	{
		if (dup2(fds_in_and_out->stdout_ori, STDOUT_FILENO) == -1)
			res = false;
		close(fds_in_and_out->stdout_ori);
		fds_in_and_out->stdout_ori = -1;
	}
	return (res);
}
 */


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

/* mix of 2: delete all token node && delete one token node
 * ADDED: delete_all, flag to delete one or all nodes in the list, 
 * depending on the flag being on 1 or 0.
*/
void	clean_token_nodes(t_token **lst, void (*del)(void *), int delete_all)
{
	t_token	*temp;

	while (*lst != NULL)
	{
		temp = (*lst)->next;
		if (del && (*lst))
		{
			if ((*lst)->content)
				(*del)((*lst)->content);
			if ((*lst)->cc) //or token_type
				(*del)((*lst)->cc); //or token_type
		}
		if ((*lst)->prev)
			(*lst)->prev->next = (*lst)->next;
		if ((*lst)->next)
			(*lst)->next->prev = (*lst)->prev;
		free_star(*lst);
		if (!delete_all)
			break ;
		*lst = temp;
	}
	if (delete_all)
		*lst = NULL;
}
/*
void	free_in_and_out_fds(t_fds *in_and_out)
{
	if (!in_and_out)
		return ;
	reset_fds_in_and_out(in_and_out);
	if (in_and_out->del_heredoc)
	{
		unlink(in_and_out->del_heredoc);
		free_star(in_and_out->del_heredoc);
	}
	if (in_and_out->infile)
		free_star(in_and_out->infile);
	if (in_and_out->outfile)
		free_star(in_and_out->outfile);
	if (in_and_out)
		free_star(in_and_out);
}
*/

/*mix of 2 clean all nodes and clean one node
void	clean_cmd_nodes(t_command **lst, void (*del)(void *))
{
	t_command	*temp;
	
	while (*lst != NULL)
	{
		temp = (*lst)->next;
		if ((*lst)->cmd)
			(*del)((*lst)->cmd);
		if ((*lst)->args)
			(*del)((*lst)->args);
		if ((*lst)->pipe_fd)
			(*del)((*lst)->pipe_fd);
		if ((*lst)->fds)
			free_in_and_out_fds((*lst)->fds);
		(*del)(*lst);
		*lst = temp;
	}
	*lst = NULL;
}
*/

/*void	clean_data(t_minishell *mini, bool clear_hist_or_not)
{
	if (mini && mini->line)
	{
		free_star(mini->line);
		mini->line = NULL;
	}
	if (mini && mini->token)
		clean_token_nodes(&mini->token, &free_star, 1);
	if (mini && mini->command)
		clean_cmd_nodes(&mini->command, &free_star);
	if (clear_hist_or_not == true)
	{
		if (mini && mini->pwd)
			free_star(mini->pwd);
		if (mini && mini->old_pwd)
			free_star(mini->old_pwd);
		if (mini && mini->env)
			free_two_stars(mini->env);
		clear_history();
	}
}
*/

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
