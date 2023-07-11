/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalic <asalic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/11 12:12:28 by asalic            #+#    #+#             */
/*   Updated: 2023/07/11 12:24:23 by asalic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Ajout nouveau maillon dans env.
 * Boucle jusqu'a la fin de la liste.
 * Puis ajoute la string.
 * (BUG: sauf qu'il l'ajoute de maniere random ou il veut dans l'env ;-;)
*/
static void	add_env(t_args **env_list, t_args *list)
{
	t_args	*new_var;
	t_args	*current;

	new_var = malloc(sizeof(t_args));
	new_var->str = ft_strdup(list->next->str);
	new_var->token = 0;
	new_var->next = NULL;
	if (*env_list)
	{
		current = *env_list;
		while (current->next != NULL)
			current = current->next;
		current->next = new_var;
	}
	else
		*env_list = new_var;
}

/* Fonction export.
 * Cherche d'abord si la VE existe deja.
 * Si oui, la modifie, dans env_list et dans shell.
 * Si non, la creee dans env_list seulement.
*/
void	ft_export(t_args *list, t_shell *shell, t_args **env_list)
{
	char	*value;
	char	*v_env;

	v_env = ft_strdupto_n(list->next->str, '=');
	value = ft_strdup_from(list->next->str, '=');
	ft_printf("NAME: %s\nVALUE: %s\n", v_env, value);
	if (change_env_exp(env_list, v_env, value) == 1)
		shell_change(shell, v_env, value);
	else
		add_env(env_list, list);
}
