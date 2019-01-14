/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/12 22:18:50 by ldedier           #+#    #+#             */
/*   Updated: 2019/01/15 00:12:18 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		ft_process_ms_cd(char *path, int flag)
{
	(void) flag;
	if (chdir(path) != 0)
	{
		if (access(path, F_OK))
			ft_printf("cd: no such file or directory: %s\n", path);
		else if (access(path, X_OK))
			ft_printf("cd: permission denied: %s\n", path);
		else
			ft_printf("cd: an error occured: %s\n", path);
		return (1);
	}
	return (1);
}

int		ms_cd(char **params, t_shell *shell)
{
	char	*home_str;
	int		i;
	int 	flag;
	char	*str;

	flag = 0;
	i = 1;
	while (params[i])
	{
		if (!ft_strcmp(params[i], "-P"))
			flag = 1;
		else if (!ft_strcmp(params[i], "-L"))
			flag = 2;
		else
		{
			if (!ft_strcmp("-", params[i]))
			{
				if ((str = get_env_value((char **)shell->env->tab, "OLDPWD")))
				{
					ft_process_ms_cd(str, flag);
				//	execute_command(pwd, shell);
				}
			}
			else
				return (ft_process_ms_cd(params[i], flag));
		}
		i++;
	}
	if ((home_str = get_env_value((char **)shell->env->tab, "HOME")))
		return (ft_process_ms_cd(home_str, flag));
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
