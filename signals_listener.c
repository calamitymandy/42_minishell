#include "minishell.h"

void	listening_no_interact_sig(void)
{
	struct sigaction	action;

	ctrl_backslash_ignore_no_interact();
	ft_memset(&action, 0, sizeof(action));
	action.sa_handler = &ctrl_c_newline_no_interact;
	sigaction(SIGINT, &action, NULL);
}

void	listening_interact_sig(void)
{
	struct sigaction	action;

	ctrl_backslash_ignore();
	ft_memset(&action, 0, sizeof(action));
	action.sa_handler = &ctrl_c_newline;
	sigaction(SIGINT, &action, NULL);
}
