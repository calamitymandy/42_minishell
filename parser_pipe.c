#include "minishell.h"

void	pipe_parser(t_minishell *ms, t_token **token_lst)
{
	t_command	*last_cmd;

	last_cmd = scroll_lstcmd(ms->command);
	last_cmd->pipe_output = true;
	addlst_cmd_container(ms, &last_cmd);
	*token_lst = (*token_lst)->next;
}