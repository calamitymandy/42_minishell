/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isblank.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: algalian <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 17:22:40 by algalian          #+#    #+#             */
/*   Updated: 2025/02/04 17:22:43 by algalian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isblank(char c)
{
	if ((c >= 9 && c <= 13) || c == ' ')
	{
		return (1);
	}
	return (0);
}
