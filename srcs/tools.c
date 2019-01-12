/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/12 22:29:00 by ldedier           #+#    #+#             */
/*   Updated: 2019/01/12 22:32:16 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		get_file_in_dir(char *filename, char *dirname)
{
	DIR				*dir;
	struct dirent	*entry;

	if (!(dir = opendir(dirname)))
		return (0);
	while ((entry = readdir(dir)) != NULL)
	{
		if (!ft_strcmp(entry->d_name, filename))
			return (1);
	}
	closedir(dir);
	return (0);
}
