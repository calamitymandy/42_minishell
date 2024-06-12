/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pow.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: algalian <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 18:47:28 by algalian          #+#    #+#             */
/*   Updated: 2023/12/06 18:47:30 by algalian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

static long double	ft_basic_pow(int base, int exp)
{
	int			i;
	int			x;
	long double	res;

	i = 1;
	res = base;
	x = ft_fabs(exp);
	while (i < x)
	{
		res = res * base;
		i++;
	}
	if (exp < 0)
		return (1 / res);
	return (res);
}

static double	ft_float_pow(double base, int exp)
{
	t_ratio	ratio;
	double	res;

	ratio = ft_ratio(base);
	res = ft_basic_pow(ratio.num, ft_fabs(exp))
		/ ft_basic_pow(ratio.den, ft_fabs(exp));
	if (exp < 0)
		return (1 / res);
	return (res);
}

static double	ft_reverse_pow(double base, double exp)
{
	double	rad;
	double	res;
	t_ratio	ratio;

	ratio = ft_ratio(ft_fabs(exp));
	rad = ft_float_pow(base, ratio.num);
	res = 0;
	while (ft_basic_pow(res, ratio.den) < rad)
		res++;
	res--;
	while (ft_float_pow(res, ratio.den) < rad)
		res += 0.0001;
	if (exp < 0)
		return (1 / res);
	return (res);
}

double	ft_pow(double base, double exp)
{
	double	b;
	double	x;

	if (exp == 0)
	{
		if (base == 0)
		{
			write(1, "0^0 undefined", 14);
			exit(1);
		}
		return (1);
	}
	b = ft_fabs(base);
	x = ft_fabs(exp);
	if (ft_truncf(b) < x)
	{
		if (ft_truncf(x) < x)
			return (ft_reverse_pow(base, exp));
		return (ft_float_pow(base, exp));
	}
	if (ft_truncf(x) < x)
		return (ft_reverse_pow(base, exp));
	return (ft_basic_pow(base, exp));
}

/*int main()
{
	double base = 2.5;
	double exp  = -3.5;
	printf("%f", ft_pow(base, exp));
}*/
