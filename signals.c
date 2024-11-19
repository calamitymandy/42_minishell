#include "minishell.h"

void	ctrl_backslash_ignore(void)
{
	struct sigaction	action;

	ft_memset(&action, 0, sizeof(action));
	action.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &action, NULL);
}

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

void	listening_interact_sig(void)
{
	struct sigaction	action;

	ctrl_backslash_ignore();
	ft_memset(&action, 0, sizeof(action));
	action.sa_handler = &ctrl_c_newline;
	sigaction(SIGINT, &action, NULL);
}


void	sigquit_handler_no_interact(int signal)
{
	(void)signal;
	ft_putstr_fd("Quit: 3\n", STDOUT_FILENO);
	rl_on_new_line();
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

void	listening_no_interact_sig(void)
{
	struct sigaction	action;

	ctrl_backslash_ignore_no_interact();
	ft_memset(&action, 0, sizeof(action));
	action.sa_handler = &ctrl_c_newline_no_interact;
	sigaction(SIGINT, &action, NULL);
}