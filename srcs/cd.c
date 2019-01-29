/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/22 20:30:39 by ldedier           #+#    #+#             */
/*   Updated: 2019/01/29 23:38:28 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_get_full_path(char *path)
{
	char		cwd[CWD_LEN];
	char		dir[CWD_LEN];
	char		*str;

	if (!getcwd(cwd, CWD_LEN))
		return (NULL);
	if (chdir(path))
		return (NULL);
	if (!getcwd(dir, CWD_LEN))
		return (NULL);
	if (chdir(cwd))
		return (NULL);
	if (!(str = ft_strdup(dir)))
		return (NULL);
	return (str);
}

char	*ft_get_previous_directory(char *current_directory)
{
	int i;
	int index;

	index = - 1;
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

int		ft_update_old_pwd(char *old_pwd, char *path, t_cd_opt flag,
			t_shell *shell)
{
	struct stat	st;
	char		cwd[CWD_LEN];
	char		*pwd_value;
	char		*final_pwd;

	if (!getcwd(cwd, CWD_LEN))
		return (-1);
	if (flag == e_cd_opt_physic)
		final_pwd = ft_strdup(cwd);
	else
	{
		if (path[0] == '/')
			final_pwd = ft_strdup(path);
		else if (!ft_strcmp("..", path))
			final_pwd = ft_get_previous_directory(old_pwd);
		else
		{
			if (!ft_end_by_char(old_pwd, '/'))
				final_pwd = ft_strjoin_3(old_pwd, "/", path);
			else
				final_pwd = ft_strjoin(old_pwd, path);
		}

	}
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

int		ft_process_ms_cd(char *path, t_cd_opt flag, t_shell *shell)
{
	struct stat st;
	char		*pwd;
	char		old_pwd[CWD_LEN];

	if (!getcwd(old_pwd, CWD_LEN))
		return (-1);
	stat(path, &st);
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
	else if (ft_update_old_pwd(old_pwd, path, flag, shell) == -1)
		return (-1);;
	return (1);
}

int		ms_cd(t_shell *shell)
{
	char	*home_str;
	int		i;
	int 	flag;
	char	*str;
	char	**tmp;

	flag = e_cd_opt_logic;
	i = 1;
	while (shell->params[i])
	{
		if (!ft_strcmp(shell->params[i], "-P"))
			flag = e_cd_opt_physic;
		else if (!ft_strcmp(shell->params[i], "-L"))
			flag = e_cd_opt_logic;
		else
		{
			if (!ft_strcmp("-", shell->params[i]))
			{
				if ((str = get_env_value((char **)shell->env->tab, "OLDPWD")))
				{
					ft_process_ms_cd(str, flag, shell);
					tmp = shell->params;
					if (!(shell->params = ft_strsplit("pwd", ' ')))
						return (1);
					ft_free_split(tmp);
					execute_command(shell);
					return (1);
				}
				else
					ft_printf("%s: cd: OLDPWD not set\n", SH_NAME);
			}
			else
				return (ft_process_ms_cd(shell->params[i], flag, shell));
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
