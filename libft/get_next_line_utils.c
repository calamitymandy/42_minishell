#include"libft.h"

static size_t	ft_strlen(char *s)
{
	size_t	i;

	i = 0;
	if (!s)
		return (0);
	while (s[i])
		i++;
	return (i);
}

static char	*ft_strchr(char *s, int c)
{
	if (!s)
		return (NULL);
	while (*s && *s != (unsigned char)c)
		s++;
	if (*s == (unsigned char)c)
		return ((char *)s);
	return (NULL);
}

static char	*ft_strjoin(char *first_str, char *buf)
{
	size_t	i;
	size_t	j;
	char	*str;

	if (!first_str)
	{
		first_str = (char *)malloc(1 * sizeof(char));
		first_str[0] = '\0';
	}
	if (!first_str || !buf)
		return (NULL);
	str = malloc(sizeof(char) * ((ft_strlen(first_str) + ft_strlen(buf)) + 1));
	if (str == NULL)
		return (NULL);
	i = -1;
	j = 0;
	if (first_str)
		while (first_str[++i] != '\0')
			str[i] = first_str[i];
	while (buf[j] != '\0')
		str[i++] = buf[j++];
	str[i++] = '\0';
	free(first_str);
	return (str);
}

char	*ft_line(char *str)
{
	int		i;
	char	*line;

	i = 0;
	if (!str[i])
		return (NULL);
	while (str[i] && str[i] != '\n')
		i++;
	line = (char *)malloc(sizeof(char) * (i + 2));
	if (!line)
		return (NULL);
	i = 0;
	while (str[i] && str[i] != '\n')
	{
		line[i] = str[i];
		i++;
	}
	if (str[i] == '\n')
	{
		line[i] = str[i];
		i++;
	}
	line[i] = '\0';
	return (line);
}

char	*ft_next_line(char *str)
{
	int		i;
	int		j;
	char	*stash;

	i = 0;
	j = 0;
	while (str[i] && str[i] != '\n')
		i++;
	if (!str[i])
	{
		free(str);
		return (NULL);
	}
	stash = (char *)malloc(sizeof(char) * (ft_strlen(str) - i + 1));
	if (!stash)
		return (NULL);
	i++;
	while (str[i])
		stash[j++] = str[i++];
	stash[j] = '\0';
	free(str);
	return (stash);
}