/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_choices_from_dir.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/22 00:06:33 by ldedier           #+#    #+#             */
/*   Updated: 2019/02/22 00:10:06 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_node_next_to_node(t_dlist **node, t_dlist *to_add)
{
	t_dlist *tmp;

	if (*node)
	{
		tmp = (*node)->next;
		(*node)->next = to_add;
		(to_add)->prev = (*node);
		(to_add)->next = tmp;
		(to_add)->next->prev = to_add;
	}
	else
		*node = to_add;
}

t_file		*new_file(char *str)
{
	t_file *res;

	if (!(res = (t_file *)malloc(sizeof(t_file))))
		return (NULL);
	res->name = str;
	if (lstat(res->name, &res->st) == -1)
		res->unstatable = 1;
	else
		res->unstatable = 0;
	return (res);
}

int		process_add_choices_from_dir(t_shell *shell,
			struct dirent *entry, char *prefix)
{
	char			*str;
	t_dlist			**prev_to_add;
	t_dlist			*to_add;
	int				ret;
	t_file			*file;

	if (!prefix && !(str = ft_strdup(entry->d_name)))
		return (1);
	else if (prefix && !(str = ft_strjoin(prefix, entry->d_name)))
		return (1);
	if ((ret = ft_preprocess_choice_add(shell, str, &prev_to_add)) != 1)
	{
		if (!(file = new_file(str)))
			return (1);
		if (!(to_add = ft_dlstnew_ptr(file, sizeof(t_file))))
			return (ft_free_turn(str, 1));
		add_node_next_to_node(prev_to_add, to_add);
		if (ret)
			shell->choices = shell->choices->prev;
	}
	return (0);
}

int		add_choices_from_dir(t_shell *shell, t_word *word, char *dirname,
			char *prefix)
{
	DIR             *dir;
	struct dirent   *entry;
	int				len;

	len = ft_strlen(word->to_compare);
	if (!ft_strcmp(dirname, "") && !(dir = opendir(".")))
		return (0);
	else if (ft_strcmp(dirname, "") && !(dir = opendir(dirname)))
		return (0);
	while ((entry = readdir(dir)) != NULL)
	{
		if (!ft_strncmp(entry->d_name, word->to_compare, len))
		{
			if (process_add_choices_from_dir(shell, entry, prefix))
			{
				closedir(dir);
				return (1);
			}
		}
	}
	closedir(dir);
	return (0);
}

