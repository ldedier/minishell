/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/12 22:27:37 by ldedier           #+#    #+#             */
/*   Updated: 2019/01/20 04:48:30 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		ft_init_shell(t_shell *shell, char **env)
{
	int			i;
	char		*str;

	shell->running = 1;
	if (!(shell->env = ft_dy_tab_new(64)))
		return (1);
	i = 0;
	while (env[i])
	{
		if (!(str = ft_strdup(env[i])))
			return (1);
		if (ft_dy_tab_add_ptr(shell->env, str))
		{
			ft_dy_tab_del(shell->env);
			return (1);
		}
		i++;
	}
	return (0);
}
