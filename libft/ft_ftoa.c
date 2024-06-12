/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ftoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: algalian <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 19:14:21 by algalian          #+#    #+#             */
/*   Updated: 2023/11/03 19:14:27 by algalian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_ftoa(double n)
{
	char	*s;
	char	*p;
	int		i;

	s = ft_itoa(ft_truncf(n));
	p = malloc(sizeof(char) * 7);
	i = 1;
	n = ft_fabs(n);
	n -= ft_truncf(n);
	p[0] = '.';
	while (i < 7)
	{
		n *= 10;
		p[i] = (int) ft_truncf(n) + '0';
		n -= ft_truncf(n);
		i++;
	}
	p[7] = '\0';
	return (ft_strjoin(s, p));
}
