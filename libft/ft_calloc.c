/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: algalian <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 14:33:13 by algalian          #+#    #+#             */
/*   Updated: 2023/01/19 14:33:16 by algalian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	void	*ptr;

	ptr = malloc(count * size);
	if (!ptr)
		return (NULL);
	ft_bzero(ptr, count * size);
	return (ptr);
}
/*int main()
{
	unsigned int objetos = 0;
	unsigned int tamaño = 0;
	void *c1;
	void *c2;
	c1 = ft_calloc(objetos, tamaño);
	c2 = calloc (objetos, tamaño);
	printf("%i",memcmp(c1, c2, objetos*tamaño));
	free(c1);
	free(c2);
	return(0);
}*/
