/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amdemuyn <amdemuyn@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 18:48:43 by amdemuyn          #+#    #+#             */
/*   Updated: 2025/01/22 21:51:52 by amdemuyn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"



bool    check_long_range(int neg, unsigned long long num, bool *error)
{
    if ((neg == 1 && num > LONG_MAX)
            || (neg == -1 && num > -(unsigned long)LONG_MIN))
        *error = true;
    return (*error);
}

int ft_atoi_long(const char *str, bool *error)
{
    unsigned long long num;
    int neg;
    int i;

    num = 0;
    neg = 1;
    i = 0;
    *error = false;
    while (str[i] && ft_isblank(str[i]))
        i++;
    if (str[i] == '-' || str[i] == '+')
    {
        if (str[i] == '-')
            neg = -1;
        i++;
    }
    while (str[i] && ft_isdigit(str[i]))
    {
        num = (num * 10) + (str[i] - '0');
        if (check_long_range(neg, num, error))
            break ;
        i++;
    }
    return (num * neg);
}

char	*strjoin_n_free(char *s1, char const *s2)
{
	size_t	i;
	size_t	j;
	size_t	len;
	char	*new;

	if (!s1 || !s2)
		return (NULL);
	i = 0;
	j = 0;
	len = ft_strlen(s1) + ft_strlen(s2) + 1;
	new = malloc(sizeof(char) * len);
	if (!new)
		return (NULL);
	while (s1[i])
	{
		new[i] = s1[i];
		i++;
	}
	while (s2[j])
		new[i++] = s2[j++];
	new[i] = 0;
	free(s1);
	return (new);
}