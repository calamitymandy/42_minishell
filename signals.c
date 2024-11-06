#include "minishell.h"

void	ms_ctrl_backslash_ignore(void)
{
	struct sigaction	action;

	ft_memset(&action, 0, sizeof(action));
	action.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &action, NULL);
}

void	ms_ctrl_c_newline_hdoc(int signal)
{
	if (signal != SIGINT)
		return ;
	g_status = 130;
	ft_putstr_fd("\1\n", STDIN_FILENO);
}

void	ms_listening_hdoc_input_sig(void)
{
	struct sigaction	action;

	ms_ctrl_backslash_ignore();
	ft_memset(&action, 0, sizeof(action));
	action.sa_handler = &ms_ctrl_c_newline_hdoc;
	sigaction(SIGINT, &action, NULL);
}

void	ms_ctrl_c_newline(int signal)
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

void	ms_listening_interact_sig(void)
{
	struct sigaction	action;

	ms_ctrl_backslash_ignore();
	ft_memset(&action, 0, sizeof(action));
	action.sa_handler = &ms_ctrl_c_newline;
	sigaction(SIGINT, &action, NULL);
}


void	ms_sigquit_handler_no_interact(int signal)
{
	(void)signal;
	ft_putstr_fd("Quit: 3\n", STDOUT_FILENO);
	rl_on_new_line();
}

void	ms_ctrl_backslash_ignore_no_interact(void)
{
	struct sigaction	action;

	ft_memset(&action, 0, sizeof(action));
	action.sa_handler = &ms_sigquit_handler_no_interact;
	sigaction(SIGQUIT, &action, NULL);
}

void	ms_ctrl_c_newline_no_interact(int signal)
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

void	ms_listening_no_interact_sig(void)
{
	struct sigaction	action;

	ms_ctrl_backslash_ignore_no_interact();
	ft_memset(&action, 0, sizeof(action));
	action.sa_handler = &ms_ctrl_c_newline_no_interact;
	sigaction(SIGINT, &action, NULL);
}