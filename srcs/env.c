/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/12 22:22:46 by ldedier           #+#    #+#             */
/*   Updated: 2019/01/12 22:38:29 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char		*get_env_value(t_list *entries, char *key)
{
	t_list		*ptr;
	t_env_entry	*entry;

	ptr = entries;
	while (ptr != NULL)
	{
		entry = (t_env_entry *)ptr->content;
		//ft_printf("%s %s\n", entry->key, entry->value);
		if (!ft_strcmp(entry->key, key))
			return (entry->value);
		ptr = ptr->next;
	}
	return (NULL);
}

t_env_entry	*ft_new_entry_from_str(char *str)
{
	t_env_entry	*entry;
	int			index;

	if (!(entry = (t_env_entry *)malloc(sizeof(t_env_entry))))
		return (NULL);
	if ((index = ft_strichr(str, '=')) == -1)
	{
		free(entry);
		return (NULL);
	}
	else
	{
		if (!(entry->key = ft_strndup(str, index)))
		{
			free(entry);
			return (NULL);
		}
		if (!(entry->value = ft_strnrest(str, index + 1)))
		{
			free(entry->key);
			free(entry);
			return (NULL);
		}
	}
	return (entry);
}

char		**convert_entries_to_env(t_list *entries)
{
	t_list		*ptr;
	char		**res;
	t_env_entry	*entry;
	int			i;

	if (!(res = (char **)malloc(sizeof(char *) * (ft_lstlength(entries) + 1))))
		return (NULL);
	ptr = entries;
	i = 0;
	while (ptr != NULL)
	{
		entry = (t_env_entry *)ptr->content;
		if (!(res[i] = ft_strjoin_3(entry->key, "=", entry->value)))
		{
			while (i--)
				free(res[i]);
			free(res);
			return (NULL);
		}
		ptr = ptr->next;
		i++;
	}
	res[ft_lstlength(entries)] = 0;
	return (res);
}

int			ft_update_env(t_shell *shell)
{
	ft_free_split(shell->env_container.env);
	if (!(shell->env_container.env =
			convert_entries_to_env(shell->env_container.entries)))
		return (1);
	return (0);
}
