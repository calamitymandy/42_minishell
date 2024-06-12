#include<stdio.h>
#include<stdlib.h>
#include"./libft/libft.h"



int main(int argc, char **argv)
{
	int	i;
	int	j;
	char *var = "VARIABLE";
	char *alias = "a";
	int	len;
	char *pos;

	i = 1;
	len = ft_strlen(alias);
	while(argv[i])
    {
		if(!ft_strrchr(argv[i], '$'))
			ft_printf("%s",argv[i]);
		else
		{
			pos = ft_strnstr(argv[i],ft_strjoin("$",alias),len);
			while(argv[i][j] != '$' && argv[i][j])
				ft_printf("%c", argv[i][j++]);
			if(pos)
				ft_printf("%s",var);
		}
		if(argv[i + 1])
			ft_printf(" ");
		i++;
	}
	return(0);
}