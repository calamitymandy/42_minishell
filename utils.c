/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amdemuyn <amdemuyn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 18:48:43 by amdemuyn          #+#    #+#             */
/*   Updated: 2024/11/19 17:19:00 by amdemuyn         ###   ########.fr       */
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
