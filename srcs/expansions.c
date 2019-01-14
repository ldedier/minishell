/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/14 23:42:37 by ldedier           #+#    #+#             */
/*   Updated: 2019/01/14 23:43:11 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		describe_entry(char *param, char *entry, char **env_value)
{
	int i;

	if (param[0] != '$')
		return (0);
	i = 1;
	while (param[i] && entry[i - 1]	&&
			param[i] ==  entry[i - 1] &&entry[i - 1] != '=')
		i++;
	if (!param[i] && entry[i - 1] == '=')
	{
		*env_value = ft_strnrest(entry, i);
		return (1);
	}
	return (0);
}

int		get_env_expansion_value(char *param, t_dy_tab *env, char **env_value)
{
	int i;

	i = 0;
	if (!ft_strcmp(param, "~"))
	{
		while (env->tab[i])
		{
			if (describe_entry("$HOME", env->tab[i] , env_value))
				return (1);
			i++;
		}
	}
	else
	{
		while (env->tab[i])
		{
			if (describe_entry(param, env->tab[i], env_value))
				return (1);
		i++;
		}
	}
	return (0);
}

int		preprocess_expansions(char **params, t_shell *shell)
{
	int		i;
	int		len;
	int		ret;
	char	*env_value;

	i = 0;
	while (params[i])
	{
		if (params[i][0] == '$' || !ft_strcmp(params[i], "~"))
		{
			if ((ret = get_env_expansion_value(params[i],
					shell->env, &env_value)))
			{
				if (env_value == NULL)
					return (1);
				free(params[i]);
				params[i] = env_value;
			}
			else
				params[i][0] = 0;
		}
		i++;
	}
	return (0);
}
