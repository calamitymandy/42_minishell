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
	i = 0;
	while(s[i] && s[i] != '$')
		i++;
	j = i;
	while(s[j] && s[j] != 32)
		j++;
	aux = ft_strjoin("$",alias);
	match = ft_strjoin(aux, " ");
	f = ft_strnstr(s, match, (size_t) ft_strlen(s));
	l = ft_substr(s, 0, i);
	r = ft_substr(s, j, ft_strlen(s));
	if(f || (ft_strnstr(s, aux,ft_strlen(s)) &&(!s[j] || s[j] == 32)) && ft_strncmp(ft_strchr(s,'$'),aux,ft_strlen(ft_strchr(s,'$'))) <= 0)
	{
		printf("So the substring is definetely there\n");
		free(aux);
		aux = var;
		match = ft_strjoin(l, aux);
		x = ft_strjoin(match, r);
		free(match);
		free(l);
		free(r);
		expand(x, alias, var);
	}
    else
	{
		printf("The substring isn't there\n");
		x = ft_strjoin(l, r);
		free(aux);
		free(match);
		free(l);
		free(r);
		expand(x,alias, var); 
	}
}

int main(int argc, char **argv)
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