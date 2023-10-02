/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_handle.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalic <asalic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/12 12:43:39 by asalic            #+#    #+#             */
/*   Updated: 2023/10/02 16:53:17 by asalic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* 
 * Gestionnaire tokens && et |.
 * Parcours list d'arg pour chercher une separation: && ou |.
 * A faire: implementer token 4 |.
 * Token 3 && fonctionnel.
*/
void	check_args(t_args **list, t_shell *shell, t_args **env_list)
{
	t_args	*current;

	current = *list;
	while (current)
	{
		if (current->token == TOKEN_AND && shell->is_work == 0)
		{
			args_handle(current->next, shell, env_list);
			return ;
		}
		else if (current->token == TOKEN_OR && shell->is_work != 0)
		{
			args_handle(current->next, shell, env_list);
			return ;
		}
		else if (current->token == TOKEN_OR && shell->is_work == 0)
			return ;
		current = current->next;
	}
}

/* 
 * Gestionnaire de commandes 2.
 * Suite des conditions de args_handle.
*/
static int	args_two(t_args *list, t_shell *shell, t_args **env_list)
{
	if (ft_strncmp(list->str, "echo", ft_strlen(list->str))
		== 0 && ft_strlen(list->str) == 4)
		shell->is_work = ft_echo(list->next, env_list, shell);
	else if (ft_strncmp(list->str, "env", ft_strlen(list->str))
		== 0 && ft_strlen(list->str) == 3)
		shell->is_work = ft_env(list, env_list, shell);
	else if (ft_strncmp(list->str, "export",
			ft_strlen(list->str)) == 0 && ft_strlen(list->str) == 6)
		shell->is_work = ft_export(list, shell, env_list);
	else if (ft_strncmp(list->str, "pwd", ft_strlen(list->str))
		== 0 && ft_strlen(list->str) == 3)
		shell->is_work = ft_pwd(shell, env_list);
	else
		shell->is_work = all_cmd(list, shell, &list, env_list);
	return (shell->is_work);
}

/* 
 * Gestionnaire de commandes.
 * Recupere l'arguments et la commande separement.
 * Traite en fonction de la commande enregistree.
*/
void	args_handle(t_args *list, t_shell *shell, t_args **env_list)
{
	t_args	*current_env;

	shell->is_work = 1;
	current_env = *env_list;
	if (list == NULL)
		return ;
	if (ft_strncmp(list->str, "cd", ft_strlen(list->str))
		== 0 && ft_strlen(list->str) == 2)
		shell->is_work = ft_cd(list, shell, current_env);
	else if (ft_strncmp(list->str, "unset",
			ft_strlen(list->str)) == 0 && ft_strlen(list->str) == 5)
		shell->is_work = ft_unset(list, shell, current_env);
	else if (ft_strncmp(list->str, "exit",
			ft_strlen(list->str)) == 0 && ft_strlen(list->str) == 4)
		ft_exit(list, current_env, shell);
	else
		shell->is_work = args_two(list, shell, env_list);
	check_args(&list, shell, env_list);
	if (!(ft_strncmp(list->str, "env", ft_strlen(list->str))
			== 0 && ft_strlen(list->str) == 3))
		update_last_ve(list, env_list);
}
