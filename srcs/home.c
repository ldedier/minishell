/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   home.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/13 23:52:50 by ldedier           #+#    #+#             */
/*   Updated: 2019/02/13 23:54:13 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_home_dup(t_shell *shell)
{
	char *str;

	if (!(str = get_env_value((char **)shell->env->tab, "HOME")))
		return (ft_strdup(LOCAL_HOME));
	else
		return (ft_strdup(str));
}

int		process_subst_home(t_shell *shell, int i)
{
	char *subst;

	if (!(subst = get_home_dup(shell)))
		return (1);
	else if (ft_substitute_str(&shell->params[i], subst, 0, 1))
	{
		free(subst);
		return (1);
	}
	free(subst);
	return (0);
}
