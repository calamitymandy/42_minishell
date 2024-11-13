/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amdemuyn <amdemuyn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 19:02:14 by amdemuyn          #+#    #+#             */
/*   Updated: 2024/11/06 19:07:11 by amdemuyn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h> // QUIT?
#include <unistd.h>
#include <fcntl.h> 
#include <readline/readline.h>
#include <readline/history.h>
#include <stdbool.h>
#include <limits.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>


#include "./libft/libft.h"

#define MAX_INPUT_SIZE 1024
# define ERR_SYNTX_QUO	"syntax error: unclosed quote:"
# define ERR_SYNTX_TKN	"syntax error near unexpected token"
# define ERR_ALLOC		"Memory allocation error"
# define CMD_UNKNOWN		127
# define CMD_NOT_EXECUTABLE	126
# define ERR_NUM_ARR		255
# define ERR_PIPE_STX		258
# define BREAK	1
# define GO		0

extern int	g_status;

typedef struct s_token
{
	bool			has_quotes;
	int				var_q_stat;
	struct s_token	*prev;
	struct s_token	*next;
	char			*content;
	char			*token_type;
	bool			is_env_var;
	int				index;
	int				type;
	char			*cc;

}	t_token;

typedef struct s_fds
{
	char		*infile;
	char		*outfile;
	int			fd_infile;
	int			fd_outfile;
	int			stdin_ori;
	int			stdout_ori;
	bool		error_msg;
	bool		heredoc_quotes;
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
	bool		ctrlcheredoc;
	t_token		*token;
	t_command	*command; //cmd??
	pid_t		pid;
	
}	t_minishell;

//enums
enum e_token_types
{
	SPACES = 1,
	HEREDOC,
	VAR,
	INPUT,
	WORD,
	PIPE,
	TRUNC,
	APPEND,
	FAILURE,
	END
};

enum e_quotes
{
	OPN_SQ,
	OPN_DQ,
	OK_Q
};

void exit_mini(t_minishell *mini, int exit_code);
bool init_main_struct(t_minishell *mini, char **env);
void main_loop(t_minishell *mini);

void	free_star(void *ptr);
void	free_two_stars(char **arr);
void		ms_ctrl_backslash_ignore(void);
void		ms_ctrl_c_newline_hdoc(int signal);
void		ms_listening_hdoc_input_sig(void);
void		ms_ctrl_c_newline(int signal);
void		ms_listening_interact_sig(void);
void		ms_sigquit_handler_no_interact(int signal);
void		ms_ctrl_backslash_ignore_no_interact(void);
void		ms_ctrl_c_newline_no_interact(int signal);
void		ms_listening_no_interact_sig(void);
bool	ms_lexer_main(t_minishell *ms);
void	ms_close_fds(t_command *cmds, bool close_backups);
void		ms_data_free(t_minishell *ms, bool clear_history);
void	free_star(void *ptr);
void	ms_del_all_nodes_tkn(t_token **lst, void (*del)(void *));
void	ms_del_one_node_tkn(t_token *lst, void (*del)(void *));
void	ms_rm_echo_empty_words(t_token **arg_list);
t_command	*ms_new_cmd_lst(void);
void	ms_exit_msg(t_minishell *ms, char *msg, int exit_code);
t_command	*ms_scroll_lstcmd(t_command *aux);
void	ms_parser_main(t_minishell *ms);
void	ms_add_tkn_lst(t_token **lst, t_token *new_node);
t_token	*ms_tkn_create(char *content, char *cntnt_cpy, int type, int qs);
int	ms_msg_err(char *cc, char *info, char *msg, int error_code);
bool	ms_dollar_error(char *content, int scan);
int ft_atoi_long(const char *str, bool *error);
char	*replace_str_heredoc(char *str, char *var_value, int index);
char	*ms_xtract_var_value(t_token *token, char *content, t_minishell *ms);
void	ms_exit_ms(t_minishell *ms, int exit_code);
char	*ms_strjoin(char *str1, char *str2);
bool	ms_quotes_err_n_read(t_minishell *ms, char *line);
void	exit_minig(t_minishell *ms, char *msg, int exit_code);
void	ms_err_stx_out(char *message, char *quote, int in_quote);

#endif