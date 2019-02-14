/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/13 19:59:05 by ldedier           #+#    #+#             */
/*   Updated: 2019/02/14 13:39:01 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_kill(int signal)
{
	(void)signal;
	ft_dprintf(2, "HANDLE SIGKILL\n");
	exit(reset_shell(0));
}

void	handle_resize(int signal)
{
	(void)signal;
	ioctl(0, TIOCGWINSZ, &g_glob.winsize);
}

void	init_signals(void)
{
	signal(SIGWINCH, handle_resize);
	signal(SIGQUIT, handle_kill);
	signal(SIGTSTP, handle_kill);
	signal(SIGSTOP, handle_kill);
	signal(SIGCONT, handle_kill);
	signal(SIGINT, handle_sigint);
}