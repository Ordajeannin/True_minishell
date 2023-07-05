/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   local_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalic <asalic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 13:13:22 by asalic            #+#    #+#             */
/*   Updated: 2023/07/05 15:03:29 by asalic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* 
 * Change de repertoire
 * Fonction a l'image de cd
*/
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

/* 
 * Affiche le repertoire courrant 
 * Fonction a l'image de 'pwd'
*/
void	ft_pwd(void)
{
	if (getcwd(NULL, 0) == NULL)
	{
		perror("Erreur de recuperation du repertoire courrant\n");
		return ;
	}
	else
		ft_printf("%s\n", getcwd(NULL, 0));
}

/* 
 * Fonction a l'image de 'env'
 * Affiche l'environnement du shell en entier
 * (Attention : env -i ./minishell doit afficher PWD, SHLVL et _)
*/
void	ft_env(void)
{
	extern char	**environ;
	int			i;

	i = 0;
	while (environ[i] != NULL)
		ft_printf("%s\n", environ[i++]);
}
