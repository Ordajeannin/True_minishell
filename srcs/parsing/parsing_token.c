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
 * Fonction permettant de rajouter un espace a la fin d'un argument
 * (update en verifiant le token?)
static char	*add_space(char	*str)
{
	char	*result;
	int		len;

	len = ft_strlen(str);
	result = malloc(sizeof(char) * len + 1);
	if (!result)
		return (NULL);
	ft_strncpy(result, str, len);
	result[len] = ' ';
	result[len + 1] = '\0';
	return (result);
}
*/

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

/*
 * Permet de verifier si un argument contient une variable d'environnement
 * correctement formatee, et de la remplacer
 * Permet egalement d'associer un token a une string pour chaque maillon
 * de la liste chainee input
*/
void	update_args(t_args **list, t_args **env_list)
{
	t_args	*current;
	char	**tmp;
	int		i;

	current = *list;
	tmp = NULL;
	while (current != NULL)
	{
		i = 0;
		tmp = ft_split_arg(current->str);
		current->str = NULL;
		while (tmp[i])
		{
			tmp[i] = is_env_var(tmp[i], env_list);
			if (tmp[i] != NULL && current->str != NULL)
				current->str = ft_strjoin(current->str, tmp[i]);
			if (tmp[i] != NULL && current->str == NULL)
				current->str = tmp[i];
			i++;
		}
		current->token = tokenize_args(current->str);
		current = current->next;
	}
}
