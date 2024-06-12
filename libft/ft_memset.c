/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: algalian <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 14:35:57 by algalian          #+#    #+#             */
/*   Updated: 2023/01/19 14:36:01 by algalian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

void	*ft_memset(void *b, int c, size_t len)
{
	unsigned int	i;
	char			*char_b;

	char_b = (char *) b;
	i = 0;
	while (i < len)
	{
		char_b[i] = c;
		i++;
	}
	return (b);
}

/*int main()
{
	char string[] = "YYYYYYYYYYYYYY";
	char valor = 'X';
	unsigned int slots = 8;
	printf("%s", memset(string, valor, slots));
	printf("\n");
	printf("%s", ft_memset(string, valor, slots));
	return(0);
}*/
