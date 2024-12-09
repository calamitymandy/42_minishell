NAME        = minishell

SRCS        = main.c close_n_free.c signals.c signals_prompt.c lexer.c parser.c errors.c tokenizer.c expander.c utils.c
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
