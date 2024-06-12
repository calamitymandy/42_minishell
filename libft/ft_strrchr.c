/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: algalian <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 14:37:42 by algalian          #+#    #+#             */
/*   Updated: 2023/01/19 14:37:43 by algalian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int		i;
	char	b;

	b = c;
	i = ft_strlen(s);
	while (i >= 0)
	{
		if (s[i] == b)
		{
			return ((char *)s + i);
		}
		i--;
	}
	return (NULL);
}

/*int main()
{

	const char string[] = "tripouille";
	int caracter = 't' + 256;
    printf ("%s", ft_strrchr(string, caracter));
	printf("\n");
	printf("%s", strrchr(string, caracter));
    return(0);
}*/