/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: algalian <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 14:32:42 by algalian          #+#    #+#             */
/*   Updated: 2023/01/19 14:32:45 by algalian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

void	ft_bzero(void *s, size_t n)
{
	size_t	i;
	char	*c;

	c = (char *) s;
	i = 0;
	while (i < n)
	{
		c[i] = '\0';
		i++;
	}
}

/*int	main()
{
	char string [] = "111111111111";
	S = 11;
	ft_bzero(string, slots);
	printf("%s", string);
	return(0);
}*/
