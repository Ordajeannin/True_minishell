/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalic <asalic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 12:09:51 by ajeannin          #+#    #+#             */
/*   Updated: 2023/07/11 18:20:28 by asalic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H

# define MINISHELL_H

# include <stdio.h>
# include <unistd.h>
# include <limits.h>
# include <stdlib.h>
# include <errno.h>
# include <signal.h>
# include <sys/types.h>
# include <sys/wait.h>
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

typedef struct s_shell
{
	char	*home;
	char	*pwd;
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
}	t_shell;

typedef struct s_args
{
	char			*str;
	int				token;
	struct s_args	*next;
}	t_args;

int		msg(char *msg);
int		handle_env(char **env, t_shell *envcpy);
int		parsing_input(char **input);

//Tok
char	*ft_strtok(char *input, char **delim, t_args **list);
void	update_args(t_args **list, t_args **env_list);
int		tokenize_args(char *input);
void	delimit_to_token(char *s, t_args **list);
char	*is_env_var(char *str, t_args **env_list);

//libc parsing
int		ft_strcmp(const char *s1, const char *s2);
char	*ft_strchr(const char *s, int c);
char	*ft_strpbrk(const char *s, const char *charset);
size_t	ft_strspn(const char *s, char **accept, t_args **list);
size_t	ft_strcspn(const char *s, char **reject, t_args **list);
char	*ft_strncpy(char *dst, const char *src, int n);
char	*ft_strrchr(const char *str, int ch);

//List concerns
void	from_input_to_list_of_args(char *input, t_args **list, t_args **e_list);
void	loop_args(t_shell *shell, t_args **list);
void	clear_args_list(t_args **list);
void	add_arg(t_args **list, char *str, int token);

//Bultins
int		find_opt(char *s1, char *s2);
void	ft_echo(t_args *list);
void	ft_cd(t_args *list, t_shell *shell, t_args *env_list);
void	ft_pwd(void);
void	ft_env(t_args *list, t_args **env_list);
void	ft_unset(t_args *list, t_shell *shell, t_args *env_list);
void	ft_export(t_args *list, t_shell *shell, t_args **env_list);

//Other commands
void	all_cmd(t_args *arg, t_shell *shell, t_args **list);
void	change_env_cd(t_args **env_list, char *new_str, char *change_value);
int		change_env_exp(t_args **env_list, char *name_env, char *value);
int		searchin_env(t_args **env_list, t_args *list);
char	*extract_cmd_path(char **paths, char *cmd);
void	shell_change(t_shell *shell, char *str, char *value);
int		set_env(t_args **env_list, char **env, t_shell *shell);

//Helpful function
char	*ft_strdupto_n(char *str, char c);
char	*ft_strdup_from(char *str, char c);

//Print Things
void	shell_style(t_shell *shell);
void	print_args_list(t_args **list);
char	*prompt_cmd(t_shell *shell);

#endif
