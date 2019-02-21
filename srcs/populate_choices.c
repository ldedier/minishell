/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   populate_choices.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/22 00:26:18 by ldedier           #+#    #+#             */
/*   Updated: 2019/02/22 00:26:35 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		add_choices_path(t_shell *shell, t_word *word, char *path_str)
{
	char    **path_split;
	int     i;

	if (!(path_split = ft_strsplit(path_str, ':')))
		return (1);
	i = 0;
	while (path_split[i])
	{
		if (add_choices_from_dir(shell, word, path_split[i]))
		{
			ft_free_split(path_split);
			return (1);
		}
		i++;
	}
	ft_free_split(path_split);
	return (0);
}

int		populate_choices_from_binaries(t_shell *shell, t_word *word)
{
	char *path_str;

	(void)shell;
	(void)word;
	if ((path_str = get_env_value((char **)shell->env->tbl, "PATH")))
	{
		if (add_choices_path(shell, word, path_str))
			return (1);
		return (0);
	}
	else
		return (0);
}

int		populate_choices_from_folder(t_shell *shell, char *binary,
		t_word *word)
{

	(void)shell;
	(void)binary;
	(void)word;
	return (0);
}

int		populate_choices_from_word(t_dy_str *command,
		t_shell *shell, t_word *word)
{
	char *binary;

	if (word->word_index == 1 || (word->word_index == 0 && !word->has_previous))
	{
		if (populate_choices_from_binaries(shell, word))
			return (1);
	}
	else
	{
		if (!(binary = get_first_word(command->str)))
			return (1);
		if (populate_choices_from_folder(shell, binary, word))
			return (1);
	}
	return (0);
}
