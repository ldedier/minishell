/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/12 13:38:12 by ldedier           #+#    #+#             */
/*   Updated: 2019/02/19 21:18:51 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
# include "libft.h"
# include <dirent.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <signal.h>
# include <termcap.h>
# include <term.h>
# include <sys/ioctl.h>
# include <fcntl.h>
# include <stdio.h>
# include <errno.h>

# define CWD_LEN	2048
# define SH_NAME	"minishell"
# define LOCAL_HOME	"/Users/ldedier"
# define PROMPT		"$minishell> "

# define CTRL_D				2
# define READ_BUFF_SIZE		4

typedef struct		s_xy
{
	int				x;
	int				y;
}					t_xy;

typedef enum		e_cd_opt
{
	e_cd_opt_logic,
	e_cd_opt_physic
}					t_cd_opt;

typedef struct		s_env_entry
{
	char			*key;
	char			*value;
}					t_env_entry;

typedef struct		s_expansion_parser
{
	char			parse_exp;
	int				exp_start;
}					t_expansion_parser;

typedef struct		s_shell
{
	char			running;
	char			should_display;
	char			**params;
	t_dy_tab		*env;
	int				expand_diff;
}					t_shell;

/*
** cursor: index in command_line
*/

typedef struct		s_glob
{
	struct termios	term;
	struct termios	term_init;
	t_dy_str		*command;
	int				cursor;
	struct winsize	winsize;
}					t_glob;

t_glob				g_glob;

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
int					execute_command_2(t_shell *shell, char *path_str);
int					execute_command_no_builtin(t_shell *shell);
int					ft_init_shell(t_shell *shell, char **env);

int					get_file_in_dir(char *filename, char *dirname);
int					is_key_of_entry(char *entry, char *key);
int					preprocess_expansions(t_shell *shell);
int					ft_process_set_env_equal(char *entry, t_dy_tab *env);
int					add_to_env(t_dy_tab *env, char *key, char *value);
int					preprocess_expansions_str(char **str, t_shell *shell);
void				free_all(t_shell *shell);
void				handle_sigint(int signal);
int					ft_update_old_pwd(char *old_pwd, char *path, t_cd_opt flag,
						t_shell *shell);
int					process_ms_env(t_dy_tab *env);
char				*get_escaped_str(char *str);
int					ft_end_expansion(char c);
int					check_execute(char *full_path, t_shell *shell);
int					execute_command_path(t_shell *shell, char *path_str);
int					execute_command_no_path(t_shell *shell);
char				*get_sanitized_path_from_old(char *old_pwd, char *path);
int					end_with_char(char *str, char c);
int					process_subst_home(t_shell *shell, int param_index);
char				*get_home_dup(t_shell *shell);
void				init_signals(void);
int					init_terminal(char **env);
int					clear_all(void);
void				move(int x, int y);
int					putchar_int(int i);
int					reset_shell(int ret);
int					set_shell(int ret);
int					get_command(t_shell *shell, t_dy_str *command);
int					render_command_line(t_dy_str *command, int carry);
int					get_true_cursor_pos(int cursor);
void				get_down_from_command(t_dy_str *command);
#endif
