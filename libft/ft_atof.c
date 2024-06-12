/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atof.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: algalian <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 20:47:03 by algalian          #+#    #+#             */
/*   Updated: 2023/11/02 20:47:11 by algalian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_error(int errorid)
{
	if (errorid == 3)
		ft_printf("Please specify numeric values\n");
	return (1);
}

static int	ft_blanks(char c)
{
	if ((c >= 9 && c <= 13) || c == ' ')
	{
		return (1);
	}
	return (0);
}

static int	clean_string(const char *str, unsigned int i)
{
	if (ft_isdigit(str[i]) == 1)
		return (i);
	if (ft_blanks(str[i]) != 1 && (str[i] != '+' && str[i] != '-'))
		if (ft_isdigit(str[i]) != 1)
			return (-1);
	if (str[i] == '+' || str[i] == '-')
	{
		if (ft_isdigit(str[i + 1]) != 1)
			return (-1);
		return (i + 1);
	}
	if (ft_blanks(str[i]) == 1)
	{
		while (ft_blanks(str[i]) == 1)
			i++;
		return (clean_string(str, i));
	}
	return (-1);
}

static int	sign(const char *str, unsigned int i)
{
	if (i == 0)
		return (1);
	if (str[i - 1] == '-')
	{
		return (-1);
	}
	return (1);
}

double	ft_atof(const char *str)
{
	int		i;
	double	num;
	double	nextf;
	int		pos;

	i = clean_string(str, 0);
	if (i < 0)
		exit(ft_error(3));
	num = str[i] - '0';
	while (ft_isdigit(str[i + 1]) == 1)
	{
		nextf = str[i++] - '0';
		num = num * 10 + nextf;
	}
	pos = 0;
	while (ft_isdigit(str[i + 1]) == 1)
	{
		nextf = str[i++] - '0';
		num = num * 10 + nextf;
		pos++;
	}
	return (num * sign(str, clean_string(str, 0)) / ft_pow(10, pos));
}
