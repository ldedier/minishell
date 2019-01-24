/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/12 13:38:12 by ldedier           #+#    #+#             */
/*   Updated: 2019/01/24 19:27:00 by ldedier          ###   ########.fr       */
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
# include <signal.h>

# define CWD_LEN	2048
# define SH_NAME	"minishell"
# define LOCAL_HOME	"/Users/ldedier"

typedef enum		e_cd_opt
{
	e_cd_opt_logic,
	e_cd_opt_physic,
}					t_cd_opt;

typedef struct		s_param_rest
{
	char			*value;
	char			glue[2];
}					t_param_rest;

typedef struct		s_env_entry
{
	char			*key;
	char			*value;
}					t_env_entry;

typedef struct		s_shell
{
	char			running;
	char			should_display;
	char			**params;
	t_dy_tab		*env;
	int				expand_diff;
}					t_shell;

int					ms_cd(t_shell *shell);
int					ms_echo(char **params);
int					ms_setenv(t_shell *shell);
int					ms_unsetenv(t_shell *shell);
int					ms_env(t_shell *shell);
int					ms_exit(t_shell *shell);

int					execute_builtin(t_shell *shell);

char				*get_env_value(char **env, char *key);
char				*get_env_entry(char **env, char *key);
t_env_entry			*ft_new_entry_from_str(char *str);
char				**convert_entries_to_env(t_list *entries);
int					ft_update_env(t_shell *shell);

int					execute_command(t_shell *shell);
int					ft_init_shell(t_shell *shell, char **env);

int					get_file_in_dir(char *filename, char *dirname);
int					is_key_of_entry(char *entry, char *key);
int					preprocess_expansions(t_shell *shell);
int					ft_process_set_env_equal(char *entry, t_dy_tab *env);
int					add_to_env(t_dy_tab *env, char *key, char *value);
int					preprocess_expansions_str(char **str, t_shell *shell);
char				**split_params(char *str, t_param_rest *rest);
void				free_all(t_shell *shell);
void				handle_sigint(int signal);
#endif
