/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkorsako <pkorsako@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 09:54:56 by asalic            #+#    #+#             */
/*   Updated: 2023/12/11 18:49:26 by pkorsako         ###   ########.fr       */
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
	if (ft_strlen(list->str) == 6 && ft_strcmp(list->str, "export") == 0
		&& !list->next)
	{
		ft_printf("que pasa\n");
		export_out_args(env_list, shell);
		return (1);
	}
	if (list->next->str[0] == '\0')
	{
		ft_printf("export : \"\": invalid identifier\n");
		set_error_nb(1, YES);
		// change_error(env_list, shell, 1);
		return (1);
	}
	if (parse_export(list->next) == 2)
		return (0);
	else if (parse_export(list->next) == 1)
	{
		ft_printf("export : \"%s\" : invalid identifier\n", list->next->str);
		set_error_nb(1, YES);
		// change_error(env_list, shell, 1);
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
		shell->is_pwd = ft_strdup(value);
	else if (ft_strncmp(v_env, "OLDPWD", ft_strlen(v_env)) == 0)
		shell->is_oldpwd = ft_strdup(value);
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
	{
		if (list->next != NULL)
			ft_export(list->next, shell, env_list);
		return (1);
	}
	if (ft_strchr(list->next->str, '='))
	{
		v_env = ft_strdupto_n(list->next->str, '=');
		if (!v_env)
			return (1);
		value = ft_strdup_from(list->next->str, '=');
		if (!value)
			return (1);
		result_change_env = change_env_exp(env_list, v_env, value);
		if (result_change_env == 0)
			ft_more_export(shell, v_env, value);
		else if (result_change_env == 1)
		{
			add_env(env_list, list->next->str);
			ft_more_export(shell, v_env, value);
		}
		else
			return (1);
	}
	if (list->next->next != NULL)
		ft_export(list->next, shell, env_list);
	set_error_nb(0, YES);
	// if (!change_error(env_list, shell, 0))
	// 	return (1);
	return (0);
}

/*
 * Gere export sans args
 * Affiche: declare -x VE env
*/
int	export_out_args(t_args **env_list, t_shell *shell)
{
	char	*bfore;
	char	*after;
	char	**env_sort;
	int		i;

	bfore = NULL;
	after = NULL;
	env_sort = ft_sort(env_list);
	i = 0;
	while (env_sort[i])
	{
		bfore = ft_strdupto_n(env_sort[i], '=');
		after = ft_strdup_from(env_sort[i], '=');
		if (ft_strncmp(bfore, "?=", 2) == 0)
			i ++;
		else
			ft_printf("declare -x %s=\"%s\"\n", bfore, after);
		i ++;
	}
	i = 0;
	set_error_nb(0, YES);
	return (0);
	///////awena//////////
	if (!change_error(env_list, shell, 0))
		return (1);
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
