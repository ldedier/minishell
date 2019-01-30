/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/12 22:27:37 by ldedier           #+#    #+#             */
/*   Updated: 2019/01/29 23:48:59 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		ft_update_shell_lvl(t_shell *shell)
{
	char	*str;
	int		new_lvl;
	char	*new_lvl_str;

	if (!(str = get_env_value((char **)shell->env->tab, "SHLVL")))
		new_lvl = 1;
	else
		new_lvl = ft_atoi(str) + 1;
	if (!(new_lvl_str = ft_itoa(new_lvl)))
	{
		ft_dy_tab_del(shell->env);
		return (1);
	}
	if (add_to_env(shell->env, "SHLVL", new_lvl_str))
	{
		free(new_lvl_str);
		ft_dy_tab_del(shell->env);
		return (1);
	}
	free(new_lvl_str);
	return (0);
}

int		ft_init_shell(t_shell *shell, char **env)
{
	int			i;
	char		*str;

	shell->running = 1;
	shell->should_display = 1;
	if (!(shell->env = ft_dy_tab_new(64)))
		return (1);
	i = 0;
	while (env[i])
	{
		if (!(str = ft_strdup(env[i])))
		{
			ft_dy_tab_del(shell->env);
			return (1);
		}
		if (ft_dy_tab_add_ptr(shell->env, str))
		{
			ft_dy_tab_del(shell->env);
			return (1);
		}
		i++;
	}
	if (ft_update_shell_lvl(shell))
		return (1);
	return (0);
}
