/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_quotes.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajeannin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 15:19:59 by ajeannin          #+#    #+#             */
/*   Updated: 2023/09/27 17:26:24 by asalic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Permet de supprimer l'ensemble des arguments + afficher un message d'erreur
 * si des quotes ont ete ouvert, mais non fermee
 */
void	was_unclosed_quotes(t_args **list)
{
	t_args	*current;
	int		flag;

	current = *list;
	flag = 0;
	while (current != NULL)
	{
		if (current->token == 42)
			flag = 1;
		current = current->next;
	}
	if (flag == 1)
	{
		clear_args_list(list);
		ft_putstr_fd("ERROR : quotes wasn't closed\n", STDOUT_FILENO);
	}
}

/*
 * Permet de... gagner de la place
*/
static int	iterate_input(const char **input, char quote)
{
	int	i;

	i = 0;
	while (**input != '\0' && **input != quote)
	{
		(*input)++;
		i++;
	}
	return (i);
}

/*
 * Permet de gerer les simples et doubles quotes rencontrees dans input
 * Parcours la chaine jusqu'a un quote equivalent a celui ayant appele cette
 * fonction.
 * Si trouve : retourne la chaine allouee
 * Sinon : quotes non fermees, erreur a gerer plus loin dans le code 
 * (cf fonction "was_unclosed_quotes")
*/
size_t	is_quotes(char *str, t_args **list, const char *input, int flag)
{
	char	quote;
	char	*result;
	int		i;

	i = 0;
	result = malloc(sizeof(char) * ft_strlen(input) + 1);
	if (!result)
		return (0);
	if (*str == '\'')
		quote = *str;
	else if (*str == '\"')
		quote = *str;
	input++;
	i = iterate_input(&input, quote);
	ft_strncpy(result, input - i, i);
	result[i] = '\0';
	if (*input == quote)
		add_arg(list, result, flag + 21);
	else
	{
		add_arg(list, "FAIL", ABORT_MISSION);
		return (0);
	}
	return (ft_strlen(result) + 1);
}

/*
 * A conserver temporairement si le raccourcissement pause probleme
size_t	is_quotes(char *str, t_args **list, const char *input, int flag)
{
	char	quote;
	char	*result;
	int		i;

	i = 0;
	result = malloc(sizeof(char) * ft_strlen(input) + 1);
	if (*str == '\'')
		quote = *str;
	else if (*str == '\"')
		quote = *str;
	input++;
	while (*input != '\0' && *input != quote)
	{
		result[i] = *input;
		i++;
		input++;
	}
	result[i] = '\0';
	if (*input == quote)
		add_arg(list, result, flag + 21);
	else
	{
		add_arg(list, "FAIL", ABORT_MISSION);
		return (0);
	}
	return (ft_strlen(result) + 1);
}
*/
