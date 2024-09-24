/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amdemuyn <amdemuyn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 19:02:14 by amdemuyn          #+#    #+#             */
/*   Updated: 2024/09/24 21:08:10 by amdemuyn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h> // QUIT?
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdbool.h>
#include <limits.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_INPUT_SIZE 1024

#include "./libft/libft.h"

extern int	g_status;

typedef struct s_token
{
	bool			has_quotes;
	struct s_token	*prev;
	struct s_token	*next;
	char			*content;
	char			*token_type;
	int				index;
	int				type;
}	t_token;

typedef struct s_fds
{
	char		*infile;
	char		*outfile;
	int			fd_infile;
	int			fd_outfile;
	int			stdin_ori;
	int			stdout_ori;
	bool		msg_err;
	char		*del_heredoc;
}	t_fds;

typedef struct s_command
{
	char				**args;
	bool				pipe_output;
	struct s_command	*prev;
	struct s_command	*next;
	int					*pipe_fd;
	t_fds				*fds;
	char				*cmd;
	char				*path;
}	t_command;

typedef struct	s_minishell
{
	char		**env;
	char		*line;
	char		*pwd;
	char		*old_pwd;
	//bool		ctrlc_heredoc;
	t_token		*token;
	t_command	*cmd;
	pid_t		pid;
	
}	t_minishell;

//enums
enum e_token_types
{
	VAR,
};

void exit_mini(t_minishell *mini, int exit_code);
bool init_main_struct(t_minishell *mini, char **env);
void main_loop(t_minishell *mini);

void	free_star(void *ptr);
void	free_two_stars(char **arr);

#endif