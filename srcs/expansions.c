/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/14 23:42:37 by ldedier           #+#    #+#             */
/*   Updated: 2019/02/14 13:23:06 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		ft_substitute_by_env(char **str, int start, int len, t_shell *shell)
{
	char	*exp_str;
	char	*env_value;
	char	*escaped_value;

	if (!len)
		return (0);
	if (!(exp_str = ft_strndup(&((*str)[start]), len)))
		return (1);
	if (!(env_value = get_env_value((char **)shell->env->tbl, exp_str)))
	{
		if (!(escaped_value = ft_strnew(0)))
			return (ft_free_turn(exp_str, 1));
	}
	else
	{
		if (!(escaped_value = ft_strdup(env_value)))
			return (ft_free_turn(exp_str, 1));
	}
	free(exp_str);
	if (ft_substitute_str(str, escaped_value, start - 1, len + 1))
		return (ft_free_turn(escaped_value, 1));
	shell->expand_diff = ft_strlen(escaped_value) - (len + 1);
	free(escaped_value);
	return (0);
}

int		process_preprocess_expansion_str(char **str, t_shell *shell, int *i,
			t_expansion_parser *p)
{
	if (p->parse_exp && ft_end_expansion((*str)[*i]))
	{
		if (ft_substitute_by_env(str, p->exp_start, *i - p->exp_start, shell))
			return (1);
		*i += shell->expand_diff;
		if ((*str)[*i] != '$')
			p->parse_exp = 0;
		else
			p->exp_start = *i + 1;
	}
	else if (!p->parse_exp && (*str)[*i] == '$')
	{
		p->parse_exp = 1;
		p->exp_start = *i + 1;
	}
	return (0);
}

int		preprocess_expansions_str(char **str, t_shell *shell)
{
	int					i;
	t_expansion_parser	p;

	p.parse_exp = 0;
	i = 0;
	while ((*str)[i])
	{
		if (process_preprocess_expansion_str(str, shell, &i, &p))
			return (1);
		i++;
	}
	if (p.parse_exp)
	{
		if (ft_substitute_by_env(str, p.exp_start, i - p.exp_start, shell))
			return (1);
		i += shell->expand_diff;
	}
	return (0);
}
