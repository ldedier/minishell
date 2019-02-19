/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_command.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/14 14:31:34 by ldedier           #+#    #+#             */
/*   Updated: 2019/02/19 21:02:39 by ldedier          ###   ########.fr       */
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

int		process_tab(t_dy_str *command)
{
	(void)command;
	return (0);
}

void	ring_bell()
{
	char *str;

	str = tgetstr("bl", NULL);
	tputs(str, 1, putchar_int);
}


void	ft_dy_str_del_char(t_dy_str *command)
{
	unsigned char	*buffer;
	int				i;
	int				j;

	j = 1;
	i = command->current_index - 1;
	while ((int)command->current_index - j >= 0 && j < 5)
	{
		buffer = (unsigned char *)&(command->str[command->current_index - j]);
		if (to_add_to_command(buffer, j))
		{
			i = command->current_index - j;
			while (j + i <= (int)command->current_size)
			{
				command->str[i] = command->str[j + i];
				i++;
			}
			command->current_index -= j;
			command->current_size -= j;
			return ;
		}
		j++;
	}
}

void	ft_dy_str_suppr_char(t_dy_str *command)
{
	unsigned char	*buffer;
	int				i;
	int				j;

	buffer = (unsigned char *)&(command->str[command->current_index]);
	i = 1;
	while (i < ft_min(5, (int)(command->current_size - command->current_index + 1)))
	{
		if (to_add_to_command(buffer, i))
		{
			j = command->current_index;
			while (j + i <= (int)command->current_size)
			{
				command->str[j] = command->str[j + i];
				j++;
			}
			command->current_size -= i;
			return ;
		}
		i++;
	}
}

int		process_delete(t_dy_str *command)
{
	if (command->current_index == 0)
		ring_bell();
	else
	{
		ft_dy_str_del_char(command);
		command->nb_chars--;
		render_command_line(command, -1);
	}
	return (0);
}

int		process_suppr(t_dy_str *command)
{
	if (command->current_index == command->current_size)
		ring_bell();
	else
	{
		ft_dy_str_suppr_char(command);
		command->nb_chars--;
		render_command_line(command, 0);
	}
	return (0);
}

int		get_left_w_char_index(t_dy_str *command)
{
	unsigned char	*buffer;
	int				j;

	j = 1;
	while ((int)command->current_index - j >= 0 && j < 5)
	{
		buffer = (unsigned char *)&(command->str[command->current_index - j]);
		if (to_add_to_command(buffer, j))
			return (command->current_index - j);
		j++;
	}
	return (command->current_index);
}

int		get_right_w_char_index(t_dy_str *command)
{
	unsigned char	*buffer;
	int				i;

	buffer = (unsigned char *)&(command->str[command->current_index]);
	i = 1;
	while (i < ft_min(5, (int)(command->current_size - command->current_index + 1)))
	{
		if (to_add_to_command(buffer, i))
			return (i + command->current_index);
		i++;
	}
	return (command->current_index);
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

int		get_true_cursor_pos(int cursor)
{
	return (cursor + ft_strlen(PROMPT));
}

void	go_up_to_prompt(int width, int cursor)
{
	char		*str;
	int			val;
	int			i;
	
	str = tgetstr("up", NULL);
	i = 0;
	ft_printf(" ");
	val = ((get_true_cursor_pos(cursor)) / width);
	while (i < val)
	{
		tputs(str, 1, putchar_int);
		i++;
	}
	str = tgetstr("cr", NULL);
	tputs(str, 1, putchar_int);
}

void	replace_cursor_after_render()
{
	char	*str;
	int		x;
	int		y;

	go_up_to_prompt(g_glob.winsize.ws_col, g_glob.command->nb_chars); 
	y = (g_glob.cursor + ft_strlen(PROMPT)) / g_glob.winsize.ws_col;
	x = (g_glob.cursor + ft_strlen(PROMPT)) % g_glob.winsize.ws_col;
	str = tgetstr("do", NULL);
	while (y--)
		tputs(str, 1, putchar_int);
	str = tgetstr("nd", NULL);
	while (x--)
		tputs(str, 1, putchar_int);
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

int		process_clear(t_dy_str *command)
{
	char *str;

	str = tgetstr("cl", NULL);
	tputs(str, 1, putchar_int);
	render_command_line(command, 0);
	return (0);
}

int		is_utf8_len_header(unsigned char first_byte, int length)
{
	int i;

	i = 0;
	while (i < length)
	{
		if (!(first_byte & ((0b10000000) >> i)))
			return (0);
		i++;
	}
	return (!(first_byte & ((0b10000000) >> i)));
}

int		is_utf8_next_byte(unsigned char next_byte)
{
	return (next_byte & 0b10000000 && !(next_byte & 0b01000000));
}

int		is_utf8_len_3(unsigned char *buffer)
{
	if (!is_utf8_len_header(buffer[0], 3) ||
			!is_utf8_next_byte(buffer[1]) || !is_utf8_next_byte(buffer[2]))
		return (0);
	else if (buffer[0] == 224)
		return (buffer[1] & 0b00100000);
	else if (buffer[0] == 225 || buffer[0] == 226 || buffer[0] == 227)
		return (1);
	else if (!(buffer[0] & 0b00001000) && buffer[0] & 0b00000100)
		return (1);
	else if (buffer[0] & 0b00001000 && !(buffer[0] & 0b00000100))
		return (1);
	else if (buffer[0] == 236)
		return (1);
	else if (buffer[0] == 237)
		return (!(buffer[1] & 0b00100000));
	else if (buffer[0] == 238 || buffer[0] == 239)
		return (1);
	else
		return (0);
}

int		is_utf8_len_4(unsigned char *buffer)
{
	if (!is_utf8_len_header(buffer[0], 4) ||
			!is_utf8_next_byte(buffer[1]) || !is_utf8_next_byte(buffer[2]) || 
				!is_utf8_next_byte(buffer[3]))
		return (0);
	else if (buffer[0] == 240)
	{
		return ((buffer[1] & 0b00100000) ||
			(!(buffer[1] & 0b00100000) && buffer[1] & 0b00010000));
	}
	else if (buffer[0] == 241)
		return (1);
	else if (!(buffer[0] & 0b00000100) && buffer[0] & 0b00000010)
		return (1);
	else if (buffer[0] == 244)
		return (!(buffer[1] & 0b00100000) && !(buffer[1] & 0b00010000));
	return (0);
}

int		to_add_to_command(unsigned char *buffer, int nb_bytes)
{
	if (nb_bytes == 1)
		return (buffer[0] >= 32 && buffer[0] <= 126 && !(buffer[0] & 0b10000000));
	else if (nb_bytes == 2)
		return (is_utf8_len_header(buffer[0], 2)
			&& is_utf8_next_byte(buffer[1]));
	else if (nb_bytes == 3)
		return (is_utf8_len_3(buffer));
	else if (nb_bytes == 4)
		return (is_utf8_len_4(buffer));
	else 
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
		if (to_add_to_command(buffer, ret))
		{
			if (add_to_command(command, buffer, ret))
				return (1);
			render_command_line(command, 1);
		//	ft_printf("%.8b\n", buffer[0]);
		}
		else if (buffer[0] == 27)
			process_escape_sequence(command, buffer);
		else if (buffer[0] == 9)
			process_tab(command);
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
	command->str[0] = 0;
	command->current_index = 0;
	command->nb_chars = 0;
	if (shell->should_display)
		ft_printf("%s%s%s%s", BOLD, CYAN, PROMPT, EOC);
	shell->should_display = 1;
	if ((ret = get_keys(shell, command)))
		return (ret);
	return (0);
}
