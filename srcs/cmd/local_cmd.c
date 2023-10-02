/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   local_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalic <asalic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 13:13:22 by asalic            #+#    #+#             */
/*   Updated: 2023/10/02 11:13:07 by asalic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* 
 * Affiche le repertoire courrant 
 * Fonction a l'image de 'pwd'
*/
int	ft_pwd(t_shell *shell, t_args **env_list)
{
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
	if (!change_error(&env_list, shell, 0))
		return (1);
	return (0);
}
