/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dlstpop_ptr.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/10 21:25:43 by ldedier           #+#    #+#             */
/*   Updated: 2019/02/10 21:27:38 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_dlstpop_ptr(t_dlist **list)
{
	void	*content;
	t_dlist	*ptr;

	if (*list != NULL)
	{
		ptr = *list;
		content = ptr->content;
		(*list)->prev->next = (*list)->next;
		(*list)->next->prev = (*list)->prev;
		free(ptr);
		if (*list == (*list)->next)
			*list = NULL;
		else
			*list = (*list)->next;
		return (content);
	}
	return (NULL);
}
