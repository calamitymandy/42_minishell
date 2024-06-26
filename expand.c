#include<stdio.h>
#include<stdlib.h>
#include"./libft/libft.h"

char  *expand(char *s, char *alias, char *var)
{

	char	*l;
	char	*r;
	char	*x;
	char	*f;
	char	*match;
	char	*aux;
	int		i;
	int		j;

	f = ft_strchr(s, '$');
	if(!f || s[ft_strlen(s) - 1] == '$' || f[1] == 32)
	{
		printf("Null or blank ended, or not found\n");
		return(s);
	}
	aux = ft_strjoin("$",alias);
	match = ft_strjoin(aux, " ");
	f = ft_strnstr(s, match, (size_t) ft_strlen(s));
	if(!f) 
	{
		printf("So the substring isn't there\n");
		i = 0;
		while(s[i] && s[i] != '$')
			i++;
		j = i;
		while(s[j] && s[j] != 32)
			j++;
		l = ft_substr(s, 0, i);
		r = ft_substr(s, j, ft_strlen(s));
		x = ft_strjoin(l, r);
		free(aux);
		free(match);
		free(l);
		free(r);
		free(s);
		return(x); 
	}
	else
		return(s);
}

/*int main(int argc, char **argv)
{
	char *s;

	if(argc != 2)
	{
		printf("arg error\n");
		return(0);
	}
	s = expand(argv[1], "asdf", "VARIABLE");
	printf("%s\n",s);
	return(0);
}
*/