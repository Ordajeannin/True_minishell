/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalic <asalic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 12:09:51 by ajeannin          #+#    #+#             */
/*   Updated: 2023/07/04 15:01:32 by asalic           ###   ########.fr       */
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

typedef struct s_env
{
	char	*home;
	char	*pwd;
	char	*user;
	char	*shell;
	char	*path;
	char	*lang;
	char	*term;
	char	*hostname;
}	t_env;

int		msg(char *msg);
int		handle_env(char **env, t_env *envcpy);

//Buldins
void	ft_echo(char *args);

//Main core
void	args_handle(char *input);

//Helpful function
char	*until_char(char *str, int c);

#endif
