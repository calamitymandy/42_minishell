/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amdemuyn <amdemuyn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 19:02:14 by amdemuyn          #+#    #+#             */
/*   Updated: 2025/02/04 19:41:19 by amdemuyn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h> // QUIT?
# include <unistd.h>
# include <fcntl.h> 
# include <readline/readline.h>
# include <readline/history.h>
# include <stdbool.h>
# include <limits.h>
# include <errno.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <signal.h>

# include "./libft/libft.h"

# define MAX_INPUT_SIZE 1024
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

typedef struct s_minishell
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

void		ctrl_backslash_ignore(void);
void		ctrl_c_newline_hdoc(int signal);
void		listening_hdoc_input_sig(void);
void		ctrl_c_newline(int signal);
void		listening_interact_sig(void);
void		sigquit_handler_no_interact(int signal);
void		ctrl_backslash_ignore_no_interact(void);
void		ctrl_c_newline_no_interact(int signal);
void		listening_no_interact_sig(void);
bool		lexer_main(t_minishell *ms);
void		close_fds(t_command *cmds, bool close_backups);
void		clean_tkn_nodes(t_token **lst, void (*del)(void *));
void		del_one_node_tkn(t_token *lst, void (*del)(void *));
void		rm_echo_empty_words(t_token **arg_list);
t_command	*new_cmd_lst(void);
t_command	*scroll_lstcmd(t_command *aux);
void		parser_main(t_minishell *ms);
void		add_tkn_lst(t_token **lst, t_token *new_node);
t_token		*tkn_create(char *content, char *cntnt_cpy, int type, int qs);
int			error_msg(char *cc, char *info, char *msg, int error_code);
bool		dollar_error(char *content, int scan);
int			ft_atoi_long(const char *str, bool *error);
char		*replace_str_heredoc(char *str, char *var_value, int index);
char		*xtract_var_value(t_token *token, char *content, t_minishell *ms);
char		*strjoin(char *str1, char *str2);
bool		quotes_err_n_read(t_minishell *ms, char *line);
void		exit_and_msg(t_minishell *ms, char *msg, int exit_code);
void		err_stx_out(char *message, char *quote, int in_quote);
int			quote_stat(int quote_stat, char *line, int scan);
int			var_name_len(char *content);
bool		isalphanum_or_underscore(char c);
void		expander_main(t_minishell *ms);
char		*replace_for_xpanded(t_token **aux, char *content, char *value,
				int scan);
char		*get_var_str(char *content, char *value, int trim_len, int scan);
int			chunk_reader(int *scan, char *line, int start_word,
				t_minishell *ms);
void		word_n_var_parser(t_minishell *ms, t_token **aux);
void		split_in_args(t_command *new_cmd, char *tkn_cntnt, t_minishell *ms);
void		addlst_cmd_container(t_minishell *ms, t_command **cmd_list);
void		infile_parser(t_minishell *ms, t_token **aux);
void		heredoc_main(t_minishell *ms, t_token **aux);
void		skip_next_token(t_token **aux);
bool		set_fd_struct(t_command *command);
bool		fds_error(t_fds *fds);
void		trunc_parser(t_minishell *ms, t_token **aux);
void		append_parser(t_minishell *ms, t_token **aux);
void		pipe_parser(t_minishell *ms, t_token **token_lst);
bool		fds_error2(t_fds *fds);

int			exec_exit_builtin(t_minishell *mini, char **args);
bool		valid_env_key(char *key);

// redirec_io_controls
bool		reset_fds_in_and_out(t_fds *fds_in_and_out);
bool		config_in_and_out(t_fds	*in_n_out);
bool		check_in_and_out(t_fds	*in_n_out);

//redirec_pipe_controls
bool		create_pipes(t_minishell *mini);
bool		set_n_close_pipes_fds(t_command *cmd_list, t_command *current_cmd);

// execute
int			create_children(t_minishell *mini);
int			exec_cmd(t_minishell *mini, t_command *command);
int			child_status(t_minishell *mini);
int			prep_the_cmd(t_minishell *mini);

//execute_cmd
int			exec_local_binary(t_minishell *mini, t_command *command);
int			exec_sys_binary(t_minishell *mini, t_command *command);
int			exec_builtin(t_minishell *mini, t_command *command);
int			exec_cmd(t_minishell *mini, t_command *command);

// execute_parse_path
char		*get_path_cmd(t_minishell *mini, char *str);
char		*find_valid_cmd_path(char *cmd, char **all_paths);

// execute_utils
bool		is_directory(char *cmd);

// builtins_export_unset
int			exec_export_builtin(t_minishell *mini, char **args);
int			export_builtin(t_minishell *mini);
int			exec_unset_builtin(t_minishell *mini, char **args);
char		**key_value_arr(char *key);

// builtins_export_utils
void		qsort_env_vars(char **env, int nb_env_var);
char		*add_env_quotes(char *env_var);

// builtins_cd_pwd
int			exec_cd(t_minishell *mini, char **args);
bool		cd(t_minishell *mini, char *path);
void		update_pwd_n_old(t_minishell *mini, char *buf_of_work_dir_path);
int			exec_pwd_builtin(t_minishell *mini, char **args);

// env_built
char		*get_env_value(char **env, char *key);
int			srch_env_i(char **env, char *pwd_or_old);
int			nb_env_variables(char **env);
bool		valid_env_key(char *key);
int			find_env_index_of_key(char **env, char *key);

// utils_errors
int			error_msg(char *cmd, char *info, char *msg, int err_nb);

// env_handlers
bool		add_or_update_env_var(t_minishell *mini, char *pwd_or_old,
				char *value);
char		**callocate_env_variables(t_minishell *mini, int size);
bool		delete_env_var_pos(t_minishell *mini, int pos);

// env_copy
bool		copy_environment(t_minishell *ms, char **envp);
void		refresh_pwd_env(t_minishell *mini);

// env_shlvl
void		modify_shlvl(t_minishell *ms);
char		*search_env(t_minishell *ms, char *env_key);
char		**arr_append(char **arr, char *line);
void		modify_or_add_env(t_minishell *ms, char *line);

//utils
char		*strjoin_n_free(char *s1, char const *s2);

//utils_init_data
bool		init_main_struct(t_minishell *mini, char **env);
bool		set_pwd_n_oldpwd(t_minishell *mini);
bool		set_env_var(t_minishell *ms, char **env);
bool		create_env(t_minishell *mini);

//builtins_env_exit
int			exec_env_builtin(t_minishell *mini, char **args);
int			exec_exit_builtin(t_minishell *mini, char **args);
int			get_exit_code(char *arg, bool *error);

// builtins_echo
int			exec_echo(t_minishell *mini, char **args);
void		print_echo(char **args, bool minus_n_flag, int i,
				t_minishell *mini);
bool		is_var_no_quotes(t_token *tkns, int index);
char		*remove_extra_spaces(const char *str);

// utils_close_n_free
void		free_star(void *ptr);
void		free_two_stars(char **arr);
void		clean_data(t_minishell *ms, bool clear_history);
void		close_fds(t_command *command, bool close_or_not);
void		exit_mini(t_minishell *mini, int exit_code);

void	split_in_args(t_command *new_cmd, char *tkn_cntnt, t_minishell *ms);
bool	cmd_arg_filler(t_token **arg_list, t_command *command);
int	word_n_var_counter(t_token *aux);
char	**create_table(int args_amnt, char **args_table, t_command *command, t_token **arg_list);
void	scan_variables(t_minishell *ms);
int	process_quotes(t_minishell *ms);
void	mark_variables(t_minishell *ms);
void	process_variables(char *value, t_token **aux, int scan, t_minishell *ms);
bool	is_env_var(t_minishell *ms, char *var_nme);
char	*dup_env_var_value(t_minishell *ms, char *var_nme);
void	val_cpy(char *new, char *value, int *j);
bool	dollar_error(char *content, int scan);
bool	is_between_d_quot(char *content, int scan);
bool	is_bad_char_next(char next);


#endif