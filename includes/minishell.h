/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalic <asalic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 12:09:51 by ajeannin          #+#    #+#             */
/*   Updated: 2023/07/05 11:05:19 by asalic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H

# define MINISHELL_H

# include <stdio.h>
# include <unistd.h>
# include <limits.h>
# include <stdlib.h>
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
	char	*user;
	char	*shell;
	char	*path;
	char	*lang;
	char	*term;
	char	*hostname;
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
void	from_input_to_list_of_args(char **input, t_args **list);
void	clear_args_list(t_args **list);

//Buldins
void	args_echo(char *args);
void	ft_echo(char *args);

//Main core
void	args_handle(char *input);

//Helpful function
char	*until_char(char *str, int c);

#endif
