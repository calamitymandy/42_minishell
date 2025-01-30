#include "minishell.h"


bool	fds_error2(t_fds *fds)
{
	if (fds->outfile)
	{
		if (fds->fd_outfile == -1 || (fds->infile && fds->fd_infile == -1))
			return (true);
		free_star(fds->outfile);
		close(fds->fd_outfile);
	}
	return (false);
}

void	create_trunc(t_fds *fds, char *file_name, char *cc)
{
	if (fds_error2(fds))
		return ;
	fds->outfile = ft_strdup(file_name);
	if (fds->outfile && fds->outfile[0] == '\0' && fds->error_msg == false)
	{
		msg_err(cc, NULL, "ambiguous redirect", false);
		fds->error_msg = true;
		return ;
	}
	fds->fd_outfile = open(fds->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0664);
	if (fds->fd_outfile == -1 && fds->error_msg == false)
	{
		msg_err(fds->outfile, NULL, strerror(errno), false);
		fds->error_msg = true;
	}
}

void	trunc_parser(t_minishell *ms, t_token **aux)
{
	t_token		*tkn_process;
	t_command	*last_cmd;

	tkn_process = *aux;
	last_cmd = scroll_lstcmd(ms->command);
	if (last_cmd->fds && last_cmd->fds->error_msg)
	{
		skip_next_token(aux); 
		return ;
	}
	if (!set_fd_struct(last_cmd))
		exit_msg(ms, ERR_ALLOC, EXIT_FAILURE); //error_msg??
	if (!ms->ctrlcheredoc) 
		create_trunc(last_cmd->fds, tkn_process->next->content, \
	tkn_process->next->cc);
	skip_next_token(aux);
}