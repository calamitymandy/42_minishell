/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_writepointer.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: algalian <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 16:34:19 by algalian          #+#    #+#             */
/*   Updated: 2023/03/07 16:34:21 by algalian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_writepointer(unsigned long long p, char *sym)
{
	int	value;

	value = 0;
	write(1, "0x", 2);
	if (p >= 16)
		value += ft_putnb(p / 16, sym);
	value += write(1, &(sym[p % 16]), 1);
	return (value + 2);
}
