/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/14 22:28:34 by ldedier           #+#    #+#             */
/*   Updated: 2019/01/23 17:30:24 by ldedier          ###   ########.fr       */
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

int		add_to_env(t_dy_tab *env, char *key, char *value)
{
	char 	*entry;
	size_t	i;

	i = 0;
	while (i < env->current_size)
	{
		if (is_key_of_entry(env->tab[i], key))
		{
			ft_dy_tab_suppr_index(env, i);
			break ;
		}
		i++;
	}
	if (!(entry = ft_strjoin_3(key, "=", value)))
		return (1);
	if (ft_dy_tab_add_ptr(env, entry))
		return (1);
	return (0);
}

int		ft_process_set_env_equal(char *entry, t_dy_tab *env)
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
	if (add_to_env(env, key, value))
	{
		free(key);
		free(value);
		return (-1);
	}
	free(key);
	free(value);
	return (1);
}

int		ms_setenv(t_shell *shell)
{
	if (ft_splitlen(shell->params) == 1)
		return (1);
	else if (ft_splitlen(shell->params) == 2)
		return (ft_process_set_env_equal(shell->params[1], shell->env));
	else if (ft_splitlen(shell->params) == 3)
	{
		if (add_to_env(shell->env, shell->params[1], shell->params[2]))
			return (-1);
	}
	else
		ft_dprintf(2, "%s: bad assignment\n", SH_NAME);
	return (1);
}
