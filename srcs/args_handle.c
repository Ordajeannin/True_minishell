/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_handle.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalic <asalic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/12 12:43:39 by asalic            #+#    #+#             */
/*   Updated: 2023/07/20 12:07:39 by asalic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* 
 * Gestionnaire tokens && et |.
 * Parcours list d'arg pour chercher une separation: && ou |.
 * A faire: implementer token 4 |.
 * Token 3 && fonctionnel.
*/
void	check_args(t_args **list, t_shell *shell, t_args **env_list, \
	char *input)
{
	t_args	*current;

	current = *list;
	while (current)
	{
		if (current->token == TOKEN_AND)
		{
			args_handle(current->next, shell, env_list, input);
			return ;
		}
		current = current->next;
	}
}

/* 
 * Gestionnaire de commandes 2.
 * Suite des conditions de args_handle.
*/
static int	args_two(t_args *list, t_shell *shell, t_args **env_list)
{
	int	work;

	work = 1;
	if (ft_strncmp(list->str, "env", ft_strlen(list->str))
		== 0)
		work = ft_env(list, env_list, shell);
	else if (ft_strncmp(list->str, "export",
			ft_strlen(list->str)) == 0)
		work = ft_export(list, shell, env_list);
	else if (ft_strncmp(list->str, "pwd", ft_strlen(list->str))
		== 0)
		work = ft_pwd(shell);
	else
		work = all_cmd(list, shell, &list);
	return (work);
}

/* 
 * Gestionnaire de commandes.
 * Recupere l'arguments et la commande separement.
 * Traite en fonction de la commande enregistree.
*/
void	args_handle(t_args *list, t_shell *shell, t_args **env_list, \
	char *input)
{
	int		is_work;
	t_args	*current_env;

	is_work = 1;
	current_env = *env_list;
	if (list == NULL)
		return ;
	else if (ft_strncmp(list->str, "echo", ft_strlen(list->str))
		== 0)
		is_work = ft_echo(list, shell);
	else if (ft_strncmp(list->str, "cd", ft_strlen(list->str))
		== 0)
		is_work = ft_cd(list, shell, current_env);
	else if (ft_strncmp(list->str, "unset",
			ft_strlen(list->str)) == 0)
		is_work = ft_unset(list, shell, current_env);
	else if (ft_strncmp(list->str, "exit",
			ft_strlen(list->str)) == 0)
		ft_exit(input, list, current_env);
	else
		is_work = args_two(list, shell, env_list);
	if (is_work == 0)
		check_args(&list, shell, env_list, input);
}
