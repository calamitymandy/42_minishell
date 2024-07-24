/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: algalian <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 17:47:41 by algalian          #+#    #+#             */
/*   Updated: 2024/07/02 17:47:44 by algalian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*fit(char *alias, char *var, char *p)
{
	int		j;
	int		k;
	char	*t;

	t = malloc(sizeof(char) * (ft_strlen(p)
				+ (ft_strlen(var) - ft_strlen(alias))));
	j = 0;
	while (j < ft_strlen(var))
	{
		t[j] = var[j];
		j++;
	}
	k = ft_strlen(alias);
	while (j < ft_strlen(p) + (ft_strlen(var) - ft_strlen(alias)))
	{
		t[j] = p[k];
		k++;
		j++;
	}
	return (t);
}

static char	*skip(char *p)
{
	int		j;
	int		k;
	char	*t;

	j = 0;
	while (p[j] && p[j] != 32)
		j++;
	t = malloc(sizeof(char) * (ft_strlen(p) - j));
	k = 0;
	while (p[j])
	{
		t[k] = p[j];
		k++;
		j++;
	}
	return (t);
}

static void	match_pattern(char *p, char *alias, char *var, char **x)
{
	char	*temp;
	char	*t;
	char	*old_x;
	int		j;
	int		k;

	j = 0;
	while (p[j] && p[j] == alias[j])
		j++;
	if (j == ft_strlen(alias) && (p[j] == 32 || !p[j]))
		t = fit(alias, var, p);
	else if (p[0] == 32)
		t = ft_strjoin("$",p);
	else
		t = skip(p);
	old_x = *x;
	temp = ft_strjoin(*x, t);
	*x = temp;
	free(t);
	if (old_x && *old_x != '\0') 
		free(old_x);
}

char	*expand(char *s, char *alias, char *var)
{
	char	**p;
	char	*x;
	char	*temp;
	int		i;

	if (!ft_strchr(s, '$'))
		return (s);
	temp = NULL;
	p = ft_split(s, '$');
	i = 0;
	if (s[i] != '$')
	{
		x = p[i];
		i++;
	}
	else
		x = "\0";
	while (p[i])
	{
		match_pattern(p[i], alias, var, &x);
		free(p[i]);
		i++;
	}
	free(p);
	if(s[ft_strlen(s) - 1] == '$')
	{
		temp = ft_strdup(x);
		free(x);
		x = ft_strjoin(temp, "$");
	}
	if(temp)
		free(temp);
	return (x);
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
	if(s != argv[1])
		free(s);
	return(0);
}*/