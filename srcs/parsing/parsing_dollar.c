/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_dollar.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajeannin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 18:29:25 by ajeannin          #+#    #+#             */
/*   Updated: 2023/07/11 22:42:06 by ajeannin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	result = malloc(sizeof(char) * n + 1);
	if (!result)
		return (NULL);
	ft_strncpy(result, str, n);
	while (current)
	{
		env = ft_strdupto_n(current->str, '=');
		if (ft_strcmp(env, str) == 0)
		{
			result = ft_strdup_from(current->str, '=');
			return (result);
		}
		current = current->next;
	}
	return (NULL);
}

/*
 * Gere le cas ou aucune variabele d'environnement n'est presente dans la chaine
 * Renvoie une copie de la chaine d'origine, jusqu'a la "fausse" $var
*/
static char	*no_env_var(char *str)
{
	char	*dollar;
	char	*result;
	int		len;

	result = NULL;
	dollar = ft_strrchr(str, '$');
	if (dollar != NULL)
	{
		len = dollar - str;
		result = malloc(sizeof(char) * len + 1);
		ft_strncpy(result, str, len);
		result[len] = '\0';
		return (result);
	}
	return (ft_strdup(str));
}

/*
 * Verifie si input contient une variable d'environnement
 * Si oui, effectue la substitution -> renvoie un pointeur vers
 * la nouvelle chaine
 * Si non, envoie un pointeur vers la chaine d'origine
*/
/*
char	*is_env_var(char *str, t_args **env_list)
{
	char	*result;
	char	*dollar;
	int		len[2];
	char	*uppercase;

	result = NULL;
	dollar = ft_strrchr(str, '$');
	if (dollar != NULL)
	{
		len[0] = dollar - str;
		len[1] = str + ft_strlen(str) - dollar;
		uppercase = dollar + 1;
		uppercase = replace_env_var(uppercase, len[1], env_list);
		if (!uppercase)
			return (no_env_var(str));
		len[1] = ft_strlen(uppercase);
		result = (char *)malloc((len[0] + len[1] + 1) * sizeof(char));
		ft_strncpy(result, str, len[0]);
		ft_strncpy(result + len[0], uppercase, len[1]);
		result[len[0] + len[1]] = '\0';
		return (result);
	}
	return (str);
}
*/

/*
 * Permet de traiter une sous-chaine, d'analyser si elle a le format
 * d'une variable d environnement, et si oui tente de la remplacer par sa
 * valeur
 * renvoie la sous-chaine sinon
 */
char	*is_env_var(char *str, t_args **env_list)
{
	char	*result;
	int		len;

	len = ft_strlen(str);
	result = NULL;
	if (str[0] == '$')
		result = replace_env_var(str + 1, len - 1, env_list);
	if (result == NULL)
		return (str);
	else
		return (result);
}
