/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalic <asalic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 12:09:51 by ajeannin          #+#    #+#             */
/*   Updated: 2023/09/26 16:46:04 by asalic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H

# define MINISHELL_H

# include <stdio.h>
# include <unistd.h>
# include <fcntl.h>
# include <limits.h>
# include <stdlib.h>
# include <errno.h>
# include <signal.h>
# include <sys/ioctl.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <dirent.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <string.h>
# include <ctype.h>
# include "libft.h"

# define ERROR_NOENV 	"env is not found"
# define ERROR			-1

# define TOKEN_CMD		1
# define TOKEN_OPTION	2
# define TOKEN_AND		3
# define TOKEN_PIPE		4
# define TOKEN_INFILE	5
# define TOKEN_OUTFILE	6
# define TOKEN_DELIM	7
# define TOKEN_APPEND	8
# define TOKEN_FPATH	9
# define TOKEN_RPATH	10
# define TOKEN_OR		11
# define TOKEN_S_QUOTES 22
# define TOKEN_D_QUOTES 23
# define TOKEN_INTERDOT 0
# define TOKEN_INTERDOT_D_QUOTES 230
# define TOKEN_TEMP_VAR	666
# define ABORT_MISSION	42

extern int	g_error;

struct s_test
{
	int	var_c;
	int	var_d;
};

typedef struct s_shell
{
	char	*home;
	char	*pwd;
	char	*is_pwd;
	char	*is_oldpwd;
	char	*oldpwd;
	char	*user;
	char	*shell;
	char	*path;
	char	*lang;
	char	*term;
	char	*hostname;
	char	*shlvl;
	char	**cmd_paths;
	char	**input;
	int		is_work;
	char	*input_bis;
}	t_shell;

typedef struct s_args
{
	char			*str;
	int				token;
	struct s_args	*next;
}	t_args;

int		is_minishell(t_shell *shell, t_args *env_list, t_args *list, \
	char *user);
void	ft_gain_place(char **av, t_args **list, char **input, \
	t_args **env_list);
int		msg(char *msg);
int		handle_env(char **env, t_shell *envcpy);
void	init_shell(t_shell *shell);
int		parsing_input(char **input);
void	args_handle(t_args *list, t_shell *shell, t_args **env_list, \
	char *input);
void	was_unclosed_quotes(t_args **list);
size_t	is_quotes(char *str, t_args **list, const char *input, int flag);
void	process_not_s_quotes(t_args *node, t_args **env_list, int flag);
void	is_there_a_redirection(t_args **list);
void	c_est_ma_direction(int token, t_args **list);
int		is_correct_format(t_args **list);

//Tok
char	*ft_strtok(char *input, char **delim, t_args **list);
void	update_args(t_args **list, t_args **env_list);
int		tokenize_args(char *input, int flag);
size_t	delimit_to_token(char *s, t_args **list, const char **input);
char	*is_env_var(char *str, t_args **env_list, int flag);
char	**ft_split_arg(char *str);

//libc parsing
int		ft_strcmp(const char *s1, const char *s2);
char	*ft_strchr(const char *s, int c);
char	*ft_strpbrk(const char *s, const char *charset);
size_t	ft_strspn(const char *s, char **accept, t_args **list);
size_t	ft_strcspn(const char *s, char **reject, t_args **list);
char	*ft_strncpy(char *dst, const char *src, int n);
char	*ft_strrchr(const char *str, int ch);
int		is_alphanum(char c);
void	*ft_realloc(void *ptr, size_t old_size, size_t new_size);
char	*ft_strcat(char *str1, const char *str2);

//List concerns
void	from_input_to_list_of_args(char *input, t_args **list, t_args **e_list);
void	loop_args(t_shell *shell, t_args **list);
void	clear_args_list(t_args **list);
void	add_arg(t_args **list, char *str, int token);

//Bultins
int		find_opt(char *s1);
int		ft_echo(t_args *list, t_args **env_list);
int		ft_cd(t_args *list, t_shell *shell, t_args *env_list);
int		ft_pwd(t_shell *shell, t_args **env_list);
int		ft_env(t_args *list, t_args **env_list);
int		ft_unset(t_args *list, t_shell *shell, t_args *env_list);
int		ft_export(t_args *list, t_shell *shell, t_args **env_list);
int		ft_exit(char *input, t_args *list, t_args *env_list);

//Other commands
int		all_cmd(t_args *arg, t_shell *shell, t_args **list, t_args **env_list);
void	change_env_cd(t_args **env_list, char *new_str, char *change_value);
void	cd_move_and_change(t_args *env_list, t_shell *shell);
int		change_env_exp(t_args **env_list, char *name_env, char *value);
int		update_last_ve(t_args **list, t_args **env_list);
int		parse_export(t_args *list);
int		change_error(t_args **env_list, int value);
int		searchin_env(t_args **env_list, t_args *list);
char	*is_path_or_cmd(char **paths, char *cmd, t_shell *shell, \
	t_args **env_list);
void	shell_change(t_shell *shell, char *str, char *value);
int		set_env(t_args **env_list, char **env, t_shell *shell);
void	add_env(t_args **env_list, char *str);
void	ft_plus_shell(t_shell *shell, t_args **env_list);
void	signal_handler(int sig);
int		export_out_args(t_args **env_list);

void	code_error(int code);
int		handle_error(int code_err);

//Helpful function
char	*ft_strdupto_n(char *str, char c);
char	*ft_strdup_from(char *str, char c);
char	*from_end_to_char(char *str, char c);
int		is_only_equal(char *str, char c);
int		len_targs(t_args *list);

int		ft_strlen_double(char **str);
int		count_back(char	*str);
int		count_dir(t_shell *shell);
char	**dup_double_string(t_args **e_list);
int		is_numeric(char *str);

//Print Things
void	shell_style(t_shell *shell);
void	print_args_list(t_args **list);
char	*get_username(t_args **env_list);
char	*get_pwd(void);
int		set_empty_env(t_shell *shell, t_args **env_list);
char	*prompt_cmd(t_shell *shell, char *user);

#endif
