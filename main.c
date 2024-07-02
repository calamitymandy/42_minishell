/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amdemuyn <amdemuyn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 20:24:50 by amdemuyn          #+#    #+#             */
/*   Updated: 2024/07/01 21:52:43 by amdemuyn         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "minishell.h"
/*compile with gcc main.c -lreadline*/

int	g_status;

bool	check_in_and_out(t_fds	*in_n_out)
{
	if (!in_n_out || (!in_n_out->infile && !in_n_out->outfile))
		return (true);
	if ((in_n_out->infile && in_n_out->fd_infile == -1)
		|| (in_n_out->outfile && in_n_out->fd_outfile == -1))
		return (false);
	return (true);
}

bool	config_in_and_out(t_fds	*in_n_out)
{
	int result;

	result = true;
	if (!in_n_out)
		return (result);
	
	in_n_out->stdin_ori = dup(STDIN_FILENO);
	in_n_out->stdout_ori = dup(STDOUT_FILENO);

	if (in_n_out->stdin_ori == -1)
		result = printf("TODO stdin_ori MSG ERROR");
	if (in_n_out->stdout_ori == -1)
		result = printf("TODO stdout_ori MSG ERROR");
	if (in_n_out->fd_infile != -1 && 
		(dup2(in_n_out->fd_infile, STDIN_FILENO) == -1))
		result = printf("TODO dup2 fd_infile MSG ERROR");
	if (in_n_out->fd_outfile != -1 && 
		(dup2(in_n_out->fd_outfile, STDIN_FILENO) == -1))
		result = printf("TODO dup2 fd_outfile MSG ERROR");
	return (result);
}

bool	reset_fds_in_and_out(t_fds *fds_in_and_out)
{
	int res;
	
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

void	close_pipe_fd(t_command *cmd, t_command *skip_cmd)
{
	while (cmd)
	{
		if (cmd != skip_cmd && cmd->pipe_fd)
		{
			close(cmd->pipe_fd[0]);
			close(cmd->pipe_fd[1]);
		}
		cmd = cmd->next;
	}
}

void	close_fds(t_command *cmd, bool close_or_not)
{
	if (cmd->fds)
	{
		if (cmd->fds->fd_infile || cmd->fds->fd_outfile)
		{
			if (cmd->fds->fd_infile != -1)
				close (cmd->fds->fd_infile);
			else
				close (cmd->fds->fd_outfile);
		}
		if (close_or_not)
			reset_fds_in_and_out(cmd->fds);
	}
    close_pipe_fd(cmd, NULL);
}

void	exit_mini(t_minishell *mini, int exit_code)
{
	if (mini)
	{
		if (mini->cmd && mini->cmd->fds)
			close_fds(mini->cmd, true);
		//free data
	}
	exit(exit_code);
}

bool	init_main_struct(t_minishell *mini, char **env)
{
	//TODO set_env_var & set_pwd_and aldpwd
	mini->env = env;
	mini->line = NULL;
	//mini->ctrlc_heredoc = false;
	mini->token = NULL;
	mini->cmd = NULL;
	mini->pid = -1;
	g_status = 0;
	return (true);
}

bool	create_pipes(t_minishell *mini)
{
	int	*fd;
	t_command *temp_cmd;
	
	temp_cmd = mini->cmd;
	while (temp_cmd)
	{
		if (temp_cmd->pipe_output || (temp_cmd->prev && temp_cmd->prev->pipe_output))
		{
			fd = malloc (sizeof * fd * 2);
			if (!fd || pipe (fd) != 0)
			{
				//free data
				return (false);
			}
			temp_cmd->pipe_fd = fd;
		}
		temp_cmd = temp_cmd->next;
	}
	return (true);
}

int	prep_the_cmd(t_minishell *mini)
{
	printf("here it prints");
	//TODO init mini->cmd->cmd & mini->token->has_quotes somewhere
	if (!mini || !mini->cmd || !mini->cmd->cmd ||
		(mini->cmd->cmd[0] == '\0' && mini->token->has_quotes == false))
		return (EXIT_SUCCESS);
	printf("here it does not print");
	if (mini->cmd && !mini->cmd->cmd)
	{
		if (mini->cmd->fds && !check_in_and_out(mini->cmd->fds))
			return (EXIT_FAILURE);
		return (EXIT_SUCCESS);
	}
	if (!create_pipes(mini))
		return (EXIT_FAILURE);
	return (127);
}

/* This function executes the processed command and returns a status code 
that updates g_status. 
127 = cmd unknown*/
int	exec_main(t_minishell *mini)
{
	int result;
	result = prep_the_cmd(mini);
	if (result != 127)
		return (result);
	if (!mini->cmd->pipe_output && !mini->cmd->prev
		&& check_in_and_out(mini->cmd->fds))
	{
		config_in_and_out(mini->cmd->fds);
		//TODO result = exec_builtin(mini, mini->cmd);
		//TODO reset in out
	}
	if (result != 127)
		return (result);
	//TODO return (create_children(mini));
	return (1);
}

void	main_loop(t_minishell *mini)
{
	while (1) {
		//TODO interact sig
		mini->line = readline("$-> ");
		//TODO no interact sig
/*
		if (!mini->line)
			break; // Exit on EOF (Ctrl-D)

        // Remove trailing newline
        mini->line[strcspn(mini->line, "\n")] = '\0';

        // Exit command
        if (strcmp(mini->line, "exit") == 0) {
            free(mini->line);
            break;
        }
*/
        if (strlen(mini->line) > 0) {
            add_history(mini->line);
        }
        free(mini->line);
		
		//process input line
		//TODO if(lexer)
			g_status = exec_main(mini);
			
		//TODO free data (NOW: free(mini->line);)
		
        //printf("You wrote: %s\n", mini->line);
		printf("You wrote: %d\n", g_status);
    }
}

int	main(int argc, char **argv, char **env)
{
	t_minishell mini;
	
	(void)argv;
	if (argc != 1)
	{
		ft_putendl_fd("Error: execute with ./minishell, nothing more", 2);
		exit_mini(NULL, EXIT_FAILURE);
	}
    if (!init_main_struct(&mini, env))
		exit_mini(NULL, EXIT_FAILURE);
	main_loop(&mini);
	return 0;
}
