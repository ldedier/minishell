/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/12 22:18:50 by ldedier           #+#    #+#             */
/*   Updated: 2019/02/14 13:23:20 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		ms_unsetenv(t_shell *shell)
{
	int i;

	if (ft_splitlen(shell->params) == 1)
		return (1);
	else
	{
		i = 0;
		while (shell->env->tbl[i])
		{
			if (is_key_of_entry(shell->env->tbl[i], shell->params[1]))
			{
				ft_dy_tab_suppr_index(shell->env, i);
				break ;
			}
			i++;
		}
	}
	return (1);
}

int		execute_builtin(t_shell *shell)
{
	if (!ft_strcmp(shell->params[0], "echo"))
		return (ms_echo(shell->params));
	else if (!ft_strcmp(shell->params[0], "cd"))
		return (ms_cd(shell));
	else if (!ft_strcmp(shell->params[0], "setenv"))
		return (ms_setenv(shell));
	else if (!ft_strcmp(shell->params[0], "unsetenv"))
		return (ms_unsetenv(shell));
	else if (!ft_strcmp(shell->params[0], "env"))
		return (ms_env(shell));
	else if (!ft_strcmp(shell->params[0], "exit"))
		return (ms_exit(shell));
	return (0);
}
