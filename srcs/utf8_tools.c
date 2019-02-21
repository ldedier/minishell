/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utf8_tools.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/20 13:37:08 by ldedier           #+#    #+#             */
/*   Updated: 2019/02/22 00:01:12 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		ft_strlen_utf8(char *str)
{
	int i;
	int res;

	i = 0;
	res = 0;
	while (str[i])
	{
		if (!(str[i] & 0b10000000))
			i++;
		else if (str[i] & 0b01000000 && !(str[i] & 0b00100000))
			i += 2;
		else if (str[i] & 0b00100000 && !(str[i] & 0b00010000))
			i += 3;
		else if (str[i] & 0b000100000 && !(str[i] & 0b00001000))
			i += 4;
		else
			i++;
		res++;
	}
	return (res);
}

int		get_left_w_char_index(t_dy_str *command)
{
	unsigned char	*buffer;
	int				j;

	j = 1;
	while ((int)command->current_index - j >= 0 && j < 5)
	{
		buffer = (unsigned char *)&(command->str[command->current_index - j]);
		if (is_printable_utf8(buffer, j))
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
	while (i < ft_min(5, (int)(command->current_size -
					command->current_index + 1)))
	{
		if (is_printable_utf8(buffer, i))
			return (i + command->current_index);
		i++;
	}
	return (command->current_index);
}
