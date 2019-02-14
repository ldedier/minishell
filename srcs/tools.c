/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/12 22:29:00 by ldedier           #+#    #+#             */
/*   Updated: 2019/02/14 13:35:29 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		putchar_int(int i)
{
	ft_putchar(i);
	return (0);
}

int		reset_shell(int ret)
{
	if (tcsetattr(0, TCSAFLUSH, &g_glob.term_init) == -1)
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

int		get_file_in_dir(char *filename, char *dirname)
{
	DIR				*dir;
	struct dirent	*entry;

	if (!(dir = opendir(dirname)))
		return (0);
	while ((entry = readdir(dir)) != NULL)
	{
		if (!ft_strcmp(entry->d_name, filename))
		{
			closedir(dir);
			return (1);
		}
	}
	closedir(dir);
	return (0);
}
