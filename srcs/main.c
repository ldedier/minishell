/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/30 02:34:27 by ldedier           #+#    #+#             */
/*   Updated: 2019/01/30 05:50:16 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void __attribute__((destructor)) end();

void    end(void)
{
		ft_printf("destructor loop\n");
		while(1);
}

void	ft_print_params(char **params)
{
	int i;

	i = 0;
	while (params[i])
	{
		ft_printf("%d: %s\n", i, params[i]);
		i++;
	}
}

int		preprocess_expansions(t_shell *shell)
{
	int		i;
	char	*subst;
	char	*str;

	i = 0;
	while (shell->params[i])
	{
		if (!ft_strcmp(shell->params[i], "~"))
		{
			if (!(str = get_env_value((char **)shell->env->tab, "HOME")))
				subst = ft_strdup(LOCAL_HOME);
			else
				subst = ft_strdup(str);
			if (!subst)
				return (-1);
			else
			{
				free(shell->params[i]);
				shell->params[i] = subst;
			}
		}
		i++;
	}
	return (0);
}

int		process_command(char **command, t_shell *shell)
{
	preprocess_expansions_str(command, shell);
	if (!(shell->params = ft_split_whitespace(*command)))
		return (1);
	if (ft_splitlen(shell->params) == 0)
	{
		ft_free_split(shell->params);
		return (0);
	}
	else
	{
		preprocess_expansions(shell);
		execute_command(shell);
	}
	ft_free_split(shell->params);
	return (0);
}

int		await_command(t_shell *shell)
{
	char			*command;
	char			**command_split;
	int				i;

	if (get_next_line(0, &command) == -1)
		return (1);
	if (!(command_split = ft_strsplit(command, ';')))
		return (ft_free_turn(command, 1));
	else
	{
		free(command);
		i = 0;
		while (command_split[i] && shell->running)
		{
			process_command(&(command_split[i]), shell);
			i++;
		}
		ft_free_split(command_split);
	}
	return (0);
}

int		main(int argc, char **argv, char **env)
{
	t_shell		shell;

	(void)argc;
	(void)argv;
	signal(SIGINT, handle_sigint);
	if (ft_init_shell(&shell, env))
	{
		ft_dprintf(2, "internal malloc error\n");
		return (1);
	}
	while (shell.running)
	{
		if (shell.should_display)
			ft_printf(CYAN"%s$MiShell> "EOC, BOLD);
		shell.should_display = 1;
		if (await_command(&shell))
		{
			ft_dprintf(2, "internal malloc error\n");
			free_all(&shell);
			return (1);
		}
	}
	free_all(&shell);
	return (0);
}
