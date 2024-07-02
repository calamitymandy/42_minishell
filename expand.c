#include<stdio.h>
#include<stdlib.h>
#include"./libft/libft.h"


/*char	*expand(char *s, char *alias, char *var)
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
	if(!f || !f[1])
		return(s);
	i = 0;
	while(s[i] && s[i] != '$')
		i++;
	j = i + 1;
	while(s[j] && s[j] != 32 && s[j] != '$') 
		j++;
	printf("Me salgo en *%c* (j = %i)\n", s[j], j);
	aux = ft_strjoin("$",alias);
	match = ft_strjoin(aux, " ");
	f = ft_strnstr(s, match, (size_t) ft_strlen(s));
	l = ft_substr(s, 0, i);
	r = ft_substr(s, j, ft_strlen(s));
	if(s[ft_strlen(s) - 1] == '$')
		r = ft_strjoin(r,"$");
	if(f || ft_strnstr(s, aux,ft_strlen(s)) && ((!s[j] || s[j] == 32) && (ft_strncmp(ft_strchr(s,'$'),aux,ft_strlen(ft_strchr(s,'$'))) <= 0 || s[ft_strlen(s) - 1] == '$')))
	{
		printf("So the substring is definetely there\n");
		free(aux);
		aux = var;
		match = ft_strjoin(l, aux);
		x = ft_strjoin(match, r);

	}
	else
	{
		printf("the substring isn't there\n");
		x = ft_strjoin(l, r);
		free(aux);
	}
	free(match);
	free(l);
	free(r);
	expand(x, alias, var);
}*/

char *expand(char *s, char *alias, char *var)
{
	char	**p;
	char	*t;
	char	*x;
	char	*temp;
	int		i;
	int		j;
	int		k;
	
	if(!ft_strchr(s, '$'))
		return(s);
	p = ft_split(s, '$');
	i = 0;
	if(s[i] != '$')
	{	
		x = ft_strdup(p[i]);
		i++;
	}
	else
		x = ft_strdup("");
	while(p[i])
	{
		j = 0;
		while(p[i][j] && p[i][j] == alias[j])
		{
			j++;
		}
		//printf("%li, %i, %c\n", ft_strlen(alias), j, p[i][j + 1]);
		if(j == ft_strlen(alias) && (p[i][j + 1] == 32 || !p[i][j + 1]))
		{
			t = malloc(sizeof(char) * (ft_strlen(p[i]) + (ft_strlen(var) - ft_strlen(alias))));
			//printf("%i\n",(int) (ft_strlen(p[i]) + (ft_strlen(var) - ft_strlen(alias))));
			j = 0;
			while(j < ft_strlen(var))
			{
				t[j] = var[j];
				//printf("%c\n", t[j]);
				j++;
			}
			k = ft_strlen(alias);
			while(j < ft_strlen(p[i]) + (ft_strlen(var) - ft_strlen(alias)))				
			{
				t[j] = p[i][k];
				//printf("%c\n", t[j]);
				k++;
				j++;	
			}
			//printf("it's there %s\n", t);
		}
		else
		{
			j = 0; 
			while(p[i][j] && p[i][j] != 32)
			{
				j++;
			}
			t = malloc(sizeof(char) * (ft_strlen(p[i]) - j));
			k = 0;
			while(p[i][j])
			{
				t[k] = p[i][j];
				k++;
				j++;
			}
			//printf("it isn't\n");
		}
		temp = ft_strjoin(x, t);
		x = temp;
		i++;
	}
	free_string(p);
	return(x); 
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
	system("leaks -q minishell");
	return(0);
}