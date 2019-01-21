/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/12 22:25:21 by ldedier           #+#    #+#             */
/*   Updated: 2019/01/21 21:25:06 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		process_execute(char *path, char **params, t_shell *shell)
{
	pid_t parent;

//	ft_printf("execution of a prog\n");
	parent = fork();
	if (parent > 0)
		wait(NULL);
	else
		execve(path, params, (char **)shell->env->tab);
	return (0);
}

int		process_execute_command_no_path(char *full_path, char **params,
			t_shell *shell)
{
	struct stat st;

	if (access(full_path, F_OK))
		ft_printf("minishell: command not found: %s\n", params[0]);
	else
	{
		if (stat(full_path, &st) == -1)
			return (-1);
		if (S_ISDIR(st.st_mode))
			ft_printf("minishell: command not found: %s\n", params[0]);
		else if (access(full_path, X_OK))
			ft_printf("minishell: permission denied: %s\n", params[0]);
		else
		{
			return (process_execute(full_path, params, shell));
		}
	}
	return (0);
}

int		execute_command_no_path(char **params, t_shell *shell)
{
	char	cwd[CWD_LEN];
	char	*full_path;

	if (params[0][0] == '/')
		return (process_execute_command_no_path(params[0], params, shell));
	else
	{
		if (getcwd(cwd, sizeof(cwd)) == NULL)
			return (1);
		if (!(full_path = ft_strjoin_3(cwd, "/", params[0])))
			return (1);
		process_execute_command_no_path(full_path, params, shell);
		free(full_path);
		return (1);
	}
	return (0);
}

int		execute_command(char **params, t_shell *shell)
{
	char	*path_str;
	char	**path_split;
	int		i;
	char	*full_path;

	if (execute_builtin(params, shell))
		return (0);
	if ((path_str = get_env_value((char **)shell->env->tab, "PATH")))
	{
		if (!(path_split = ft_strsplit(path_str, ':')))
			return (1);
		i = 0;
		while (path_split[i])
		{
			if (get_file_in_dir(params[0], path_split[i]))
			{
				if (!(full_path = ft_strjoin_3(path_split[i], "/", params[0])))
					return (1);
				process_execute(full_path, params, shell);
				free(full_path);
				ft_free_split(path_split);
				return (0);
			}
			i++;
		}
		ft_free_split(path_split);
	}
	return (execute_command_no_path(params, shell));
}
