/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amdemuyn <amdemuyn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 20:24:50 by amdemuyn          #+#    #+#             */
/*   Updated: 2024/06/20 20:59:52 by amdemuyn         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "minishell.h"
/*compile with gcc main.c -lreadline*/

int g_status;

void    exit_mini(t_minishell *mini, int exit_code)
{
    //TODO
    exit(exit_code);
}

bool    init_main_struct(t_minishell *mini, char **env)
{
    //TODO set_env_var & set_pwd_and aldpwd
    mini->env = env;
    mini->line = NULL;
    //mini->ctrlc_heredoc = false;
    //mini->token = NULL;
    //mini->cmd = NULL;
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
	//TODO STRUCTS CMD/TOKEN & CHECK IO FCT
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
}

void    main_loop(t_minishell *mini)
{
    while (1) {
        mini->line = readline("$-> ");
        if (!mini->line) {
            break; // Exit on EOF (Ctrl-D)
        }

        // Remove trailing newline
        mini->line[strcspn(mini->line, "\n")] = '\0';

        // Exit command
        if (strcmp(mini->line, "exit") == 0) {
            free(mini->line);
            break;
        }

		//process input line
		//if(lexer)
			g_status = exec_main(mini);

        // Here you will add command execution
        printf("You entered: %s\n", mini->line);

        // Add to history
        if (strlen(mini->line) > 0) {
            add_history(mini->line);
        }
        free(mini->line);
    }
}

int main(int argc, char **argv, char **env)
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
