NAME        = minishell

SRCS        = main.c close_n_free.c signals.c lexer.c parser.c errors.c utils.c tokenizer.c expander.c\

OBJS        = $(SRCS:.c=.o)

LIBFT_DIR   = libft
LIBFT       = $(LIBFT_DIR)/libft.a
INCLUDES    = -I$(LIBFT_DIR)
LIBS        = -lreadline -L$(LIBFT_DIR) -lft

CC          = gcc
CFLAGS      = $(INCLUDES) -g -I.
LDFLAGS     = $(LIBS)

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	@$(CC) $(OBJS) $(LDFLAGS) -o $(NAME)

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT):
	@make -sC $(LIBFT_DIR)

clean:
	@make clean -sC $(LIBFT_DIR)
	@rm -f $(OBJS)

fclean: clean
	@make fclean -sC $(LIBFT_DIR)
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
