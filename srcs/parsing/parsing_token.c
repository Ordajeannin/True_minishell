/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_token.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajeannin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 18:29:25 by ajeannin          #+#    #+#             */
/*   Updated: 2023/07/07 18:42:06 by ajeannin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Analyse une chaine de caractere issue d'input
 * Pour y associer un token
 * :warning: CMD, OPTION, FPATH et RPATH ne sont pas encore traites (cf 12910)
*/
int	tokenize_args(char *input)
{
	if (input[0] == '&' && input[1] == '&' && input[2] == '\0')
		return (TOKEN_AND);
	else if (input[0] == '|' && input[1] == '\0')
		return (TOKEN_PIPE);
	else if (input[0] == '<' && input[1] == '\0')
		return (TOKEN_INFILE);
	else if (input[0] == '<' && input[1] == '<' && input[2] == '\0')
		return (TOKEN_DELIM);
	else if (input[0] == '>' && input[1] == '\0')
		return (TOKEN_OUTFILE);
	else if (input[0] == '>' && input[1] == '>' && input[2] == '\0')
		return (TOKEN_APPEND);
	else
		return (12910);
}

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
	strncpy(result, str, n);
	while (current)
	{
		env = ft_strdupto_n(current->str, '=');
		if (ft_strcmp(env, result) == 0)
		{
			result = ft_strdup_from(current->str, '=');
			return (result);
		}
		current = current->next;
	}
	return (NULL);
}

static char	*no_env_var(char *str)
{
	char	*dollar;
	char	*result;
	int		len;

	result = NULL;
	dollar = strrchr(str, '$');
	if (dollar != NULL)
	{
		len = dollar - str;
		result = malloc(sizeof(char) * len + 1);
		strncpy(result, str, len);
		result[len] = '\0';
		return (result);
	}
	return (strdup(str));
}

char	*is_env_var(char *str, t_args **env_list)
{
	char	*result;
	char	*dollar;
	int		len[2];
	char	*uppercase;

	result = NULL;
	dollar = strrchr(str, '$');
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
		strncpy(result, str, len[0]);
		strncpy(result + len[0], uppercase, len[1]);
		result[len[0] + len[1]] = '\0';
		return (result);
	}
	return (str);
}

/*
 * Permet d'associer un token a une string pour chaque maillon
 * de la liste chainee input
*/
void	update_args(t_args **list, t_args **env_list)
{
	t_args	*current;
	char	*tmp;

	current = *list;
	tmp = NULL;
	while (current != NULL)
	{
		while (tmp != current->str)
		{
			printf("%s\n", current->str);
			tmp = current->str;
			current->str = is_env_var(current->str, env_list);
		}
		printf("%s\n", current->str);
	//	current->str = is_env_var(current->str, env_list);
		current->token = tokenize_args(current->str);
		current = current->next;
	}
}
