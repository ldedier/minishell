/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/30 02:34:27 by ldedier           #+#    #+#             */
/*   Updated: 2019/02/27 19:28:47 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		preprocess_expansions(t_shell *shell)
{
	int		i;
	char	*subst;

	i = 0;
	while (shell->params[i])
	{
		if (!ft_strcmp(shell->params[i], "~"))
		{
			if (!(subst = get_home_dup(shell)))
				return (-1);
			else
			{
				free(shell->params[i]);
				shell->params[i] = subst;
			}
		}
		else if ((!ft_strncmp(shell->params[i], "~/", 2) ||
			!ft_strncmp(shell->params[i], "~:", 2))
				&& (process_subst_home(shell, &shell->params[i])))
			return (-1);
		i++;
	}
	return (0);
}

int		process_command(char **command, t_shell *shell)
{
	int ret;

	ret = 0;
	if (preprocess_expansions_str(command, shell))
		return (1);
	if (!(shell->params = ft_split_whitespace(*command)))
		return (1);
	if (ft_splitlen(shell->params) == 0)
	{
		ft_free_split(shell->params);
		return (0);
	}
	else
	{
		if (preprocess_expansions(shell) == -1)
		{
			ft_free_split(shell->params);
			return (1);
		}
		if (execute_command(shell) == -1)
			ret = 1;
	}
	ft_free_split(shell->params);
	return (ret);
}

int		await_command(t_shell *shell)
{
	char			**command_split;
	int				i;
	int				ret;

	if ((ret = get_command(shell, g_glob.command)))
		return (ret);
	if (!(command_split = ft_strsplit(g_glob.command->str, ';')))
		return (1);
	else
	{
		i = 0;
		while (command_split[i] && shell->running)
		{
			if (process_command(&(command_split[i]), shell))
			{
				ft_free_split(command_split);
				return (1);
			}
			i++;
		}
		ft_free_split(command_split);
	}
	return (0);
}

int		main(int argc, char **argv, char **env)
{
	t_shell		shell;
	int			ret;

	(void)argc;
	(void)argv;
	if (init_terminal(env))
		return (1);
	init_signals();
	if (ft_init_shell(&shell, env))
	{
		ft_dprintf(2, "internal malloc error\n");
		free_all(&shell);
		return (reset_shell(1));
	}
	while (shell.running)
	{
		if ((ret = await_command(&shell)) && shell.running)
		{
			ft_dprintf(2, "internal malloc error\n");
			free_all(&shell);
			return (reset_shell(1));
		}
	}
	free_all(&shell);
	return (reset_shell(0));
}
