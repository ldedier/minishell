/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/14 23:42:37 by ldedier           #+#    #+#             */
/*   Updated: 2019/01/30 05:44:18 by ldedier          ###   ########.fr       */
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
	return (ft_isseparator(c) || c == '$' || c == '~');
}

int		should_escape_char(char c)
{
	return (c == '"' || c == '\\');
}

char	*get_escaped_str(char *str)
{
	int		i;
	int		len;
	char	*res;

	i = 0;
	len = 0;
	while (str[i])
	{
		if (should_escape_char(str[i++]))
			len++;
		len++;
	}
	if (!(res = ft_strnew(len)))
		return (NULL);
	i = 0;
	len = 0;
	while (str[i])
	{
		if (should_escape_char(str[i]))
			res[len++] = '\\';
		res[len++] = str[i++];
	}
	res[len] = '\0';
	return (res);
}
