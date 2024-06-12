/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalnum.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: algalian <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 14:33:27 by algalian          #+#    #+#             */
/*   Updated: 2023/01/19 14:33:29 by algalian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

int	ft_isalnum(int c)
{
	if (!((c >= 65 && c <= 90) || (c >= 97 && c <= 122)))
	{
		if (!(c >= 48 && c <= 57))
		{
			return (0);
		}
	}
	return (1);
}

/*int main()
{
	unsigned char  caracter = '1';
	printf("%i", ft_isalnum(caracter));
	printf("%i", isalnum(caracter));
	return(0);
}*/
