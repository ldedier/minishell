/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/14 23:42:37 by ldedier           #+#    #+#             */
/*   Updated: 2019/01/23 00:06:23 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		describe_entry(char *param, char *entry, char **env_value)
{
	int i;

	if (param[0] != '$')
		return (0);
	i = 1;
	while (param[i] && entry[i - 1]	&&
			param[i] ==  entry[i - 1] && entry[i - 1] != '=')
		i++;
	if (!param[i] && entry[i - 1] == '=')
	{
		*env_value = ft_strnrest(entry, i);
		return (1);
	}
	return (0);
}

int		get_env_expansion_value(char *param, t_dy_tab *env, char **env_value)
{
	int i;

	i = 0;
	if (!ft_strcmp(param, "~"))
	{
		while (env->tab[i])
		{
			if (describe_entry("$HOME", env->tab[i] , env_value))
				return (1);
			i++;
		}
	}
	else
	{
		while (env->tab[i])
		{
			if (describe_entry(param, env->tab[i], env_value))
				return (1);
		i++;
		}
	}
	*env_value = ft_strdup("");
	return (1);
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

void	ft_suppr_n_char_index(char *str, int index, int len)
{
	int i;

	i = index;
	while (str[i + len])
	{
		str[i] = str[i + len];
		i++;
	}
	str[i] = '\0';
}

int		ft_inject_str_in_str(char **str, char *to_inject, int start_index)
{
	char	*tmp;
	int		receiver_len;
	int		to_inject_len;

	to_inject_len = ft_strlen(to_inject);
	receiver_len = ft_strlen(*str);
	if (!(tmp = ft_strnew(to_inject_len + receiver_len)))
		return (1);
	ft_memcpy(tmp, *str, start_index);
	ft_memcpy(&tmp[start_index], to_inject, to_inject_len);
	ft_memcpy(&tmp[start_index + to_inject_len], &((*str)[start_index]),
			receiver_len - start_index);
	free(*str);
	*str = tmp;
	return (0);
}

/*
** realloc str and inject to_inject at the index_to_inject at the place of 
** the len character index starting from that index
*/

int		ft_substitute_str(char **str, char *to_inject,
			int index_to_inject, int len)
{
	ft_suppr_n_char_index(*str, index_to_inject, len);
	if (ft_inject_str_in_str(str, to_inject, index_to_inject))
		return (1);
	return (0);
}

int		ft_substitute_by_env(char **str, int start, int len, t_shell *shell)
{
	char	*exp_str;
	char	*env_value;
	char	*escaped_value;

	if (!len)
		return (0);
	if (!(exp_str = ft_strndup(&((*str)[start]), len)))
		return (1);
	if (!(env_value = get_env_value((char **)shell->env->tab, exp_str)))
	{
		if (!(escaped_value = ft_strnew(0)))
			return (ft_free_turn(exp_str, 1));
	}
	else
	{
		if (!(escaped_value = get_escaped_str(env_value)))
			return (ft_free_turn(exp_str, 1));
	}
	free(exp_str);
	if (ft_substitute_str(str, escaped_value, start - 1, len + 1))
		return (ft_free_turn(escaped_value, 1));
	shell->expand_diff = ft_strlen(escaped_value) - (len + 1);
	free(escaped_value);
	return (0);
}

int		preprocess_expansions_str(char **str, t_shell *shell)
{
	int		i;
	char	parse_exp;
	int		exp_start;

	parse_exp = 0;
	i = 0;
	while ((*str)[i])
	{
		if (parse_exp && ft_end_expansion((*str)[i]))
		{
			if (ft_substitute_by_env(str, exp_start, i - exp_start, shell))
				return (1);

			i += shell->expand_diff;
			if ((*str)[i] != '$')
				parse_exp = 0;
			else
				exp_start = i + 1;
		}
		else if (!parse_exp && (*str)[i] == '$')
		{
			parse_exp = 1;
			exp_start = i + 1;
		}
		i++;
	}
	if (parse_exp)
	{
		if (ft_substitute_by_env(str, exp_start, i - exp_start, shell))
			return (1);
		i += shell->expand_diff;
	}
	return (0);
}
