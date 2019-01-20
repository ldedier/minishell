/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/12 13:38:12 by ldedier           #+#    #+#             */
/*   Updated: 2019/01/20 05:45:03 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
# include <stdio.h>
# include "libft.h"
# include <dirent.h>
# include <sys/stat.h>
# include <sys/wait.h>

# define CWD_LEN	2048
# define SH_NAME	"minishell"

typedef enum		e_cd_opt
{
	e_cd_opt_logic,
	e_cd_opt_physic,
}					t_cd_opt;

typedef struct		s_env_entry
{
	char			*key;
	char			*value;
}					t_env_entry;

typedef struct		s_shell
{
	char			running;
	t_dy_tab		*env;
}					t_shell;

int					ms_cd(char **params, t_shell *shell);
int					ms_echo(char **params);
int					ms_setenv(char **params, t_shell *shell);
int					ms_unsetenv(char **params, t_shell *shell);
int					ms_env(char **params, t_shell *shell);
int					ms_exit(char **params, t_shell *shell);

int					execute_builtin(char **params, t_shell *shell);

char				*get_env_value(char **env, char *key);
char				*get_env_entry(char **env, char *key);
t_env_entry			*ft_new_entry_from_str(char *str);
char				**convert_entries_to_env(t_list *entries);
int					ft_update_env(t_shell *shell);

int					execute_command(char **params, t_shell *shell);
int					ft_init_shell(t_shell *shell, char **env);

int					get_file_in_dir(char *filename, char *dirname);
int					is_key_of_entry(char *entry, char *key);
int					preprocess_expansions(char **params, t_shell *shell);
int					add_to_env(t_shell *shell, char *key, char *value);
#endif
