/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_echo.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amdemuyn <amdemuyn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 17:45:22 by amdemuyn          #+#    #+#             */
/*   Updated: 2025/02/04 19:16:26 by amdemuyn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Handle the special -n flag, which suppress the newline at the end of the 
 * output.
 * Ensures that a string like -n, -nn, -nnn, etc., is valid but not something 
 * like -nX or -nx
 * Mix of 2 with n_flag
*/
int	exec_echo(t_minishell *mini, char **args)
{
	int		i;
	int		j;
	bool	minus_n_flag;

	minus_n_flag = false;
	i = 1;
	while (args[i])
	{
		j = 0;
		if (args[i][j] != '-' || (args[i][j] == '-' && args[i][j + 1] != 'n'))
			break ;
		j++;
		while (args[i][j] && args[i][j] == 'n')
			j++;
		if (args[i][j] != '\0')
			break ;
		minus_n_flag = true;
		i++;
	}
	print_echo(args, minus_n_flag, i, mini);
	return (EXIT_SUCCESS);
}

void	print_echo(char **args, bool minus_n_flag, int i, t_minishell *mini)
{
	char	*clean_arg;

	if (!args[i])
	{
		if (!minus_n_flag)
			ft_putchar_fd('\n', STDOUT_FILENO);
		return ;
	}
	while (args[i])
	{
		if (is_var_no_quotes(mini->token, i))
		{
			clean_arg = remove_extra_spaces(args[i]);
			ft_putstr_fd(clean_arg, STDOUT_FILENO);
			free(clean_arg);
		}
		else
			ft_putstr_fd(args[i], STDOUT_FILENO);
		if (args[i +1])
			ft_putchar_fd(' ', STDOUT_FILENO);
		else if (!args[i + 1] && !minus_n_flag)
			ft_putchar_fd('\n', STDOUT_FILENO);
		i++;
	}
}

/*Mix of 2 with quotes checker*/
bool	is_var_no_quotes(t_token *tkns, int index)
{
	t_token	*lst;
	int		i;

	i = 0;
	lst = tkns;
	while (lst)
	{
		if (lst->index == index && lst->type == VAR)
		{
			while (lst->cc[i])
			{
				if (lst->cc[i] == '"' || lst->cc[i] == '\'')
					return (false);
				i++;
			}
			return (true);
		}
		lst = lst->next;
	}
	return (false);
}

/* 
Mix of 2 with count_no_ws
*/
char	*remove_extra_spaces(const char *str)
{
	char	*new_string;
	int		i;
	int		j;

	i = 0;
	j = 0;
	new_string = malloc((ft_strlen(str) + 1) * sizeof(char));
	if (!new_string)
		return (NULL);
	i = 0;
	while (str[i])
	{
		if (!ft_isblank(str[i]) || (i > 0 && !ft_isblank(str[i - 1])))
			new_string[j++] = str[i];
		i++;
	}
	while (j > 0 && ft_isblank(new_string[j - 1]))
		j--;
	new_string[j] = '\0';
	return (new_string);
}
