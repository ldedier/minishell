/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/12 22:18:50 by ldedier           #+#    #+#             */
/*   Updated: 2019/01/20 17:15:51 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		ft_update_old_pwd(char *path, t_cd_opt flag, t_shell *shell)
{
	struct stat	st;
	char		cwd[CWD_LEN];
	char		*pwd_value;

	if (!getcwd(cwd, CWD_LEN))
		return (1);
	if (!(pwd_value = get_env_value((char **)shell->env->tab, "PWD")))
		add_to_env(shell, "OLDPWD", cwd);
	else
		add_to_env(shell, "OLDPWD", pwd_value);
	add_to_env(shell, "PWD", cwd);
	ft_printf("%s\n", cwd);
	return (0);
}

int		ft_process_ms_cd(char *path, t_cd_opt flag, t_shell *shell)
{
	struct stat st;

	lstat(path, &st);
	ft_printf("%s\n", path);
	if (chdir(path) != 0)
	{
		if (access(path, F_OK))
			ft_printf("%s: cd: %s: Not such file or directory\n",
					SH_NAME, path);
		else
		{
			if (stat(path, &st) == -1)
				return (1);
			else
			{
				if (!S_ISDIR(st.st_mode))
					ft_printf("%s: cd: %s: Not a directory\n", SH_NAME, path);
				else if (access(path, X_OK))
					ft_printf("%s: cd: %s: Permission denied\n", SH_NAME, path);
			}
		}
		return (1);
	}
	else
		ft_update_old_pwd(path, flag, shell);
	return (1);
}

int		ms_cd(char **params, t_shell *shell)
{
	char	*home_str;
	int		i;
	int 	flag;
	char	*str;
	char	**pwd_split;

	flag = e_cd_opt_logic;
	i = 1;
	while (params[i])
	{
		if (!ft_strcmp(params[i], "-P"))
			flag = e_cd_opt_physic;
		else if (!ft_strcmp(params[i], "-L"))
			flag = e_cd_opt_logic;
		else
		{
			if (!ft_strcmp("-", params[i]))
			{
				if ((str = get_env_value((char **)shell->env->tab, "OLDPWD")))
				{
					ft_process_ms_cd(str, flag, shell);
					if (!(pwd_split = ft_strsplit("pwd", ' ')))
						return (1);
					execute_command(pwd_split, shell);
					ft_free_split(pwd_split);
					return (1);
				}
				else
					ft_printf("%s: cd: OLDPWD not set\n", SH_NAME);
			}
			else
				return (ft_process_ms_cd(params[i], flag, shell));
		}
		i++;
	}
	if ((home_str = get_env_value((char **)shell->env->tab, "HOME")))
		return (ft_process_ms_cd(home_str, flag, shell));
	else
	{
		ft_printf("%s: cd: HOME not set\n", SH_NAME);
		return (1);
	}
	return (1);
}
