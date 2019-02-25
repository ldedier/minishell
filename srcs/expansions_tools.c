/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/14 23:42:37 by ldedier           #+#    #+#             */
/*   Updated: 2019/02/25 22:23:35 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		describe_entry(char *param, char *entry, char **env_value)
{
	int i;

	if (param[0] != '$')
		return (0);
	i = 1;
	while (param[i] && entry[i - 1] && param[i] == entry[i - 1] &&
			entry[i - 1] != '=')
		i++;
	if (!param[i] && entry[i - 1] == '=')
	{
		*env_value = ft_strnrest(entry, i);
		return (1);
	}
	return (0);
}

int		ft_end_expansion(char c)
{
	return (ft_isseparator(c) || c == '$' || c == '~' || c == '/'
			|| c == ':' || c == '=');
}
