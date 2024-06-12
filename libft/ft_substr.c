/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: algalian <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 15:09:43 by algalian          #+#    #+#             */
/*   Updated: 2023/01/24 15:09:47 by algalian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char			*substr;
	unsigned int	i;

	if (start >= ft_strlen(s) || len >= ft_strlen(s))
	{
		len = ft_strlen(s);
	}
	substr = (char *)malloc((sizeof(char) * (len + 1)));
	if (!substr || !s)
		return (NULL);
	i = 0;
	while (i < len && start < ft_strlen(s))
	{
		substr[i] = s[start];
		start++;
		i++;
	}
	substr[i] = '\0';
	return (substr);
}

/*int main()
{
	char const string[] = "tripouille";
	unsigned int start = 0;
	unsigned int end = 42000;
	printf("%s", ft_substr(string, start, end));
	return(0);
}*/