/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalic <asalic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 12:09:51 by ajeannin          #+#    #+#             */
/*   Updated: 2023/07/04 16:27:47 by ajeannin         ###   ########.fr       */
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

int		msg(char *msg);
int		handle_env(char **env, t_shell *envcpy);

//Buldins
void	ft_echo(char *args);

//Main core
void	args_handle(char *input);

//Helpful function
char	*until_char(char *str, int c);

#endif
