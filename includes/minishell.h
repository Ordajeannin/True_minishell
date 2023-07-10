/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalic <asalic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 12:09:51 by ajeannin          #+#    #+#             */
/*   Updated: 2023/07/10 11:12:41 by asalic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H

# define MINISHELL_H

# include <stdio.h>
# include <unistd.h>
# include <limits.h>
# include <stdlib.h>
# include <errno.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>
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
void	from_input_to_list_of_args(char *input, t_args **list);
void	clear_args_list(t_args **list);
int		parsing_input(char **input);
char	*ft_strtok(char *input, char **delim, t_args **list);
void	add_arg(t_args **list, char *str, int token);
void	update_args(t_args **list);
int		tokenize_args(char *input);

//Bultins
int		find_opt(char *s1, char *s2);
void	ft_echo(t_args *list);
void	ft_cd(t_args *list, t_shell *shell, t_args *env_list);
void	ft_pwd(void);
void	ft_env(t_args *list, t_args *env_list);
void	ft_unset(t_args *list, t_shell *shell, t_args *env_list);

//Other commands
void	all_cmd(t_args *list, t_shell *shell);
void	change_env(t_args **env_list, char *new_str, char *change_value);
char	*extract_cmd_path(char **paths, char *cmd);
void	shell_change(t_shell *shell, t_args *list);

//Helpful function
char	*until_char(char *str, int c);

//Print Things
void	shell_style(t_shell *shell);
void	print_args_list(t_args **list);

#endif
