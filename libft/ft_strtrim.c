/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: algalian <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 19:42:33 by algalian          #+#    #+#             */
/*   Updated: 2023/01/24 19:42:37 by algalian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

static int	ft_left(char const *s1, char const *set)
{
	unsigned int	i;

	i = 0;
	while (ft_strchr(set, s1[i]) != NULL && i < ft_strlen(s1))
	{
		i++;
	}
	return (i);
}

static int	ft_right(char const *s1, char const *set)
{
	unsigned int	i;

	i = ft_strlen(s1);
	i--;
	while (ft_strchr(set, s1[i]) != NULL)
	{
		i--;
	}
	return (i);
}

static char	*ft_write(char *trimmed, char const *s1, int left, int size)
{
	int	k;	

	k = 0;
	while (k < size + 1)
	{
		trimmed[k] = s1[left];
		k++;
		left++;
	}
	trimmed[k] = '\0';
	return (trimmed);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	unsigned int	left;
	unsigned int	size;
	char			*trimmed;

	left = ft_left(s1, set);
	if (left >= ft_strlen(s1))
	{
		trimmed = malloc(1);
		if (!trimmed)
			return (NULL);
		trimmed[0] = '\0';
		return (trimmed);
	}
	size = ft_right(s1, set) - left;
	trimmed = malloc((sizeof(char) * size) + 2);
	if (!trimmed)
	{
		return (NULL);
	}
	return (ft_write(trimmed, s1, left, size));
}

/*int main()
{
	char string[] = "AAAAAA";
	char set []= "";
	printf("%s", ft_strtrim(string, set));
	return(0);
	system("leaks a.out");

}*/