/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_term.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/14 12:50:17 by ldedier           #+#    #+#             */
/*   Updated: 2019/02/25 23:38:45 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		init_terminal_data(char **env)
{
	char	*termtype;
	int		success;

	termtype = get_env_value(env, "TERM");
	if (termtype == NULL)
	{
		ft_dprintf(2, "Specify a terminal type with 'export TERM'\n");
		return (1);
	}
	success = tgetent(NULL, termtype);
	if (success < 0)
	{
		ft_dprintf(2, "Could not access the termcap data base\n");
		return (1);
	}
	if (success == 0)
	{
		ft_dprintf(2, "Terminal type `%s' is not defined\n", termtype);
		return (1);
	}
	return (0);
}

int		init_terminal(char **env)
{
	if (init_terminal_data(env))
		return (1);
	if (tcgetattr(0, &g_glob.term) == -1)
	{
		ft_dprintf(2, "could not copy the terminal attributes\n");
		return (1);
	}
	if (tcgetattr(0, &g_glob.term_init) == -1)
	{
		ft_dprintf(2, "could not copy the terminal attributes\n");
		return (1);
	}
	g_glob.term.c_lflag &= ~(ICANON);
	g_glob.term.c_lflag &= ~(ECHO);
	g_glob.term.c_lflag |= ISIG;
	g_glob.term.c_lflag &= ~(ISIG);
	g_glob.term.c_cc[VMIN] = 1;
	g_glob.term.c_cc[VTIME] = 0;
	if (tcsetattr(0, TCSADRAIN, &g_glob.term) == -1)
	{
		ft_dprintf(2, "could not modify this terminal attributes\n");
		return (1);
	}
	ioctl(0, TIOCGWINSZ, &g_glob.winsize);
	return (0);
}
