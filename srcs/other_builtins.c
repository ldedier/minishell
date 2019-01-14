/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   other_builtins.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/14 22:31:56 by ldedier           #+#    #+#             */
/*   Updated: 2019/01/14 22:31:59 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		ms_echo(char **params)
{
	int i;

	i = 1;
	while (params[i])
	{
		if (i != 1)
			ft_printf(" %s", params[i]);
		else
			ft_printf("%s", params[i]);
		i++;
	}
	ft_printf("\n");
	return (1);
}

int		ms_env(char **params, t_shell *shell)
{
	int i;

	i = 0;
	while (shell->env->tab[i])
	{
		ft_printf("%s\n", shell->env->tab[i]);
		i++;
	}
	return (1);
}

int		ms_exit(char **params, t_shell *shell)
{
	shell->running = 0;
	return (1);
}
