/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/12 22:27:37 by ldedier           #+#    #+#             */
/*   Updated: 2019/01/12 22:27:43 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		ft_init_shell(t_shell *shell, char **env)
{
	int			i;
	t_env_entry	*entry;

	shell->running = 1;
	i = 0;
	while (env[i])
	{
		if (!(entry = ft_new_entry_from_str(env[i])))
			return (1);
		if (ft_add_to_list_back(&shell->env_container.entries,
				entry, sizeof(t_env_entry)))
			return (1);
		i++;
	}
	if (!(shell->env_container.env =
			convert_entries_to_env(shell->env_container.entries)))
		return (1);
	return (0);
}
