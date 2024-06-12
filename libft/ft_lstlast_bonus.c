/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstlast_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: algalian <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 16:03:56 by algalian          #+#    #+#             */
/*   Updated: 2023/02/16 16:03:57 by algalian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstlast(t_list *lst)
{
	t_list	*pointer;

	if (ft_lstsize(lst) == 0)
		return (NULL);
	if (ft_lstsize(lst) == 1)
		return (lst);
	pointer = lst;
	while (pointer->next != NULL)
	{
		pointer = pointer->next;
	}
	return (pointer);
}
