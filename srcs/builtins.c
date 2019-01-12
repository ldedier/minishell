/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/12 22:18:50 by ldedier           #+#    #+#             */
/*   Updated: 2019/01/12 22:31:17 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		ft_process_ms_cd(char *path)
{
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

	if (ft_splitlen(params) == 1)
	{
		if ((home_str = get_env_value(shell->env_container.entries, "HOME")))
			return (ft_process_ms_cd(home_str));
		else
			return (1);
	}
	else
		return (ft_process_ms_cd(params[1]));
	return (1);
}

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

int		ms_setenv(char **params, t_shell *shell)
{
	return (1);
}

int		ms_unsetenv(char **params, t_shell *shell)
{
	return (1);
}

int		ms_env(char **params, t_shell *shell)
{
	int i;

	i = 0;
	while (shell->env_container.env[i])
	{
		ft_printf("%s\n", shell->env_container.env[i]);
		i++;
	}
	return (1);
}

int		ms_exit(char **params, t_shell *shell)
{
	shell->running = 0;
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
