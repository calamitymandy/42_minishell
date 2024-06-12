/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: algalian <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 16:08:38 by algalian          #+#    #+#             */
/*   Updated: 2023/03/07 16:08:40 by algalian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_checkplaceholder(char a, va_list args)
{
	int		value;
	char	*hex;

	hex = "0123456789abcdef";
	value = 0;
	if (a == 'c')
		value = ft_writechar(va_arg(args, int));
	else if (a == 's')
		value = ft_writestr(va_arg(args, char *));
	else if (a == 'p')
		value = ft_writepointer(va_arg(args, unsigned long long), hex);
	else if (a == 'd' || a == 'i')
		value = ft_putnb(va_arg(args, int), "0123456789");
	else if (a == 'u')
		value = ft_putnb(va_arg(args, unsigned int), "0123456789");
	else if (a == 'x')
		value = ft_putnb(va_arg(args, unsigned int), hex);
	else if (a == 'X')
		value = ft_putnb(va_arg(args, unsigned int), "0123456789ABCDEF");
	else if (a == '%')
		value = write(1, "%", 1);
	return (value);
}

int	ft_printf(char const *item, ...)
{
	va_list			args;
	unsigned int	i;
	unsigned int	count;

	va_start(args, item);
	count = 0;
	i = 0;
	while (i < ft_strlen(item))
	{
		if (item[i] == '%')
		{
			count += ft_checkplaceholder(item[i + 1], args);
			i = i + 2;
		}
		if (i >= ft_strlen(item))
			break ;
		if (item[i] != '%')
		{
			count += write(1, &item[i], 1);
			i++;
		}
	}
	va_end(args);
	return (count);
}
/*int main()
{
	ft_printf("patatuelas");
	return(0);
}*/
