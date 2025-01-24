/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: algalian <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 13:28:25 by algalian          #+#    #+#             */
/*   Updated: 2023/01/19 13:28:29 by algalian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

int	ft_atoi(const char *str)
{
	int				i;
	int				neg;
	unsigned long	res;

	i = 0;
	neg = 1;
	res = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			neg *= -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		res = (str[i] - 48) + (res * 10);
		if (res > 9223372036854775808UL && neg == -1)
			return (0);
		if (res > 9223372036854775808UL && neg == 1)
			return (-1);
		i++;
	}
	return (res * neg);
}

/*int main(int argc, char **argv)
{

	printf("%li\n", ft_atoi(argv[1]));
	printf("%i\n", atoi(argv[1]));
	return(0);
}*/