/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalic <asalic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 09:54:56 by asalic            #+#    #+#             */
/*   Updated: 2023/09/13 10:13:20 by asalic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* 
 * Check les erreurs de export
 * check si l'argument est NULL ou s'il existe mais qu'il est vide
 * if (list->next == NULL) : cas special
 * 			-->affiche declare -x et VE avec toutes les VE
*/
int	export_errors(t_args *list, t_args **env_list)
{
	if (!list->next)
	{
		export_out_args(env_list);
		return (1);
	}
	if (list->next->str[0] == '\0')
	{
		ft_printf("%s : invalid identifier\n", list->str);
		change_error(env_list, 1);
		return (1);
	}
	return (0);
}

/* 
 * Gere les boucles de export pour changer les VE et sinon les creer
*/
static void	ft_more_export(t_shell *shell, char *v_env, char *value)
{
	shell_change(shell, v_env, value);
	if (ft_strncmp(v_env, "PWD", ft_strlen(v_env)) == 0)
		shell->is_pwd = value;
	else if (ft_strncmp(v_env, "OLDPWD", ft_strlen(v_env)) == 0)
		shell->is_oldpwd = value;
}

/* Fonction export.
 * Cherche d'abord si la VE existe deja.
 * Si oui, la modifie, dans env_list et dans shell.
 * Si non, la creee dans env_list seulement.
 * Gere le cas ou il y a plusieurs creation/remplacement de VE

*/
int	ft_export(t_args *list, t_shell *shell, t_args **env_list)
{
	char	*value;
	char	*v_env;

	if (export_errors(list, env_list) == 1)
		return (1);
	v_env = ft_strdupto_n(list->next->str, '=');
	value = ft_strdup_from(list->next->str, '=');
	if (!(ft_strchr(list->next->str, '=')) || is_only_equal(value, '=') == 1
		&& (is_only_equal(v_env, ' ') == 1 || v_env == NULL))
		return (1);
	if (change_env_exp(env_list, v_env, value) == 1)
		ft_more_export(shell, v_env, value);
	else
	{
		add_env(env_list, list);
		ft_more_export(shell, v_env, value);
	}
	if (list->next->next != NULL)
		ft_export(list->next, shell, env_list);
	change_error(env_list, 0);
	return (0);
}
