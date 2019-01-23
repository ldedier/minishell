/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/12 13:37:25 by ldedier           #+#    #+#             */
/*   Updated: 2019/01/23 19:50:07 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void __attribute__((destructor)) end();

void    end(void)
{
//	ft_printf("destructor loop\n");
//	while(1);
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

int		await_command(t_shell *shell)
{
	char			*command;
	char			processed_input;

	if (get_next_line(0, &command) == -1)
		return (1);
	preprocess_expansions_str(&command, shell);
	if (!(shell->params = ft_split_whitespace(command)))
		return (1);
	free(command);
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

void	handle_sigint(int signal)
{
	(void)signal;
	ft_printf(CYAN BOLD "\n$MiShell> "EOC);
}

int main(int argc, char **argv, char **env)
{
	t_shell		shell;

	(void)argc;
	(void)argv;
	signal(SIGINT, handle_sigint); 
	if (ft_init_shell(&shell, env))
		return (1);
	while (shell.running)
	{
		ft_printf(CYAN BOLD "$MiShell> "EOC);
		if (await_command(&shell))
			return (1);
	}
	free_all(&shell);
	return (0);
}
