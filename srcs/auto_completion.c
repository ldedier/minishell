/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   auto_completion.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/20 14:56:46 by ldedier           #+#    #+#             */
/*   Updated: 2019/02/21 23:57:47 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		get_word_len(char *s, int index)
{
	int i;

	i = index;
	while (s[i])
	{
		if (ft_isseparator(s[i]))
			return (i - index);
		i++;
	}
	return (i - index);
}

int		process_populate_word_by_index(char *s, t_word *word,
			int nb_words, int parseword)
{
	word->word_index = (parseword ? nb_words : 0);
	if (word->word_index)
	{
		word->len = get_word_len(s, word->start_index);
		if (!(word->str = ft_strndup(&s[word->start_index], word->len)))
			return (1);
	}
	else
	{
		if (!(word->str = ft_strdup("")))
			return (1);
		word->len = 0;
	}
	return (0);
}

void	increment_word(int i, int index, t_word *word)
{
	word->start_index = i;
	word->cursor_x = index - word->start_index;
	word->has_previous = 1;
}

int		populate_word_by_index(char *s, int index, t_word *word)
{
	int nb_w;
	int parseword;
	int i;

	i = 0;
	parseword = 0;
	nb_w = 0;
	word->has_previous = 0;
	while (s[i])
	{
		if (i == index)
			return (process_populate_word_by_index(s, word, nb_w, parseword));
		if (!ft_isseparator(s[i]) && !parseword)
		{
			increment_word(i, index, word);
			parseword = 1;
			nb_w++;
		}
		if (ft_isseparator(s[i]) && parseword)
			parseword = 0;
		i++;
	}
	if (i == index)
		return (process_populate_word_by_index(s, word, nb_w, parseword));
	return (0);
}

char	*get_first_word(char *str)
{
	int i;
	int start;

	start = -1;
	i = 0;
	while (str[i])
	{
		if (!ft_isseparator(str[i]) && start == -1)
			start = i;
		else if (ft_isseparator(str[i]) && start != -1)
			return (ft_strndup(&(str[start]), i - start));
		i++;
	}
	if (start == -1)
		return (NULL);
	else
		return (ft_strndup(&(str[start]), i - start));
}

int		str_cmp_len(char *str1, char *str2)
{
	int i;

	i = 0;
	while (str1[i] && str2[i] && str1[i] == str2[i])
		i++;
	return (i);
}

int		process_preprocess_choice_add(t_shell *shell,
			char *entry, int *to_ret, t_dlist ***to_add)
{
	t_dlist *ptr;
	char	*str;
	int		ret;
	int		first;

	ptr = shell->choices;
	first = 1;
	while ((ptr != shell->choices && ptr != NULL) || (first && ptr != NULL))
	{
		str = ptr->content;
		if (!(ret = ft_strcmp(str, entry)))
			return (ft_free_turn(entry, 1));
		if (ret > 0 && *to_add == NULL)
		{
			if (ptr == shell->choices)
				*to_ret = 2;
			*to_add = &(ptr->prev);
		}
		shell->choices_common_len = ft_min(shell->choices_common_len,
				str_cmp_len(entry, str));
		ptr = ptr->next;
		first = 0;
	}
	return (0);
}

int		ft_preprocess_choice_add(t_shell *shell, char *entry, t_dlist ***to_add)
{
	int		to_ret;

	to_ret = 0;
	*to_add = NULL;
	if (shell->choices == NULL)
	{
		*to_add = &shell->choices;
		shell->choices_common_len = ft_strlen(entry);
	}
	if (process_preprocess_choice_add(shell, entry, &to_ret, to_add))
		return (1);
	if (*to_add == NULL && shell->choices != NULL)
		*to_add = &shell->choices->prev;
	return (to_ret);
}

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

void	print_node(t_dlist *dlist)
{
	char *content;

	content = dlist->content;
	ft_printf("%s\n", content);
}

int		process_add_choices_from_dir(t_shell *shell,
			struct dirent *entry)
{
	char			*str;
	t_dlist			**prev_to_add;
	t_dlist			*to_add;
	int				ret;

	if (!(str = ft_strdup(entry->d_name)))
		return (1);
	if ((ret = ft_preprocess_choice_add(shell, str, &prev_to_add)) != 1)
	{
		if (!(to_add = ft_dlstnew_ptr(str, sizeof(str))))
			return (ft_free_turn(str, 1));
		add_node_next_to_node(prev_to_add, to_add);
		if (ret)
			shell->choices = shell->choices->prev;
	}
	return (0);
}

int		add_choices_from_dir(t_shell *shell, t_word *word, char *dirname)
{
	DIR             *dir;
	struct dirent   *entry;

	if (!(dir = opendir(dirname)))
		return (0);
	while ((entry = readdir(dir)) != NULL)
	{
		if (!ft_strncmp(entry->d_name, word->str, word->len))
		{
			if (process_add_choices_from_dir(shell, entry))
			{
				closedir(dir);
				return (1);
			}
		}
	}
	closedir(dir);
	return (0);
}

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

char	*get_completion_str(t_shell *shell)
{
	if (ft_dlstlength(shell->choices) == 1)
	{
		shell->choices_common_len++;
		return (ft_strjoin(shell->choices->content, " "));
	}
	else
		return (ft_strndup(shell->choices->content,
					shell->choices_common_len));
}

int		process_substitute_command(t_dy_str *command, char *str,
			t_word word, t_shell *shell)
{
	if (ft_substitute_dy_str(command, str, word.start_index, word.len))
		return (1);
	command->current_index += (shell->choices_common_len - word.cursor_x);
	command->nb_chars += (ft_strlen_utf8(str) - ft_strlen_utf8(word.str));
	render_command_line(command, shell->choices_common_len - word.cursor_x);
	return (0);
}

int		process_completion(t_shell *shell, t_dy_str *command, t_word word)
{
	char *str;

	if (!(str = get_completion_str(shell)))
		return (1);
	if (!ft_strcmp(word.str, str))
	{
		if (ft_dlstlength(shell->choices) == 1)
		{
			if (process_substitute_command(command, str, word, shell))
				return (ft_free_turn(str, 1));
		}
//		else
//			ft_dlstiter(shell->choices, print_node);
	}
	else if (process_substitute_command(command, str, word, shell))
		return (ft_free_turn(str, 1));
	return (ft_free_turn(str, 0));
}

int		process_tab(t_shell *shell, t_dy_str *command)
{
	t_word	word;
	int		ret;

	ret = 0;
	shell->choices_common_len = -1;
	populate_word_by_index(command->str, command->current_index, &word);
	ft_dlstdel(&shell->choices);
	if (populate_choices_from_word(command, shell, &word))
		return (ft_free_turn(word.str, 1));
	if (shell->choices != NULL)
		ret = process_completion(shell, command, word);
	return (ft_free_turn(word.str, ret));
}
