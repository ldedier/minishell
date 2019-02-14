/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_command.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/14 14:31:34 by ldedier           #+#    #+#             */
/*   Updated: 2019/02/14 16:19:25 by ldedier          ###   ########.fr       */
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
}

void	process_right(t_dy_str *command)
{
	(void)command;
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

int		get_keys(t_shell *shell, t_dy_str  *command)
{
	char buffer[3];

	(void)shell;
	(void)command;
	while (1)
	{
		read(0, buffer, 3);
		if (buffer[0] >= 32 && buffer[0] <= 126)
		{
			ft_dy_str_add_index(command, buffer[0], g_glob.cursor);
			ft_printf("%c", buffer[0]);
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
			print_buffer(buffer);
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
