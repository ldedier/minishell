/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/12 22:18:50 by ldedier           #+#    #+#             */
/*   Updated: 2019/01/20 06:07:09 by ldedier          ###   ########.fr       */
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
		return (1);
	return (1);
}

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
