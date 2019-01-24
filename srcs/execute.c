/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/12 22:25:21 by ldedier           #+#    #+#             */
/*   Updated: 2019/01/24 19:31:58 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static pid_t g_parent = 1;

void	handle_sigint(int signal)
{
	if (g_parent > 0)
	{
		kill(g_parent, SIGINT);
		ft_printf(CYAN BOLD "\n$MiShell> "EOC);
	}
}

int		check_execute(char *full_path, t_shell *shell)
{
	int i;

	struct stat st;
	ft_printf("%s\n", full_path);
	i = access(full_path, F_OK);
	while (1) ;
	ft_printf("STUCK IN WHILE 1\n");
	if (access(full_path, F_OK))
	{
		ft_printf("minishell: command not found: %s\n", shell->params[0]);
		return (1);
	}
	else
	{
		if (stat(full_path, &st) == -1)
			return (-1);
		if (S_ISDIR(st.st_mode))
		{
			ft_printf("minishell: command not found: %s\n", shell->params[0]);
			return (1);
		}
		else if (access(full_path, X_OK))
		{
			ft_printf("minishell: permission denied: %s\n", shell->params[0]);
			return (1);
		}
	}
	return (0);
}

int		process_execute(char *path, t_shell *shell)
{
	int	stat_loc;

	if (check_execute(path, shell)) //LEAKS
		return (1);
	if ((g_parent = fork()) == -1)
		return (1);
	if (g_parent == 0)
	{
		if (execve(path, shell->params, (char **)shell->env->tab) == -1)
			exit(1);
	}
	else
	{
		wait(&stat_loc);
		shell->should_display = !(WIFSIGNALED(stat_loc));
	}
	ft_printf("OUAI\n");
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

int		execute_command(t_shell *shell)
{
	char	*path_str;
	char	**path_split;
	int		i;
	char	*full_path;

	if (execute_builtin(shell))
		return (0);
	if ((path_str = get_env_value((char **)shell->env->tab, "PATH")))
	{
		if (!(path_split = ft_strsplit(path_str, ':')))
			return (1);
		i = 0;
		while (path_split[i])
		{
			if (get_file_in_dir(shell->params[0], path_split[i]))
			{
				if (!(full_path = ft_strjoin_3(path_split[i], "/", shell->params[0])))
					return (1);
				process_execute(full_path, shell);
				free(full_path);
				ft_free_split(path_split);
				return (0);
			}
			i++;
		}
		ft_free_split(path_split);
	}
	return (execute_command_no_path(shell));
}
