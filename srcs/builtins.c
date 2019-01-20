/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/12 22:18:50 by ldedier           #+#    #+#             */
/*   Updated: 2019/01/20 17:14:21 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		ms_unsetenv(char **params, t_shell *shell)
{
	int i;

	if (ft_splitlen(params) == 1)
		return (1);
	else
	{
		i = 0;
		while (shell->env->tab[i])
		{
			if (is_key_of_entry(shell->env->tab[i], params[1]))
			{
				ft_dy_tab_suppr_index(shell->env, i);
				break;
			}
			i++;
		}
	}
	return (1);
}

int		execute_builtin(char **params, t_shell *shell)
{
	if (!ft_strcmp(params[0], "echo"))
		return (ms_echo(params));
	else if (!ft_strcmp(params[0], "cd"))
		return (ms_cd(params, shell));
	else if (!ft_strcmp(params[0], "setenv"))
		return (ms_setenv(params, shell));
	else if (!ft_strcmp(params[0], "unsetenv"))
		return (ms_unsetenv(params, shell));
	else if (!ft_strcmp(params[0], "env"))
		return (ms_env(params, shell));
	else if (!ft_strcmp(params[0], "exit"))
		return (ms_exit(params, shell));
	return (0);
}
