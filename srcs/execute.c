/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/12 22:25:21 by ldedier           #+#    #+#             */
/*   Updated: 2019/02/24 22:31:45 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static pid_t g_parent = 1;

void	handle_sigint(int signal)
{
	(void)signal;
	if (g_parent > 0)
	{
		kill(g_parent, SIGINT);
		get_down_from_command(g_glob.command);
		g_glob.cursor = 0;
		g_glob.command->current_size = 0;
		g_glob.command->current_index = 0;
		ft_bzero(g_glob.command->str, g_glob.command->max_size);
		g_glob.command->nb_chars = 0;
		render_command_line(g_glob.command, 0);
	}
}

int		process_execute(char *path, t_shell *shell)
{
	int	stat_loc;

	if (check_execute(path, shell))
		return (1);
	if (reset_shell(0) == -1)
		return (1);
	if ((g_parent = fork()) == -1)
		return (1);
	if (g_parent == 0)
	{
		if (execve(path, shell->params, (char **)shell->env->tbl) == -1)
		{
			free_all(shell);
			exit(1);
		}
	}
	else
	{
		wait(&stat_loc);
		if (set_shell(0) == -1)
			return (1);
		shell->should_display = !(WIFSIGNALED(stat_loc));
	}
	return (0);
}

int		execute_command_no_path(t_shell *shell)
{
	char	cwd[CWD_LEN];
	char	*full_path;

	if (shell->params[0][0] == '/')
		return (process_execute(shell->params[0], shell));
	else
	{
		if (getcwd(cwd, CWD_LEN) == NULL)
			return (1);
		if (!(full_path = ft_strjoin_3(cwd, "/", shell->params[0])))
			return (1);
		process_execute(full_path, shell);
		free(full_path);
		return (1);
	}
	return (0);
}

int		execute_command_path(t_shell *shell, char *path_str)
{
	char	**path_split;
	int		i;
	char	*full_path;

	if (!(path_split = ft_strsplit(path_str, ':')))
		return (1);
	i = 0;
	while (path_split[i])
	{
		if (get_file_in_dir(shell->params[0], path_split[i]))
		{
			if (!(full_path = ft_strjoin_3(path_split[i], "/",
							shell->params[0])))
				return (1);
			process_execute(full_path, shell);
			free(full_path);
			ft_free_split(path_split);
			return (0);
		}
		i++;
	}
	ft_free_split(path_split);
	return (2);
}

int		execute_command(t_shell *shell)
{
	char	*path_str;
	int		ret;

	if (execute_builtin(shell))
		return (0);
	if ((path_str = get_env_value((char **)shell->env->tbl, "PATH")))
	{
		if ((ret = execute_command_path(shell, path_str)) != 2)
			return (ret);
	}
	return (execute_command_no_path(shell));
}
