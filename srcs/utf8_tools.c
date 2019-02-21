/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utf8_tools.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/20 13:37:08 by ldedier           #+#    #+#             */
/*   Updated: 2019/02/20 13:38:22 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
