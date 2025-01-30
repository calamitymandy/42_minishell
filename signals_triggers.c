#include "minishell.h"

void	ctrl_backslash_ignore(void)
{
	struct sigaction	action;

	ft_memset(&action, 0, sizeof(action));
	action.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &action, NULL);
}

void	ctrl_c_newline(int signal)
{
	if (signal != SIGINT)
		return ;
	ft_putchar_fd('\n', STDOUT_FILENO);
	rl_on_new_line();
	rl_replace_line("", STDOUT_FILENO);
	rl_on_new_line();
	rl_redisplay();
	g_status = 130;
}

void	ctrl_backslash_ignore_no_interact(void)
{
	struct sigaction	action;

	ft_memset(&action, 0, sizeof(action));
	action.sa_handler = &sigquit_handler_no_interact;
	sigaction(SIGQUIT, &action, NULL);
}

void	ctrl_c_newline_no_interact(int signal)
{
	if (signal != SIGINT)
		return ;
	ft_putchar_fd('\n', STDOUT_FILENO);
	rl_on_new_line();
	rl_replace_line("", STDIN_FILENO);
	rl_on_new_line();
	rl_redisplay();
	rl_on_new_line();
}
