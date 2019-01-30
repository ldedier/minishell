/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/23 16:49:58 by ldedier           #+#    #+#             */
/*   Updated: 2019/01/30 05:39:43 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**ft_split_cpy_index(char **split, int index)
{
	char	**res;
	int		len;
	char	*str;
	int		i;

	len = ft_splitlen(split) - index;
	if (len < 0)
		return (NULL);
	if (!(res = (char **)malloc(sizeof(char *) * (len + 1))))
		return (NULL);
	i = index;
	while (split[i])
	{
		if (!(str = ft_strdup(split[i])))
		{
			while (--i - index >= 0)
				free(res[i - index]);
			free(res);
			return (NULL);
		}
		res[i - index] = str;
		i++;
	}
	res[i - index] = 0;
	return (res);
}

int		get_env_dump(int *i, t_shell *shell)
{
	int dump;

	dump = 0;
	*i = 1;
	while (shell->params[*i])
	{
		if (!ft_strcmp(shell->params[*i], "-i"))
			dump = 1;
		else if (!ft_strcmp(shell->params[*i], "--"))
		{
			*i += 1;
			break ;
		}
		else
			break ;
		*i += 1;
	}
	return (dump);
}

int		process_env_arg(t_shell *shell, int i, char **params_save,
			t_dy_tab *env_save)
{
	char		*str;

	if (ft_is_in_str(shell->params[i], '='))
	{
		if (!(str = ft_strdup(shell->params[i])))
			return (-1);
		if (ft_process_set_env_equal(str, shell->env) == -1)
			return (ft_free_turn(str, 1));
		free(str);
	}
	else
	{
		if (!(shell->params = ft_split_cpy_index(shell->params, i)))
		{
			shell->params = params_save;
			return (-1);
		}
		execute_command_2(shell, get_env_value((char **)(env_save)->tab,
			"PATH"));
		ft_free_split(shell->params);
		shell->params = params_save;
		return (2);
	}
	return (0);
}

int		process_env_args(int i, t_shell *shell, t_dy_tab *env_save)
{
	char		**params_save;
	int			ret;

	params_save = shell->params;
	while (shell->params[i])
	{
		if ((ret = process_env_arg(shell, i, params_save, env_save)))
			return (ret);
		i++;
	}
	return (1);
}

int		ms_env(t_shell *shell)
{
	int			i;
	t_dy_tab	*env_save;
	int			ret;

	env_save = shell->env;
	i = 0;
	if (get_env_dump(&i, shell) == 1)
	{
		if (!(shell->env = ft_dy_tab_new(63)))
			return (-1);
	}
	else
	{
		if (!(shell->env = ft_dy_tab_cpy_str(env_save)))
			return (-1);
	}
	if ((ret = process_env_args(i, shell, env_save) != 2))
	{
		if (ret == -1)
			return (-1);
		process_ms_env(shell->env);
	}
	ft_dy_tab_del(shell->env);
	shell->env = env_save;
	return (1);
}
