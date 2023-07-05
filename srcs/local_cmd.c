/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   local_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalic <asalic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 13:13:22 by asalic            #+#    #+#             */
/*   Updated: 2023/07/05 13:56:38 by asalic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_cd(t_args *list)
{
	char	*buf;

	if (!list->next)
		buf = getenv("HOME");
	else
		buf = list->next->str;
	if (chdir(buf) == -1)
	{
		perror("Erreur de changement de repertoire");
		return ;
	}
	else
		ft_printf("%s\n", getcwd(NULL, 0));
}

void	ft_pwd(void)
{
	if (getcwd(NULL, 0) == NULL)
	{
		ft_printf("Erreur de recuperation du repertoire courrant\n");
		return ;
	}
	else
		ft_printf("%s\n", getcwd(NULL, 0));
}
