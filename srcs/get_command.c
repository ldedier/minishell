/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_command.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/14 14:31:34 by ldedier           #+#    #+#             */
/*   Updated: 2019/02/18 19:01:21 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_buffer(char *buffer)
{
	int i;

	i = 0;
	while (i < 3)
	{
		ft_printf("buffer[%d]: %d\n", i, buffer[i]);
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

int		process_delete(t_dy_str *command)
{
	if (g_glob.cursor == 0)
		ring_bell();
	else
	{
		ft_dy_str_suppr_index(command, g_glob.cursor);
		render_command_line(command, -1);
	}
	return (0);
}

int		process_suppr(t_dy_str *command)
{
	if (g_glob.cursor == (int)command->current_size)
		ring_bell();
	else
	{
		ft_dy_str_suppr_index(command, g_glob.cursor + 1);
		render_command_line(command, 0);
	}
	return (0);
}

void	process_left(t_dy_str *command)
{
	if (g_glob.cursor > 0)
		render_command_line(command, -1);
	else
		ring_bell();
}

void	process_right(t_dy_str *command)
{
	if (g_glob.cursor < (int)command->current_size)
		render_command_line(command, 1);
	else
		ring_bell();
}


int		process_escape_sequence(t_dy_str *command,
		char buffer[6])
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

	go_up_to_prompt(g_glob.winsize.ws_col, g_glob.command->current_size); 
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

int		get_keys(t_shell *shell, t_dy_str  *command)
{
	char buffer[6];

	(void)shell;
	while (1)
	{
		read(0, buffer, 6);
		if (buffer[0] >= 32 && buffer[0] <= 126)
		{
			ft_dy_str_add_index(command, buffer[0], g_glob.cursor);
			render_command_line(command, 1);
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
	//	print_buffer(buffer);
		ft_bzero(buffer, 6);
	}
}

int		get_command(t_shell* shell, t_dy_str *command)
{
	int ret;

	g_glob.cursor = 0;
	command->str[0] = 0;
	if (shell->should_display)
		ft_printf("%s%s%s%s", BOLD, CYAN, PROMPT, EOC);
	shell->should_display = 1;
	if ((ret = get_keys(shell, command)))
		return (ret);
	return (0);
}
