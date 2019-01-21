/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/12 13:37:25 by ldedier           #+#    #+#             */
/*   Updated: 2019/01/21 20:54:03 by ldedier          ###   ########.fr       */
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

int		await_command(t_shell *shell)
{
	char			*command;
	char			**params;
	char			processed_input;

	if (get_next_line(0, &command) == -1)
		return (1);
	preprocess_expansions_str(&command, shell);
	if (!(params = ft_split_whitespace(command)))
		return (1);
	free(command);
	if (ft_splitlen(params) == 0)
	{
		ft_free_split(params);
		return (0);
	}
	else
	{
	//	preprocess_expansions(params, shell);
		execute_command(params, shell);
	}
	ft_free_split(params);
	return (0);
}

int main(int argc, char **argv, char **env)
{
	t_shell		shell;

	(void)argc;
	(void)argv;

	ft_init_shell(&shell, env);
	while (shell.running)
	{
		ft_printf(CYAN BOLD "$MiShell> "EOC);
		if (await_command(&shell))
			return (1);
	}
	return (0);
}
