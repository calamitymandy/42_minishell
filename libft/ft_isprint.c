/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isprint.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: algalian <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 14:34:37 by algalian          #+#    #+#             */
/*   Updated: 2023/01/19 14:34:38 by algalian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

int	ft_isprint(int a)
{
	if (a >= 32 && a < 127)
	{
		return (1);
	}
	return (0);
}

/*int main()
{
	int caracter = 127;
	printf("%i", ft_isprint(caracter));
	write(1, "\n", 1);
	printf("%i", isprint(caracter));
	return(0);
}*/
