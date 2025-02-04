/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_var.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: algalian <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 19:34:56 by algalian          #+#    #+#             */
/*   Updated: 2025/02/04 19:34:58 by algalian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	isalphanum_or_underscore(char c)
{
	if (!ft_isalnum(c) && c != '_')
		return (false);
	else
		return (true);
}

int	var_name_len(char *content)
{
	int		i;
	int		len;

	i = 0;
	while (content[i] != '$')
		i++;
	i++;
	if ((content[i] >= '0' && content[i] <= '9') || content[i] == '?')
		return (1);
	len = 0;
	while (content[i])
	{
		if (!isalphanum_or_underscore(content[i]))
			break ;
		len++;
		i++;
	}
	return (len);
}

char	*get_var_str(char *content, char *value, int trim_len, int scan)
{
	int		i;
	int		j;
	char	*new;

	i = 0;
	j = 0;
	new = malloc(sizeof(char) * trim_len);
	if (!new)
		return (NULL);
	while (content[i])
	{
		if (content[i] == '$' && i == scan)
		{
			val_cpy(new, value, &j);
			i = i + var_name_len(content + scan) + 1;
			if (content[i] == '\0')
				break ;
		}
		new[j++] = content[i++];
	}
	new[j] = '\0';
	return (new);
}
