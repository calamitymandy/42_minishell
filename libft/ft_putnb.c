/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnb.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: algalian <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 16:37:27 by algalian          #+#    #+#             */
/*   Updated: 2023/03/07 16:37:29 by algalian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putnb(long long nb, char *sym)
{
	unsigned int	base;
	int				value;

	value = 0;
	base = ft_strlen(sym);
	if (nb == -2147483648)
	{
		value += ft_putnb(nb / base, sym);
		write(1, &(sym[-(nb % base)]), 1);
		return (value + 1);
	}
	if (nb < 0)
	{
		write(1, "-", 1);
		value += ft_putnb(-nb, sym);
		return (value + 1);
	}
	if (nb >= base)
		value += ft_putnb(nb / base, sym);
	value += write(1, &(sym[nb % base]), 1);
	return (value);
}
