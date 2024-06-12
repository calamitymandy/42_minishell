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
	char string[] = "tripouille";
	int caracter = 't' + 256;
	printf("%s", ft_strchr(string, caracter));
	printf("\n");
	printf("%s", strchr(string, caracter));
	return(0);
}*/
