/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_dollar.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkorsako <pkorsako@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 18:29:25 by ajeannin          #+#    #+#             */
/*   Updated: 2023/12/18 18:50:52 by ajeannin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*help_replace_var(char **result, t_args *current)
{
	*result = ft_strdup_from(current->str, '=');
	//protection?
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
//		printf("set_couille :%s\n", ft_itoa(set_error_nb(0, NO)));
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
