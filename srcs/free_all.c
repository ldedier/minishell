/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_all.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/23 16:45:53 by ldedier           #+#    #+#             */
/*   Updated: 2019/01/23 16:48:03 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_file(t_file *file)
{
//	ft_printf("%s\n", file->name);
	free(file->name);
	free(file);
}

void	free_file_dlst(void *f, size_t dummy)
{
	(void)dummy;
	free_file((t_file *)f);
}

void	free_all(t_shell *shell)
{
	ft_dy_tab_del(shell->env);
	ft_dlstdel(&shell->choices, &free_file_dlst);
}
