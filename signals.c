#include "minishell.h"


void	ctrl_c_newline_hdoc(int signal)
{
	if (signal != SIGINT)
		return ;
	g_status = 130;
	ft_putstr_fd("\1\n", STDIN_FILENO);
}

void	listening_hdoc_input_sig(void)
{
	struct sigaction	action;

	ctrl_backslash_ignore();
	ft_memset(&action, 0, sizeof(action));
	action.sa_handler = &ctrl_c_newline_hdoc;
	sigaction(SIGINT, &action, NULL);
}


void	sigquit_handler_no_interact(int signal)
{
	(void)signal;
	ft_putstr_fd("Quit: 3\n", STDOUT_FILENO);
	rl_on_new_line();
}



