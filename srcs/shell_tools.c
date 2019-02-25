/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_tools.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/24 22:50:14 by ldedier           #+#    #+#             */
/*   Updated: 2019/02/25 23:32:46 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		putchar_int(int i)
{
	ft_putchar_fd(i, 0);
	return (0);
}

int		reset_shell(int ret)
{
	if (tcsetattr(0, TCSAFLUSH, &g_glob.term_init) == -1)
		return (-1);
	return (ret);
}

int		set_shell(int ret)
{
	if (tcsetattr(0, TCSADRAIN, &g_glob.term) == -1)
		return (-1);
	return (ret);
}

void	move(int x, int y)
{
	char *res;

	res = tgetstr("cm", NULL);
	tputs(tgoto(res, x, y), 1, putchar_int);
}

int		clear_all(void)
{
	char *res;

	if (!(res = tgetstr("cl", NULL)))
		return (-1);
	tputs(res, 1, putchar_int);
	return (0);
}
