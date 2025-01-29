#include "libft.h"

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
