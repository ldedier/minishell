/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   home.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/13 23:52:50 by ldedier           #+#    #+#             */
/*   Updated: 2019/02/25 23:11:39 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_home_dup(t_shell *shell)
{
	char			*str;
	struct passwd	*pwd;
	uid_t			uid;

	if (!(str = get_env_value((char **)shell->env->tbl, "HOME")))
	{
		uid = geteuid();
		if (!(pwd = getpwuid(uid)))
			return (NULL);
		else
			return (ft_strdup(pwd->pw_dir));
	}
	else
		return (ft_strdup(str));
}

int		process_subst_home(t_shell *shell, char **str)
{
	char *subst;

	if (!(subst = get_home_dup(shell)))
		return (1);
	else if (ft_substitute_str(str, subst, 0, 1))
	{
		free(subst);
		return (1);
	}
	free(subst);
	return (0);
}
