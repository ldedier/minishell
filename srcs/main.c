/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/12 13:37:25 by ldedier           #+#    #+#             */
/*   Updated: 2019/01/12 22:30:52 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		await_command(t_shell *shell)
{
	char	*command;
	char	**params;

	if (get_next_line(0, &command) == -1)
		return (1);
	if (!(params = ft_split_whitespace(command)))
		return (1);
	free(command);
	if (ft_splitlen(params) == 0)
		return (0);
	else
		execute_command(params, shell);
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
		ft_printf(CYAN BOLD "$MyMiniShell> "EOC);
		await_command(&shell);
	}
	return (0);
}
