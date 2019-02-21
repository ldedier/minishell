/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   auto_completion.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/20 14:56:46 by ldedier           #+#    #+#             */
/*   Updated: 2019/02/22 00:28:48 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	print_node(t_dlist *dlist)
{
	char *content;

	content = dlist->content;
	ft_printf("%s\n", content);
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