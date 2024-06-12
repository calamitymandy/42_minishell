/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: algalian <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 14:36:30 by algalian          #+#    #+#             */
/*   Updated: 2023/01/19 14:36:32 by algalian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

char	*ft_strdup(const char *s1)
{
	char			*s2;

	s2 = malloc((ft_strlen(s1) * sizeof(const char)) + 1);
	if (!s2)
	{
		ft_printf("Error\n");
		exit(1);
	}
	ft_strlcpy(s2, s1, ft_strlen(s1) + 1);
	s2[ft_strlen(s1)] = '\0';
	return (s2);
}

/*int main()
{   
	char string[] = "Cópiame ésta";
	printf("ft_strd:%s",ft_strd(string));
	printf("\n");
	printf("strd:%s", strd(string));
	return(0);
}*/
