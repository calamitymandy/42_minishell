/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: algalian <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 14:37:16 by algalian          #+#    #+#             */
/*   Updated: 2023/01/19 14:37:17 by algalian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	while ((s1[i] != '\0' || s2[i] != '\0') && i < n)
	{
		if (s1[i] != s2[i])
		{
			return (((unsigned char *)s1)[i] - ((unsigned char *)s2)[i]);
		}
		i++;
	}
	return (0);
}

/*int main()
{
    char string1[] = "AAAAA";
    char string2[] = "AAAB";
    unsigned int alcance = 9;
    printf("%i", ft_strncmp(string1, string2, alcance));
	printf("\n");
	printf("%i", strncmp(string1, string2, alcance));
}*/
