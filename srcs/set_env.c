/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/14 22:28:34 by ldedier           #+#    #+#             */
/*   Updated: 2019/01/14 22:28:50 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		get_key_len(char *entry)
{
	int i;

	i = 0;
	while (entry[i])
	{
	   if (entry[i] == '=')
		   return (i);
	   i++;
	}
	return (-1);
}

int		add_to_env(t_shell *shell, char *key, char *value)
{
	char 	*entry;
	int		i;
	i = 0;
	while (shell->env->tab[i])
	{
		if (is_key_of_entry(shell->env->tab[i], key))
		{
			ft_dy_tab_suppr_index(shell->env, i);
			break ;
		}
		i++;
	}
	if (!(entry = ft_strjoin_3(key, "=", value)))
		return (1);
	if (ft_dy_tab_add_ptr(shell->env, entry))
		return (1);
	return (0);
}

int		ft_process_set_env_equal(char *entry, t_shell *shell)
{
	char	*value;
	char	*key;
	int		key_len;

	if ((key_len = get_key_len(entry)) == -1)
		return (1);
	if (!(key = ft_strndup(entry, key_len)))
		return (-1);
	if (!(value = ft_strnrest(entry, key_len + 1)))
	{
		free(key);
		return (-1);
	}
	if (add_to_env(shell, key, value))
		return (-1);
	return (0);
}

int		ms_setenv(char **params, t_shell *shell)
{
	if (ft_splitlen(params) == 1)
		return (1);
	else if (ft_splitlen(params) == 2)
		return (ft_process_set_env_equal(params[1], shell));
	else if (ft_splitlen(params) == 3)
	{
		if (add_to_env(shell, params[1], params[2]))
			return (-1);
	}
	return (1);
}
