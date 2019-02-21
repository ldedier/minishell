/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dlstpop.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/10 20:11:41 by ldedier           #+#    #+#             */
/*   Updated: 2019/02/10 21:24:53 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_dlstpop(t_dlist **list)
{
	void	*content;
	t_dlist	*ptr;

	if (*list != NULL)
	{
		ptr = *list;
		content = ptr->content;
		(*list)->prev->next = (*list)->next;
		(*list)->next->prev = (*list)->prev;
		free(content);
		free(ptr);
		if (*list == (*list)->next)
			*list = NULL;
		else
			*list = (*list)->next;
	}
}
