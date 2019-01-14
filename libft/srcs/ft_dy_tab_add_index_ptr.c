/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dy_tab_add_index_ptr.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/14 15:55:30 by ldedier           #+#    #+#             */
/*   Updated: 2019/01/14 16:29:50 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_dy_tab_add_index_ptr(t_dy_tab *d_tab, void *to_add, size_t index)
{
	int i;

	if (d_tab->current_size >= d_tab->max_size)
	{
		if (ft_dy_tab_realloc(d_tab))
			return (1);
	}
	i = d_tab->current_size;
	while ((size_t)i > index)
	{
		d_tab->tab[i] = d_tab->tab[i - 1];
		i--;
	}
	d_tab->tab[i] = to_add;
	d_tab->tab[++d_tab->current_size] = 0;
	return (0);
}
