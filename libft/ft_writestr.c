/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_writestr.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: algalian <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 16:27:55 by algalian          #+#    #+#             */
/*   Updated: 2023/03/07 16:27:57 by algalian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_writestr(char *string)
{
	if (!string)
		return ((int) write(1, "(null)", 6));
	ft_putstr_fd(string, 1);
	return ((int) ft_strlen(string));
}
