/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: algalian <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 12:52:13 by algalian          #+#    #+#             */
/*   Updated: 2023/01/30 12:52:15 by algalian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static unsigned int	ft_count(unsigned int m)
{
	int	i;

	i = 1;
	while (m >= 10)
	{
		m = m / 10;
		i++;
	}
	return (i);
}

static unsigned int	ft_sign(int n)
{
	if (n < 0)
	{
		return (2);
	}
	return (1);
}

char	*ft_itoa(int n)
{
	char			*str;
	unsigned int	count;
	unsigned int	m;

	m = ft_abs(n);
	count = ft_count(m) - 1 + ft_sign(n);
	str = malloc((sizeof (char) * count) + 1);
	if (!str)
		return (NULL);
	str[count] = '\0';
	count--;
	while (count > ft_sign(n) - 1)
	{
		str[count] = (m % 10) + '0';
		m = m / 10;
		count--;
	}
	str[ft_sign(n) - 1] = m + '0';
	if (n < 0)
		str[0] = '-';
	return (str);
}
/*int main()
{
	int nb = 1234;
	printf("%s",ft_itoa(nb));
	return(0);
}*/