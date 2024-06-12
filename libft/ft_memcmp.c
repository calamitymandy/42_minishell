/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: algalian <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 14:35:04 by algalian          #+#    #+#             */
/*   Updated: 2023/01/19 14:35:06 by algalian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		if (((unsigned char *) s1)[i] != ((unsigned char *) s2)[i])
		{
			return (((unsigned char *) s1)[i] - ((unsigned char *) s2)[i]);
		}
		i++;
	}
	return (0);
}

/*int main()
{
	char string1[] = "";
	char string2[] = "";
	unsigned int alcance = 2;
	printf("memcmp:%i", memcmp(string1,string2,alcance));
	printf("\n");
	printf("ft_memcmp: %i", ft_memcmp(string1,string2,alcance));
	return(0);
}*/
