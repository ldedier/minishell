/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_tools.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/30 00:01:34 by ldedier           #+#    #+#             */
/*   Updated: 2019/02/14 13:26:01 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		end_with_char(char *str, char c)
{
	int i;

	i = 0;
	while (str[i])
		i++;
	if (--i >= 0 && str[i] == c)
		return (1);
	return (0);
}

char	*get_path_from_request(char *old_pwd, char *path)
{
	if (path[0] == '/')
		return (get_sanitized_path_from_old("/", &(path[1])));
	else
		return (get_sanitized_path_from_old(old_pwd, path));
}

int		ft_update_old_pwd(char *old_pwd, char *path, t_cd_opt flag,
			t_shell *shell)
{
	char		cwd[CWD_LEN];
	char		*pwd_value;
	char		*final_pwd;

	if (!getcwd(cwd, CWD_LEN))
		return (-1);
	if (flag == e_cd_opt_physic)
		final_pwd = ft_strdup(cwd);
	else
		final_pwd = get_path_from_request(old_pwd, path);
	if (!final_pwd)
		return (-1);
	if (!(pwd_value = get_env_value((char **)shell->env->tbl, "PWD")))
		add_to_env(shell->env, "OLDPWD", old_pwd);
	else
		add_to_env(shell->env, "OLDPWD", pwd_value);
	add_to_env(shell->env, "PWD", final_pwd);
	free(final_pwd);
	return (0);
}
