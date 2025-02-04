/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_arrdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: algalian <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 17:23:07 by algalian          #+#    #+#             */
/*   Updated: 2025/02/04 17:23:09 by algalian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	**ft_arrdup(char **arr)
{
	char	**newarr;
	size_t	i;

	i = 0;
	while (arr[i])
		i++;
	newarr = ft_calloc(sizeof(char *), i + 1);
	if (!newarr)
		return (NULL);
	i = 0;
	while (arr[i])
	{
		newarr[i] = ft_strdup(arr[i]);
		if (!newarr[i])
		{
			free_string(newarr);
			return (NULL);
		}
		i++;
	}
	newarr[i] = NULL;
	return (newarr);
}
