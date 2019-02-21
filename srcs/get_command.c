/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_command.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/14 14:31:34 by ldedier           #+#    #+#             */
/*   Updated: 2019/02/21 18:55:04 by ldedier          ###   ########.fr       */
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
//	else if (!buffer[1] && !buffer[2])
//	{
		//escape
//	}
	return (0);
}

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
	(void)nb_bytes;
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
//	write(1, &buffer[0], nb_bytes);
	//exit(0);
	return (0);
}

int		get_keys(t_shell *shell, t_dy_str  *command)
{
	unsigned char buffer[READ_BUFF_SIZE];
	int ret;

	(void)shell;
	while (1)
	{
		ret = read(0, buffer, READ_BUFF_SIZE);
		if (is_printable_utf8(buffer, ret))
		{
			if (add_to_command(command, buffer, ret))
				return (1);
			render_command_line(command, 1);
		//	ft_printf("%.8b\n", buffer[0]);
		}
		else if (buffer[0] == 27)
			process_escape_sequence(command, buffer);
		else if (buffer[0] == 9)
			process_tab(shell, command);
		else if (buffer[0] == 12)
			process_clear(command);
		else if (buffer[0] == 10)
		{
			get_down_from_command(command);
			command->current_size = 0;
			return (0);
		}
		else if (buffer[0] == 127)
			process_delete(command);
		else if (buffer[0] == 4)
		{
			if (command->current_size == 0)
			{
				ft_printf("exit\n");
				shell->running = 0;
				return (CTRL_D);
			}
		}
//		printf("WOWO\n");
	//	print_buffer(buffer);
		ft_bzero(buffer, READ_BUFF_SIZE);
	}
}

int		get_command(t_shell* shell, t_dy_str *command)
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
