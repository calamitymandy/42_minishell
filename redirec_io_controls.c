//ADD HEADER

#include "minishell.h"

/**
 * Restores the original stdin & stdout fds if they were saved:
 * Uses dup2 to restore the original stdin & stdout.
 * Closes the temporary file descriptor.
 * Resets stdin_ori to -1.
 * returns a boolean value indicating whether the fds have been successfully reset.
 */
bool	reset_fds_in_and_out(t_fds *fds_in_and_out)
{
	int	res;

	res = true;
	if (!fds_in_and_out)
		return (res);
	if (fds_in_and_out->stdin_ori != -1)
	{
		if (dup2(fds_in_and_out->stdin_ori, STDIN_FILENO) == -1)
			res = false;
		close(fds_in_and_out->stdin_ori);
		fds_in_and_out->stdin_ori = -1;
	}
	if (fds_in_and_out->stdout_ori != -1)
	{
		if (dup2(fds_in_and_out->stdout_ori, STDOUT_FILENO) == -1)
			res = false;
		close(fds_in_and_out->stdout_ori);
		fds_in_and_out->stdout_ori = -1;
	}
	return (res);
}

/**
 * SLIGHT MODIF
 * configures the standard input and output file descriptors for a process.
 * It makes duplicates of the current standard input and output, then 
 * redirects them to specified files if provided. 
 * 
 * 1. Checks if the in_n_out structure is not NULL.
 * 2. Duplicates the current standard input and output file descriptors and
 * stores them in the stdin_ori and stdout_ori fields of the in_n_out structure.
 * 3. Redirects the standard input to fd_infile if it is provided.
 * 4. Redirects the standard output to fd_outfile if it is provided.
 * 5. Handles any errors during these operations and logs them using the 
 * error_msg function.
 * 6. Returns true if all operations succeed and false if any operation fails.
 */
bool	config_in_and_out(t_fds	*in_n_out)
{
	int	result;

	result = true;
	if (!in_n_out)
		return (result);
	in_n_out->stdin_ori = dup(STDIN_FILENO);
	in_n_out->stdout_ori = dup(STDOUT_FILENO);
	if (in_n_out->stdin_ori == -1)
		result = error_msg("dup", "stdin_ori", strerror(errno), false);
	if (in_n_out->stdout_ori == -1)
		result = error_msg("dup", "stdout_ori", strerror(errno), false);
	if (in_n_out->fd_infile != -1
		&& (dup2(in_n_out->fd_infile, STDIN_FILENO) == -1))
		result = error_msg("dup2", in_n_out->infile, strerror(errno), false);
	if (in_n_out->fd_outfile != -1
		&& (dup2(in_n_out->fd_outfile, STDOUT_FILENO) == -1))
		result = error_msg("dup2", in_n_out->outfile, strerror(errno), false);
	return (result);
}

/**
 * Checks if input and output file descriptors are valid in a given structure.
 * If the file descriptors are valid, it returns true. 
 * Otherwise, it returns false.
 */
bool	check_in_and_out(t_fds	*in_n_out)
{
	if (!in_n_out || (!in_n_out->infile && !in_n_out->outfile))
		return (true);
	if ((in_n_out->infile && in_n_out->fd_infile == -1)
		|| (in_n_out->outfile && in_n_out->fd_outfile == -1))
		return (false);
	return (true);
}