/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: algalian <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 14:34:51 by algalian          #+#    #+#             */
/*   Updated: 2023/01/19 14:34:52 by algalian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned int	i;
	const char		*char_s;
	char			b;

	b = c;
	char_s = (char *) s;
	i = 0;
	while (i < n)
	{
		if (char_s[i] == b)
		{
			return ((void *)s + i);
		}
		i++;
	}
	return (NULL);
}
