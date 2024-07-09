NAME        = minishell

SRCS        = expand.c\

OBJS        = $(SRCS:.c=.o)

LIBFT_DIR   = libft
LIBFT       = $(LIBFT_DIR)/libft.a
INCLUDES    = -I$(LIBFT_DIR)
LIBS        = -lreadline -L$(LIBFT_DIR) -lft

CC          = gcc
CFLAGS      = $(INCLUDES) -I.
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
