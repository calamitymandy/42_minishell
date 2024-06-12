/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: algalian <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 14:35:46 by algalian          #+#    #+#             */
/*   Updated: 2023/01/19 14:35:49 by algalian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t n)
{
	char			*char_dst;
	const char		*char_src;

	if (dst == src)
	{
		return (dst);
	}
	char_dst = (char *) dst;
	char_src = (char *) src;
	if (dst < src)
	{
		ft_memcpy(dst, src, n);
	}
	if (dst > src)
	{
		while (n > 0)
		{
			n--;
			char_dst[n] = char_src[n];
		}
	}
	return (dst);
}

/*int main()
{
	const char origen[20] = "abcd";
	char destino[20] = "123456789";
	size_t slots = 4;
	ft_memmove(destino, origen, slots);
	printf("%s", destino);
	return(0);
}*/
