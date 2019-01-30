/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/22 20:30:39 by ldedier           #+#    #+#             */
/*   Updated: 2019/01/30 06:28:27 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		ft_print_cd_errors(char *path)
{
	struct stat st;

	if (access(path, F_OK))
		ft_printf("%s: cd: %s: Not such file or directory\n",
				SH_NAME, path);
	else
	{
		if (stat(path, &st) == -1)
			return (-1);
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

int		ft_process_ms_cd(char *path, t_cd_opt flag, t_shell *shell)
{
	char		old_pwd[CWD_LEN];

	if (!getcwd(old_pwd, CWD_LEN))
		return (-1);
	if (chdir(path) != 0)
		return (ft_print_cd_errors(path));
	else if (ft_update_old_pwd(old_pwd, path, flag, shell) == -1)
		return (-1);
	return (1);
}

int		ft_process_ms_cd_args(t_shell *shell, int flag, int i)
{
	char	**tmp;
	char	*str;

	if (!ft_strcmp("-", shell->params[i]))
	{
		if ((str = get_env_value((char **)shell->env->tab, "OLDPWD")))
		{
			ft_process_ms_cd(str, flag, shell);
			tmp = shell->params;
			if (!(shell->params = ft_strsplit("pwd", ' ')))
				return (-1);
			ft_free_split(tmp);
			return (execute_command(shell) == -1 ? -1 : 1);
		}
		else
		{
			ft_printf("%s: cd: OLDPWD not set\n", SH_NAME);
			return (1);
		}
	}
	else
		return (ft_process_ms_cd(shell->params[i], flag, shell));
}

int		ms_cd(t_shell *shell)
{
	char	*home_str;
	int		i;
	int		flag;
	int		ret;

	flag = e_cd_opt_logic;
	i = 1;
	while (shell->params[i])
	{
		if (!ft_strcmp(shell->params[i], "-P"))
			flag = e_cd_opt_physic;
		else if (!ft_strcmp(shell->params[i], "-L"))
			flag = e_cd_opt_logic;
		else if ((ret = ft_process_ms_cd_args(shell, flag, i)))
			return (ret);
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
