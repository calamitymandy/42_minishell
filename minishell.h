/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amdemuyn <amdemuyn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 19:02:14 by amdemuyn          #+#    #+#             */
/*   Updated: 2025/01/28 17:51:34 by amdemuyn         ###   ########.fr       */
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
	char			*content;
	char			*cc;
	bool			is_env_var;
	int				type;
	int				var_q_stat;
	int				index;
	bool			has_quotes;
	struct s_token	*prev;
	struct s_token	*next;
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
	t_command	*command;
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
void		ctrl_backslash_ignore(void);
void		ctrl_c_newline_hdoc(int signal);
void		listening_hdoc_input_sig(void);
void		ctrl_c_newline(int signal);
void		listening_interact_sig(void);
void		sigquit_handler_no_interact(int signal);
void		ctrl_backslash_ignore_no_interact(void);
void		ctrl_c_newline_no_interact(int signal);
void		listening_no_interact_sig(void);
bool	lexer_main(t_minishell *ms);
void	close_fds(t_command *cmds, bool close_backups);
void		clean_data(t_minishell *ms, bool clear_history);
void	free_star(void *ptr);
void	clean_tkn_nodes(t_token **lst, void (*del)(void *));
void	del_one_node_tkn(t_token *lst, void (*del)(void *));
void	rm_echo_empty_words(t_token **arg_list);
t_command	*new_cmd_lst(void);
void	exit_msg(t_minishell *ms, char *msg, int exit_code);
t_command	*scroll_lstcmd(t_command *aux);
void	parser_main(t_minishell *ms);
void	add_tkn_lst(t_token **lst, t_token *new_node);
t_token	*tkn_create(char *content, char *cntnt_cpy, int type, int qs);
int	msg_err(char *cc, char *info, char *msg, int error_code);
bool	dollar_error(char *content, int scan);
int ft_atoi_long(const char *str, bool *error);
char	*replace_str_heredoc(char *str, char *var_value, int index);
char	*xtract_var_value(t_token *token, char *content, t_minishell *ms);
char	*strjoin(char *str1, char *str2);
bool	quotes_err_n_read(t_minishell *ms, char *line);
void	exit_and_msg(t_minishell *ms, char *msg, int exit_code);
void	err_stx_out(char *message, char *quote, int in_quote);
int	quote_stat(int quote_stat, char *line, int scan);
int	var_name_len(char *content);
bool	isalphanum_or_underscore(char c);
void	expander_main(t_minishell *ms);
char	*replace_for_xpanded(t_token **aux, char *content, char *value, int scan);
char	*get_var_str(char *content, char *value, int trim_len, int scan);
bool	reset_fds_in_and_out(t_fds *fds_in_and_out);

int	exec_exit_builtin(t_minishell *mini, char **args);
bool	valid_env_key(char *key);

// builtins_export_unset
int			exec_export_builtin(t_minishell *mini, char **args);
int			export_builtin(t_minishell *mini);
int 		exec_unset_builtin(t_minishell *mini, char **args);
char		**key_value_arr(char *key);

// builtins_export_utils
void	qsort_env_vars(char **env, int nb_env_var);
char	*add_env_quotes(char *env_var);

// env_built
char	*get_env_value(char **env, char *key);
int		srch_env_i(char **env, char *pwd_or_old);
int	nb_env_variables(char **env);
bool	valid_env_key(char *key);

// utils_errors
int	error_msg(char *cmd, char *info, char *msg, int err_nb);

// builtins_cd_pwd
int	exec_cd(t_minishell *mini, char **args);
bool	cd(t_minishell *mini, char *path);
void	update_pwd_n_old(t_minishell *mini, char *buf_of_work_dir_path);
int	exec_pwd_builtin(t_minishell *mini, char **args);

// env_handlers
bool	add_or_update_env_var(t_minishell *mini, char *pwd_or_old, char *value);
char	**callocate_env_variables(t_minishell *mini, int size);
bool		delete_env_var_pos(t_minishell *mini, int pos);

//utils
char	*strjoin_n_free(char *s1, char const *s2);

//utils_init_data
bool    set_pwd_n_oldpwd(t_minishell *mini);

//builtins_env_exit
int	exec_env_builtin(t_minishell *mini, char **args);
int	exec_exit_builtin(t_minishell *mini, char **args);
int	get_exit_code(char *arg, bool *error);

// builtins_echo
int	exec_echo(t_minishell *mini, char **args);
void	print_echo(char **args, bool minus_n_flag, int i, t_minishell *mini);


int	chunk_reader(int *scan, char *line, int start_word, t_minishell *ms);
void	word_n_var_parser(t_minishell *ms, t_token **aux);
void	addlst_cmd_container(t_minishell *ms, t_command **cmd_list);
void	infile_parser(t_minishell *ms, t_token **aux);
void	heredoc_main(t_minishell *ms, t_token **aux);
void	skip_next_token(t_token **aux);
bool	set_fd_struct(t_command *command);
bool	fds_error(t_fds *fds);


#endif