/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_scan_var.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: algalian <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 18:34:49 by algalian          #+#    #+#             */
/*   Updated: 2025/02/04 18:34:51 by algalian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*xtract_var_name(t_minishell *ms, char *content)
{
	char	*var_name;
	char	*tmp;
	int		start;
	int		len;
	int		i;

	i = -1;
	start = 0;
	while (content[++i])
	{
		if (content[i] == '$')
		{
			start = i + 1;
			break ;
		}
	}
	len = var_name_len(content);
	var_name = ft_substr(content, start, len);
	if (!var_name)
		exit_msg(ms, ERR_ALLOC, EXIT_FAILURE);
	tmp = ft_strjoin(var_name, "=");
	free_star(var_name);
	var_name = tmp;
	return (var_name);
}

char	*xtract_var_value(t_token *token, char *content, t_minishell *ms)
{
	char	*value;
	char	*var_nme;

	var_nme = xtract_var_name(ms, content);
	if (var_nme && is_env_var(ms, var_nme))
	{
		if (token)
			token->is_env_var = true;
		value = dup_env_var_value(ms, var_nme);
	}
	else if (var_nme && var_nme[0] == '?' && var_nme[1] == '=')
		value = ft_itoa(g_status);
	else
		value = NULL;
	free_star(var_nme);
	return (value);
}

bool	xpand_if_null(t_token **aux, char *content, int scan)
{
	int		i;
	int		j;
	int		trim_len;
	char	*trim_cntnt;

	trim_len = ft_strlen(content) - var_name_len(content + scan);
	trim_cntnt = (char *)malloc(sizeof(char) * trim_len + 1);
	if (!trim_cntnt)
		return (false);
	i = 0;
	j = 0;
	while (content[i])
	{
		if (content[i] == '$' && i == scan)
		{
			i = i + var_name_len(content + scan) + 1;
			if (content[i] == '\0')
				break ;
		}
		trim_cntnt[j++] = content[i++];
	}
	trim_cntnt[j] = '\0';
	free_star((*aux)->content);
	(*aux)->content = trim_cntnt;
	return (true);
}

char	*replace_for_xpanded(t_token **aux, char *content, \
		char *value, int scan)
{
	int		trim_len;
	char	*trim_cntnt;

	trim_len = (ft_strlen(content) - var_name_len(content + scan)
			+ ft_strlen(value));
	trim_cntnt = get_var_str(content, value, trim_len, scan);
	if (aux && *aux)
	{
		free_star((*aux)->content);
		(*aux)->content = trim_cntnt;
	}
	return (trim_cntnt);
}

void	process_variables(char *value, t_token **aux, int scan, t_minishell *ms)
{
	if (!value)
	{
		if (!xpand_if_null(aux, (*aux)->content, scan))
		{
			free_star(value);
			exit_msg(ms, ERR_ALLOC, EXIT_FAILURE);
		}
	}
	else
	{
		if (!replace_for_xpanded(aux, (*aux)->content, value, scan))
		{
			free_star(value);
			exit_msg(ms, ERR_ALLOC, EXIT_FAILURE);
		}
	}
	free_star(value);
}
