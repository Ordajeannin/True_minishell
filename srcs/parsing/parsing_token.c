/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_token.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkorsako <pkorsako@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 18:29:25 by ajeannin          #+#    #+#             */
/*   Updated: 2023/12/20 06:26:36 by ajeannin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Permet de supprimer les arguments qui se voulaient etre des
 * variables d'environnement, mais mal formatee
*/
void	delete_null_nodes(t_args **list)
{
	t_args	*current;
	t_args	*prev;

	if (*list == NULL)
		return ;
	current = *list;
	prev = NULL;
	while (current != NULL)
	{
		if (current->str == NULL && current->token != 23)
		{
			if (prev == NULL)
				*list = current->next;
			else
				prev->next = current->next;
			current = current->next;
		}
		else
		{
			prev = current;
			current = current->next;
		}
	}
}

/*
 * Analyse une chaine de caractere issue d'input
 * Pour y associer un token
 * :warning: CMD, OPTION, FPATH et RPATH ne sont pas encore traites (cf 12910)
*/
int	tokenize_args(char *input, int flag)
{
	if (flag == 23)
		return (flag);
	else if (input[0] == '&' && input[1] == '&' && input[2] == '\0')
		return (TOKEN_AND);
	else if (input[0] == '|' && input[1] == '|' && input[2] == '\0')
		return (TOKEN_OR);
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
 * Remplace les $var APRES la gestion des heredocs
*/
int	update_args2(t_args **list, t_args **env_list)
{
	t_args	*current;

	delete_null_nodes(list);
	current = *list;
	while (current != NULL)
	{
		if (current->token != TOKEN_S_QUOTES)
		{
			if (process_not_s_quotes(current, env_list) == 1)
				return (1);
		}
		current = current->next;
	}
	return (0);
}

int	update_args(t_args **list)
{
	t_args	*current;
	char	help[2];

	current = *list;
	help[0] = '\0';
	help[1] = '\0';
	while (current != NULL)
	{
		if (current->str != NULL && current->token < 20)
			current->token = tokenize_args(current->str, 0);
		if (current->str != NULL && current->token == 23)
			current->token = tokenize_args(current->str, 23);
		if (current->str == NULL && current->token == 23)
		{
			current->str = ft_strdup(help);
			if (!current->str)
				return (1);
		}
		current = current->next;
	}
	return (0);
}
