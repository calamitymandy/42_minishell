/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ratio.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: algalian <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 19:09:15 by algalian          #+#    #+#             */
/*   Updated: 2023/12/14 19:09:17 by algalian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_ratio	ft_ratio(double n)
{
	t_ratio	input;
	t_ratio	output;
	int		i;

	input.num = ft_fabs(n) * 10000;
	input.den = 10000;
	i = 1;
	output.num = input.num;
	output.den = input.den;
	while (i < input.num / 2 && i < input.num / 2)
	{
		if (output.num % i == 0 && output.den % i == 0)
		{
			output.num /= i;
			output.den /= i;
			i = 1;
		}
		i++;
	}
	return (output);
}

/*int main()
{
	t_ratio ratio;

	ratio = ft_ratio();

	printf("%i/%i\n", ratio.num, ratio.den);
}*/
