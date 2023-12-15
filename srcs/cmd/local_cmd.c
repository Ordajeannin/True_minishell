/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   local_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkorsako <pkorsako@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 13:13:22 by asalic            #+#    #+#             */
/*   Updated: 2023/12/12 14:51:05 by pkorsako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* 
 * Affiche le repertoire courrant 
 * Fonction a l'image de 'pwd'
*/
int	ft_pwd(t_shell *shell, t_args **env_list)
{
	if (shell->secret_pwd)
		printf("%s\n", shell->secret_pwd);
	// else
	// {
	// 	printf("pwd: error retrieving current directory:");
	// 	printf(" No such file or directory");
	// 	set_error_nb(1, YES);
	// }
	set_error_nb(0, YES);
	return (1);
	////AWENA//////
	if (shell->is_pwd == NULL)
	{
		change_error(env_list, shell, errno);
		return (1);
	}
	else
		ft_printf("%s\n", shell->is_pwd);
	if (!change_error(env_list, shell, 0))
		return (1);
	return (0);
}

/* 
 * Fonction unset.
 * Supprime une variable d'environnement appelee.
 * Change aussi les VE saved dans struct t_shell a NULL.
 * Si elle est vide ou n'existe pas, renvoie juste l'invite de commande.
*/
int	ft_unset(t_args *list, t_shell *shell, t_args *env_list)
{
	if (!list->next)
		return (1);
	if (searchin_env(&env_list, list))
		shell_change(shell, list->next->str, NULL);
	if (list->next->next != NULL)
		ft_unset(list->next, shell, env_list);
	set_error_nb(0, YES);
	// if (!change_error(&env_list, shell, 0))
	// 	return (1);
	return (0);
}
