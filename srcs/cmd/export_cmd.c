/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkorsako <pkorsako@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 09:54:56 by asalic            #+#    #+#             */
/*   Updated: 2023/12/22 20:54:35 by ajeannin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Check les erreurs de export
 * check si l'argument est NULL ou s'il existe mais qu'il est vide
 * ATTENTION: voir cas speciaux et faire mini parsing des args avant d'export.
*/
int	export_errors(t_args *list, t_args **env_list)
{
	if (ft_strlen(list->str) == 6 && ft_strcmp(list->str, "export") == 0
		&& !list->next)
	{
		export_out_args(env_list);
		return (1);
	}
	if (list->next && list->next->str[0] == '\0')
	{
		ft_printf("export : \"\": invalid identifier\n");
		set_error_nb(1, YES);
		return (1);
	}
	if (parse_export(list->next) == 2)
		return (0);
	else if (parse_export(list->next) == 1)
	{
		ft_printf("export : \"%s\" : invalid identifier\n", list->next->str);
		set_error_nb(1, YES);
		return (1);
	}
	return (0);
}

int	dont_segfault_please(t_args *env_list)
{
	export_out_args(&env_list);
	return (0);
}

/* Fonction export.
 * Cherche d'abord si la VE existe deja.
 * Si oui, la modifie, dans env_list et dans shell.
 * Si non, la creee dans env_list seulement.
 * Gere le cas ou il y a plusieurs creation/remplacement de VE
 * le deuxieme if, je cite : "Je ne sais pas a quoi ca sert!!"
*/
int	ft_export(t_args *list, t_shell *shell, t_args *env_list)
{
	t_args	*new_variable;

	if (!list->next)
		return (dont_segfault_please(env_list));
	if (export_errors(list, &env_list) == 1)
	{
		if (list->next->next != NULL)
		{
			ft_export(list->next, shell, env_list);
		}
		return (1);
	}
	if (ft_strchr(list->next->str, '='))
	{
		new_variable = find_a(ft_strdupto_n(list->next->str, '='), env_list);
		if (new_variable)
			new_variable->str = list->next->str;
		else
			add_arg(&env_list, list->next->str, 0);
	}
	if (list->next->next != NULL)
		ft_export(list->next, shell, env_list);
	set_error_nb(0, YES);
	return (0);
}

/*
 * Gere export sans args
 * Affiche: declare -x VE env
*/
int	export_out_args(t_args **env_list)
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
	if (!ft_isalpha(list->str[0]) && list->str[0] != '_')
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
