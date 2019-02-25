/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/24 22:43:23 by ldedier           #+#    #+#             */
/*   Updated: 2019/02/24 22:45:34 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_buffer(unsigned char buffer[READ_BUFF_SIZE])
{
	int i;

	i = 0;
	while (i < READ_BUFF_SIZE)
	{
		ft_printf("\nbuffer[%d]: %d (%.8b)", i, buffer[i], buffer[i]);
		i++;
	}
}

int		process_escape_sequence(t_dy_str *command,
		unsigned char buffer[READ_BUFF_SIZE])
{
	if (buffer[1] == 91 && buffer[2] == 67)
		process_right(command);
	else if (buffer[1] == 91 && buffer[2] == 68)
		process_left(command);
	else if (buffer[1] == 91 && buffer[2] == 51)
		process_suppr(command);
	return (0);
}

void	process_keys(t_shell *shell, t_dy_str *command,
				unsigned char *buffer)
{
	if (buffer[0] == 27)
		process_escape_sequence(command, buffer);
	else if (buffer[0] == 9)
		process_tab(shell, command);
	else if (buffer[0] == 12)
		process_clear(command);
	else if (buffer[0] == 127)
		process_delete(command);
	else if (buffer[0] == 3)
	{
		get_down_from_command(g_glob.command);
		g_glob.cursor = 0;
		g_glob.command->current_size = 0;
		g_glob.command->current_index = 0;
		ft_bzero(g_glob.command->str, g_glob.command->max_size);
		g_glob.command->nb_chars = 0;
		render_command_line(g_glob.command, 0);
	}
}

int		process_keys_ret(t_shell *shell, t_dy_str *command,
			unsigned char *buffer)
{
	if (buffer[0] == 10)
	{
		get_down_from_command(command);
		command->current_size = 0;
		return (0);
	}
	else if (buffer[0] == 4)
	{
		if (command->current_size == 0)
		{
			ft_printf("exit\n");
			shell->running = 0;
			return (CTRL_D);
		}
	}
	return (1);
}

int		get_keys(t_shell *shell, t_dy_str *command)
{
	unsigned char	buffer[READ_BUFF_SIZE];
	int				ret;

	ft_bzero(buffer, READ_BUFF_SIZE);
	while (1)
	{
		ret = read(0, buffer, READ_BUFF_SIZE);
		if (is_printable_utf8(buffer, ret))
		{
			if (add_to_command(command, buffer, ret))
				return (1);
			render_command_line(command, 1);
		}
		process_keys(shell, command, buffer);
		ret = process_keys_ret(shell, command, buffer);
		if (ret == CTRL_D || ret == 0)
			return (ret);
		ft_bzero(buffer, READ_BUFF_SIZE);
	}
}
