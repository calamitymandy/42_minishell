/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: algalian <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 14:36:54 by algalian          #+#    #+#             */
/*   Updated: 2023/01/19 14:36:55 by algalian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t	i;

	i = 0;
	if (dstsize == 0)
	{
		return (ft_strlen(src));
	}
	while (src[i] != '\0' && i < dstsize - 1)
	{
		dst[i] = src[i];
		i++;
	}
	if (dstsize > i)
	{
		dst[i] = '\0';
	}
	return (ft_strlen(src));
}

/*int main()
{
	char origen[] = "AAAAAAAAA";
	char destino[] = "BBBBBBB";
	size_t size = 35;
	//printf("%lu", ft_strlcpy(destino, origen,size));//
	printf("\n");
	printf("%lu", strlcpy(destino, origen, size));
	printf("\n");
	printf("%s", destino);
	return(0);
}*/
