/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dy_tab_realloc.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/14 16:00:04 by ldedier           #+#    #+#             */
/*   Updated: 2019/01/23 17:39:50 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_dy_tab_realloc(t_dy_tab *d_tab)
{
	void **tmp;

	if (!(tmp = (void **)ft_memalloc(sizeof(void *) *
			((d_tab->max_size * 2) + 1))))
		return (1);
	ft_memcpy(tmp, d_tab->tab, sizeof(void *) * d_tab->current_size);
	free(d_tab->tab);
	d_tab->tab = tmp;
	d_tab->max_size *= 2;
	return (0);
}
