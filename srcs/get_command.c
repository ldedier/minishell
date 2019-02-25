/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_command.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/14 14:31:34 by ldedier           #+#    #+#             */
/*   Updated: 2019/02/24 22:43:46 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		render_command_line(t_dy_str *command, int cursor_inc)
{
	char	*str;

	go_up_to_prompt(g_glob.winsize.ws_col, g_glob.cursor);
	str = tgetstr("cd", NULL);
	tputs(str, 1, putchar_int);
	ft_printf("%s%s%s%s", BOLD, CYAN, PROMPT, EOC);
	ft_printf("%s", command->str);
	g_glob.cursor += cursor_inc;
	replace_cursor_after_render();
	return (0);
}

int		add_to_command(t_dy_str *command,
		unsigned char buffer[READ_BUFF_SIZE], int nb_bytes)
{
	int i;

	i = 0;
	while (i < nb_bytes)
	{
		if (ft_dy_str_add_index(command, (char)buffer[i],
					command->current_index++))
			return (1);
		i++;
	}
	command->nb_chars++;
	return (0);
}

int		get_command(t_shell *shell, t_dy_str *command)
{
	int ret;

	g_glob.cursor = 0;
	ft_bzero(command->str, command->max_size);
	command->current_index = 0;
	command->nb_chars = 0;
	if (shell->should_display)
		ft_printf("%s%s%s%s", BOLD, CYAN, PROMPT, EOC);
	shell->should_display = 1;
	if ((ret = get_keys(shell, command)))
		return (ret);
	return (0);
}
