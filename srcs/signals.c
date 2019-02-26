/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/13 19:59:05 by ldedier           #+#    #+#             */
/*   Updated: 2019/02/27 00:33:31 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_resize(int signal)
{
	(void)signal;
	ioctl(0, TIOCGWINSZ, &g_glob.winsize);
	render_command_line(g_glob.command, 0);
}

void	init_signals(void)
{
	signal(SIGWINCH, handle_resize);
	signal(SIGALRM, transmit_sig_and_die);
	signal(SIGTERM, transmit_sig_and_die);
	signal(SIGHUP, transmit_sig_and_die);
	signal(SIGABRT, transmit_sig_and_die);
	signal(SIGCONT, transmit_sig);
	signal(SIGQUIT, transmit_sig);
	signal(SIGTSTP, transmit_sig);
	signal(SIGSTOP, transmit_sig);
	signal(SIGKILL, transmit_sig);
	signal(SIGINT, transmit_sig);
}
