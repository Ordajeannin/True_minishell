/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_input.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajeannin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 15:19:59 by ajeannin          #+#    #+#             */
/*   Updated: 2023/07/07 18:31:07 by ajeannin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * :WARNING:
 * Cet ensemble de fonctions ne gere pas encore les "str" et 'str'
*/
size_t	delimit_to_token(char *str, t_args **list, const char **input)
{
	if (ft_strcmp(str, "\'") == 0)
		return (is_quotes(str, list, *input, 1));
	else if (ft_strcmp(str, "\"") == 0)
		return (is_quotes(str, list, *input, 2));
	else if (ft_strcmp(str, " ") != 0)
		add_arg(list, str, 0);
	return (0);
}

/*
 * reduit la taille de ft_strtok
*/
static void	help_strtok(char *token_end, char **last_token)
{
	if (*(token_end - 1) == '<' || *(token_end - 1) == '>')
		*(token_end - 1) = '\0';
	*token_end = '\0';
	*last_token = token_end + 1;
}

/*
 * Permet de decouper input en une serie de tokens
 * en utilisant les delimitateurs specifies
 * Chaque appel de la fonction retourne un token de la chaine jusqu a
 * ce que tous soient extraits
 * Si tous les tokens ont ete extraits, retourne NULL
 * Sinon, renvoie un pointeur vers le debut du token extrait
*/
char	*ft_strtok(char *input, char **delim, t_args **list)
{
	static char	*last_token = NULL;
	char		*token_end;

	if (input != NULL)
		last_token = input;
	else
	{
		if (last_token == NULL)
			return (NULL);
		input = last_token;
	}
	input += ft_strspn(input, delim, list);
	if (*input == '\0')
	{
		last_token = NULL;
		return (NULL);
	}
	add_arg(list, input, 0);
	token_end = input + ft_strcspn(input, delim, list);
	if (*token_end != '\0')
		help_strtok(token_end, &last_token);
	else
		last_token = NULL;
	return (input);
}
