NAME        = minishell

SRCS        = 	main.c \
				signals.c \
				tokenizer.c \
				lexer.c lexer_syntax.c \
				heredoc_loop.c heredoc_xpander.c \
				signals_listener.c signals_triggers.c \
				parser_main.c parser_args.c parser_words.c parser_commands.c parser_infile.c chunk_reader.c parser_heredoc.c parser_trunc.c parser_append.c parser_pipe.c \
				expander_main.c expander_scan.c expander_quotes.c expander_mark.c expander_var.c expander_x_var.c expander_utils.c \
				env_handlers.c env_shlvl.c env_built.c \
				redirec_io_n_pipes_controls.c \
				execute.c execute_cmd.c execute_parse_path.c \
				builtins_export_unset.c builtins_cd_pwd.c builtins_export_utils.c builtins_env_exit.c builtins_echo.c \
				utils_free_n_exit.c utils.c utils_init_data.c utils_clean_data.c utils_errors.c \

OBJS        = $(SRCS:.c=.o)

LIBFT_DIR   = libft
LIBFT       = $(LIBFT_DIR)/libft.a

# Detect the OS
UNAME := $(shell uname)

# Set Readline paths for Linux and macOS
ifeq ($(UNAME), Linux)
    READLINE_DIR = /usr
    LDFLAGS      = -L$(READLINE_DIR)/lib -L$(LIBFT_DIR) -lreadline -lft
    CPPFLAGS     = -I$(READLINE_DIR)/include -I$(LIBFT_DIR) -I.
else
    READLINE_DIR = /opt/homebrew/opt/readline
    LDFLAGS      = -L$(READLINE_DIR)/lib -L$(LIBFT_DIR) -lreadline -lft
    CPPFLAGS     = -I$(READLINE_DIR)/include -I$(LIBFT_DIR) -I.
endif

CC          = gcc
CFLAGS      = -g -Wall -Wextra -Werror

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(OBJS) $(LDFLAGS) $(CPPFLAGS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

$(LIBFT):
	@make -C $(LIBFT_DIR)

clean:
	@make clean -C $(LIBFT_DIR)
	rm -f $(OBJS)

fclean: clean
	@make fclean -C $(LIBFT_DIR)
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
