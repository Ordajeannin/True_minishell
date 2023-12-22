/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   local_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkorsako <pkorsako@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 13:13:22 by asalic            #+#    #+#             */
/*   Updated: 2023/12/19 17:41:15 by pkorsako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* 
 * Affiche le repertoire courrant 
 * Fonction a l'image de 'pwd'
*/
int	ft_pwd(t_shell *shell)
{
	if (shell->secret_pwd)
		printf("%s\n", shell->secret_pwd);
	else
	{
		printf("pwd: error retrieving current directory:");
		set_error_nb(1, YES);
	}
	set_error_nb(0, YES);
	return (1);
}

/* 
 * Boucle principale d'unset.
 * Cherche une VE et la supprime s'il la trouve.
*/
int	searchin_env(t_args **env_list, t_args *list, t_shell *shell)
{
	t_args	*current;
	t_args	*temp;
	char	*name_env;

	current = *env_list;
	if (!ft_strcmp(list->next->str, ft_strdupto_n(current->str, '=')))
	{
		shell->env_list = (*env_list)->next;
		*env_list = (*env_list)->next;
		return (1);
	}		
	while (current && current->next)
	{
		name_env = ft_strdupto_n(current->next->str, '=');
		if (!name_env)
			return (1);
		if (ft_strcmp(list->next->str, name_env) == 0)
		{
			printf("VAR found\n");
			return (help_s_e(&temp, &current));
		}
		current = current->next;
	}
	printf("VAR didn't found\n");
	return (1);
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
	searchin_env(&env_list, list, shell);
	// if (searchin_env(&env_list, list, shell))
		// shell_change(shell, list->next->str, NULL);
	if (list->next->next != NULL)
		ft_unset(list->next, shell, env_list);
	set_error_nb(0, YES);
	return (0);
}
