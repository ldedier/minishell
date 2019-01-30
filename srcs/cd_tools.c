/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_tools.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/30 00:01:34 by ldedier           #+#    #+#             */
/*   Updated: 2019/01/30 06:28:30 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_get_previous_directory(char *current_directory)
{
	int i;
	int index;

	index = -1;
	i = 0;
	while (current_directory[i])
	{
		if (current_directory[i] == '/')
			index = i;
		i++;
	}
	if (index == 0 && i == 1)
		return (ft_strdup("/"));
	else
		return (ft_strndup(current_directory, ft_max(1, index)));
}

int		ft_end_by_char(char *str, char c)
{
	int i;

	i = 0;
	while (str[i])
		i++;
	if (--i == -1 || str[i] != c)
		return (0);
	return (1);
}

char	*get_path_from_request(char *old_pwd, char *path)
{
	if (path[0] == '/')
		return (ft_strdup(path));
	else if (!ft_strcmp("..", path))
		return (ft_get_previous_directory(old_pwd));
	else if (!ft_strcmp(".", path))
		return (ft_strdup(old_pwd));
	else
	{
		if (!ft_end_by_char(old_pwd, '/'))
			return (ft_strjoin_3(old_pwd, "/", path));
		else
			return (ft_strjoin(old_pwd, path));
	}
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
	if (!(pwd_value = get_env_value((char **)shell->env->tab, "PWD")))
		add_to_env(shell->env, "OLDPWD", old_pwd);
	else
		add_to_env(shell->env, "OLDPWD", pwd_value);
	add_to_env(shell->env, "PWD", final_pwd);
	free(final_pwd);
	return (0);
}
