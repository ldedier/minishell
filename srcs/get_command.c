/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_command.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/14 14:31:34 by ldedier           #+#    #+#             */
/*   Updated: 2019/02/18 00:03:53 by ldedier          ###   ########.fr       */
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

int		process_delete(t_dy_str *command)
{
	(void)command;
	return (0);
}

void	process_left(t_dy_str *command)
{
	(void)command;
	g_glob.cursor = ft_max(0, g_glob.cursor - 1);
	render_command_line(command, 0);
}

void	process_right(t_dy_str *command)
{
	g_glob.cursor = ft_min(command->current_size, g_glob.cursor + 1);
	render_command_line(command, 0);
}

int		process_escape_sequence(t_dy_str *command,
		char buffer[3])
{
	if (buffer[1] == 91 && buffer[2] == 67) 
		process_right(command);
	else if (buffer[1] == 91 && buffer[2] == 68)
		process_left(command);
	else if (!buffer[1] && !buffer[2])
	{
		//escape
	}
	return (0);
}

int		get_true_cursor_pos(int cursor)
{
	return (cursor + ft_strlen(PROMPT));
}

void	go_up_to_prompt(int width, int cursor, int carry)
{
	char *str;
	int val;
	int i;

	static int lol = 0;

	if (carry && lol++ >= 1)
	{
		carry = 0;
//		exit(1);
	}
	str = tgetstr("up", NULL);
	i = 0;
	val = ((get_true_cursor_pos(cursor) - 1) / width);
	while (i < val - carry)
	{
		tputs(str, 1, putchar_int);
		i++;
	}
	str = tgetstr("cr", NULL);
	tputs(str, 1, putchar_int);
}
/*
int		ft_patoi_coord(char **str)
{
	int		result;

	result = 0;
	while (!ft_isdigit(**str))
		(*str)++;
	while (ft_isdigit(**str))
	{
		result = result * 10 + (**str - '0');
		(*str)++;
	}
	return (result);
}

int		get_cursor_pos(int *x, int *y)
{
	char	*answer;
	char	*save;
	size_t	answerlen;
	int		ttyfd;

	if (!(answer = (char *)malloc(sizeof(char) * 16)))
		return (1);
	if ((ttyfd = open("/dev/tty", O_RDWR)) < 0)
	{
		printf ("Cannot open /devv/tty\n");
		return (ft_free_turn(answer, 1));
	}
	write(ttyfd, "\x1B[6n\n", 5);
	answerlen = 0;
	while (answerlen < 16 - 1 && read (ttyfd, answer + answerlen, 1) == 1)
	{
		if (answer[answerlen ++] == 'R')
			break;
	}
	answer[answerlen]= '\0';
	save = answer;
	*x = ft_patoi_coord(&answer);
	*y = ft_patoi_coord(&answer);
	free(save);
	return (0);
}
*/

void	replace_cursor_after_render()
{
	char	*str;
	int		x;
	int		y;

	go_up_to_prompt(g_glob.winsize.ws_col, g_glob.cursor, 0);
	y = (g_glob.cursor + ft_strlen(PROMPT)) / g_glob.winsize.ws_col;
	x = (g_glob.cursor + ft_strlen(PROMPT)) % g_glob.winsize.ws_col;
	str = tgetstr("do", NULL);
	while (y--)
		tputs(str, 1, putchar_int);
	str = tgetstr("nd", NULL);
	while (x--)
		tputs(str, 1, putchar_int);
//	tputs(str, 1, putchar_int);

}

int		render_command_line(t_dy_str *command, int carry)
{
	char	*str;
//	t_xy	xy;

//	get_cursor_pos(&xy.x, &xy.y)
	go_up_to_prompt(g_glob.winsize.ws_col, g_glob.cursor, carry);
	str = tgetstr("cd", NULL);
	tputs(str, 1, putchar_int);
	ft_printf("%s%s%s%s", BOLD, CYAN, PROMPT, EOC);
	ft_printf("%s", command->str);
	replace_cursor_after_render();
	return (0);
}

int		get_keys(t_shell *shell, t_dy_str  *command)
{
	char buffer[3];

	(void)shell;
	(void)command;
	while (1)
	{
		read(0, buffer, 6);
		if (buffer[0] >= 32 && buffer[0] <= 126)
		{
			ft_dy_str_add_index(command, buffer[0], g_glob.cursor++);
			if ((int)((get_true_cursor_pos(g_glob.cursor) - 0) / g_glob.winsize.ws_col) >
					(int)((get_true_cursor_pos(g_glob.cursor) - 1) / g_glob.winsize.ws_col))
				render_command_line(command, 1);
			else
				render_command_line(command, 0);
		}
		else if (buffer[0] == 27)
			process_escape_sequence(command, buffer);
		else if (buffer[0] == 10)
			process_tab(command);
		else if (buffer[0] == 127)
			process_delete(command);
		else if (buffer[0] == 4)
		{
			shell->running = 0;
			return (CTRL_D);
		}
		//print_buffer(buffer);
	}
}

int		get_command(t_shell* shell, t_dy_str **command)
{
	if (!(*command = ft_dy_str_new(128)))
		return (1);
	ft_printf("%s%s%s%s", BOLD, CYAN, PROMPT, EOC); 
	if (get_keys(shell, *command))
		return (1);
	return (0);
}
