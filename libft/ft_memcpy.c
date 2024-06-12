/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: algalian <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 14:35:20 by algalian          #+#    #+#             */
/*   Updated: 2023/01/19 14:35:22 by algalian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	unsigned int	i;
	char			*char_dst;
	const char		*char_src;

	if (!dst && !src)
	{
		return (0);
	}
	char_src = (char *) src;
	char_dst = (char *) dst;
	i = 0;
	while (i < n)
	{
		char_dst[i] = char_src[i];
		i++;
	}
	return (char_dst);
}

/*int main()
{
	char destino [] = "zzzzzzzzzzzzzzzzzzzzzzzzzzzzz";
	char origen [] = "abcdefg";
	unsigned int slots = 3;
	printf("ft_memcpy: %s", ft_memcpy(destino, origen, slots));
	printf("\n");
	printf("memcpy: %s", memcpy(destino, origen, slots));
	return(0);
}*/
