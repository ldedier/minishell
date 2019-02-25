/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   edit_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/20 13:39:39 by ldedier           #+#    #+#             */
/*   Updated: 2019/02/24 21:40:46 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ring_bell(void)
{
	char *str;

	str = tgetstr("bl", NULL);
	tputs(str, 1, putchar_int);
}

void	process_delete(t_dy_str *command)
{
	int diff;
	int left_w_char_index;

	if (command->current_index == 0)
		ring_bell();
	else
	{
		left_w_char_index = get_left_w_char_index(command);
		diff = command->current_index - left_w_char_index;
		while (left_w_char_index + diff <= (int)command->current_size)
		{
			command->str[left_w_char_index] =
				command->str[left_w_char_index + diff];
			left_w_char_index++;
		}
		command->current_index -= diff;
		command->current_size -= diff;
		command->nb_chars--;
		render_command_line(command, -1);
	}
}

void	process_suppr(t_dy_str *command)
{
	int diff;
	int i;

	if (command->current_index == command->current_size)
		ring_bell();
	else
	{
		diff = get_right_w_char_index(command) - command->current_index;
		i = command->current_index;
		while (i + diff <= (int)command->current_size)
		{
			command->str[i] = command->str[i + diff];
			i++;
		}
		command->current_size -= diff;
		command->nb_chars--;
		render_command_line(command, 0);
	}
}

void	process_left(t_dy_str *command)
{
	if (command->current_index > 0)
	{
		command->current_index = get_left_w_char_index(command);
		render_command_line(command, -1);
	}
	else
		ring_bell();
}

void	process_right(t_dy_str *command)
{
	if (command->current_index < command->current_size)
	{
		command->current_index = get_right_w_char_index(command);
		render_command_line(command, 1);
	}
	else
		ring_bell();
}
