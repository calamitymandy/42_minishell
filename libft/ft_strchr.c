/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: algalian <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 14:36:14 by algalian          #+#    #+#             */
/*   Updated: 2023/01/19 14:36:16 by algalian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"
#include <stdio.h>

char	*ft_strchr(const char *s, int c)
{
	unsigned int	i;
	char			b;

	b = c;
	i = 0;
	while (i < ft_strlen(s) + 1)
	{
		if (s[i] == b)
		{
			return ((char *)s + i);
		}
		i++;
	}
	return (NULL);
}

/*int main()
{
	char string[] = "1234 $asdf 98234kjhss";
	int caracter = '$';
	printf("%s", ft_strchr(string, caracter));
	return(0);
}*/
