/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amdemuyn <amdemuyn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 19:02:14 by amdemuyn          #+#    #+#             */
/*   Updated: 2024/06/20 20:59:55 by amdemuyn         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdbool.h>

#define MAX_INPUT_SIZE 1024

#include "./libft/libft.h"

extern int	g_status;

typedef struct s_token
{
	
}	t_token;

typedef struct s_command
{
	bool				pipe_output;
	struct s_command	*prev;
	struct s_command	*next;
	int					*pipe_fd;
}	t_command;

typedef struct	s_minishell
{
	char		**env;
	char		*line;
	//bool		ctrlc_heredoc;
	//t_token		*token;
	t_command	*cmd;
	pid_t		pid;
	
}	t_minishell;

void exit_mini(t_minishell *mini, int exit_code);
bool init_main_struct(t_minishell *mini, char **env);
void main_loop(t_minishell *mini);

#endif