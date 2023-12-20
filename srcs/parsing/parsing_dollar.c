/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_dollar.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkorsako <pkorsako@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 18:29:25 by ajeannin          #+#    #+#             */
/*   Updated: 2023/12/20 06:30:36 by ajeannin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//protection?
static char	*help_replace_var(char **result, t_args *current)
{
	*result = ft_strdup_from(current->str, '=');
	return (*result);
}

/*
 * Remplace une variable d'environnement par sa valeur correspondante
 * retourne un pointeur vers la nouvelle chaine si la variable existe,
 * NULL sinon
*/
static char	*replace_env_var(char *str, int n, t_args **env_list)
{
	char	*result;
	char	*env;
	t_args	*current;

	current = *env_list;
	env = NULL;
	result = ft_calloc(n + 1, sizeof(char));
	if (!result)
		return (NULL);
	ft_strncpy(result, str, n);
	while (current)
	{
		env = ft_strdupto_n(current->str, '=');
		if (ft_strcmp(env, str) == 0)
			return (help_replace_var(&result, current));
		current = current->next;
	}
	return (NULL);
}

/*
 * Permet de traiter une sous-chaine, d'analyser si elle a le format
 * d'une variable d environnement, et si oui tente de la remplacer par sa
 * valeur
 * renvoie la sous-chaine sinon, ou si $?
 */
char	*is_env_var(char *str, t_args **env_list)
{
	char	*result;
	int		len;

	len = ft_strlen(str);
	result = NULL;
	if (str[0] == '$' && str[1] == '?')
	{
		result = ft_itoa(set_error_nb(0, NO));
		return (result);
	}
	if (str[0] == '$' && is_alphanum(str[1]) == 0)
	{
		result = replace_env_var(str + 1, len - 1, env_list);
		return (result);
	}
	return (str);
}

static int	process_tmp_node(t_args *node, char *tmp)
{
	char	*tmp_node;

	tmp_node = NULL;
	if (tmp != NULL && node->str != NULL)
	{
		tmp_node = ft_strdup(node->str);
		if (!tmp_node)
			return (1);
		node->str = ft_strjoin(tmp_node, tmp);
		if (!node->str)
			return (1);
	}
	else if (tmp != NULL && node->str == NULL)
	{
		node->str = ft_strdup(tmp);
		if (!node->str)
			return (1);
	}
	return (0);
}

int	process_not_s_quotes(t_args *node, t_args **env_list)
{
	char	**tmp;
	int		i;

	tmp = ft_split_arg(node->str);
	if (!tmp)
		return (1);
	node->str = NULL;
	i = 0;
	while (tmp[i])
	{
		tmp[i] = is_env_var(tmp[i], env_list);
		if (process_tmp_node(node, tmp[i]) == 1)
			return (1);
		i++;
	}
	return (0);
}

/*
 * Permet de gerer le remplacement des variables d'environnement,
 * si correctement formate
 * Il faut proteger !!!
*/
/*
int	process_not_s_quotes(t_args *node, t_args **env_list)
{
	char	**tmp;
	int		i;
	char	*tmp_node;

	tmp = ft_split_arg(node->str);
	if (!tmp)
		return (1);
	node->str = NULL;
	tmp_node = NULL;
	i = 0;
	while (tmp[i])
	{
		tmp[i] = is_env_var(tmp[i], env_list);
		if (tmp[i] != NULL && node->str != NULL)
		{
			tmp_node = ft_strdup(node->str);
			node->str = ft_strjoin(tmp_node, tmp[i]);
			if (!node->str)
				return (1);
		}
		else if (tmp[i] != NULL && node->str == NULL)
		{
			node->str = ft_strdup(tmp[i]);
			if (!node->str)
				return (1);
		}
		i++;
	}
	i = 0;
	return (0);
}
*/
