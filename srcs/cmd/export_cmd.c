/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalic <asalic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 09:54:56 by asalic            #+#    #+#             */
/*   Updated: 2023/10/02 11:12:54 by asalic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* 
 * Check les erreurs de export
 * check si l'argument est NULL ou s'il existe mais qu'il est vide
 * ATTENTION: voir cas speciaux et faire mini parsing des args avant d'export.
*/
int	export_errors(t_args *list, t_args **env_list, t_shell *shell)
{
	if (!list->next)
	{
		export_out_args(env_list, shell);
		return (1);
	}
	if (list->next->str[0] == '\0')
	{
		ft_printf("export : \"\": invalid identifier\n");
		change_error(env_list, shell, 1);
		return (1);
	}
	if (parse_export(list->next) == 2)
		return (0);
	else if (parse_export(list->next) == 1)
	{
		ft_printf("export : \"%s\" : invalid identifier\n", list->next->str);
		change_error(env_list, shell, 1);
		return (1);
	}
	return (0);
}

/* 
 * Gere les boucles de export pour changer les VE et sinon les creer
*/
static void	ft_more_export(t_shell *shell, char *v_env, char *value)
{
	if (ft_strcmp(v_env, "SHLVL") == 0 && ft_strlen(v_env) == 5
		&& ft_atoi(value) < 0)
		value = "0";
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
	int		result_change_env;

	if (export_errors(list, env_list, shell) == 1)
		return (1);
	if (ft_strchr(list->next->str, '='))
	{
		v_env = ft_strdupto_n(list->next->str, '=');
		if (! v_env)
			return (1);
		value = ft_strdup_from(list->next->str, '=');
		if (! value)
		{
			free(v_env);
			return (1);
		}
		result_change_env = change_env_exp(env_list, v_env, value);
		if (result_change_env == 0)
			ft_more_export(shell, v_env, value);
		else if (result_change_env == 1)
		{
			add_env(env_list, list->next->str);
			ft_more_export(shell, v_env, value);
		}
		else
		{
			free(v_env);
			free(value);
			return (1);
		}
	}
	free(v_env);
	free(value);
	if (list->next->next != NULL)
		ft_export(list->next, shell, env_list);
	if (!change_error(env_list, shell, 0))
		return (1);
	return (0);
}

/*
 * Gere export sans args
 * Affiche: declare -x VE env
*/
int	export_out_args(t_args **env_list, t_shell *shell)
{
	t_args	*current;
	char	*bfore;
	char	*after;

	current = *env_list;
	bfore = NULL;
	after = NULL;
	while (current != NULL)
	{
		bfore = ft_strdupto_n(current->str, '=');
		after = ft_strdup_from(current->str, '=');
		if (ft_strncmp(current->str, "?=", 2) == 0)
			current = current->next;
		else
		{
			ft_printf("declare -x %s=\"%s\"\n", bfore, after);
			current = current->next;
		}
		free(bfore);
		free(after);
	}
	if (!change_error(env_list, shell, 0))
		return (1);
	return (0);
}

/* 
 * Mini parsing des args d'export
 * Check si le nom est conforme, juste des chiffres, des lettres et des
 * underscores
*/
int	parse_export(t_args *list)
{
	int	i;

	i = 0;
	if (!(list->str[0] >= 'A' && list->str[0] <= 'Z') && !(list->str[0] >= 'a'
			&& list->str[0] <= 'z'))
		return (1);
	while (list->str[i] && list->str[i] != '=')
	{
		if (!(list->str[i] >= '0' && list->str[i] <= '9') && !(list->str[i] \
			>= 'A' && list->str[i] <= 'Z') && !(list->str[i] >= 'a' \
			&& list->str[i] <= 'z') && list->str[i] != '_')
			return (1);
		i ++;
	}
	if (list->str[i] == '=')
		return (0);
	return (2);
}
