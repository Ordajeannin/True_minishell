/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalic <asalic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 19:59:47 by ajeannin          #+#    #+#             */
/*   Updated: 2023/09/26 17:00:43 by asalic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Supprime le maillon actuel + le suivant
 * si l'argument est une redirection
*/
static t_args	*j_ai_peter_les_plombs(t_args **list)
{
	t_args	*current;

	current = *list;
	*list = (*list)->next;
	free(current);
	if (*list == NULL)
		return (NULL);
	current = *list;
	*list = (*list)->next;
	free(current);
	if (*list != NULL)
		return (*list);
	else
		return (NULL);
}

/*
 * Juste pour verifier si un argument est une redirection
*/
static int	yes_there_is_one(int token)
{
	if (token == TOKEN_INFILE)
		return (token);
	if (token == TOKEN_DELIM)
		return (token);
	if (token == TOKEN_OUTFILE)
		return (token);
	if (token == TOKEN_APPEND)
		return (token);
	return (0);
}

/*
 * Fonction principale de la redirection
 * Va parcourir la liste des arguments, et appeler la gestion des redirections
 * si besoin.
 * ensuite, si fonction appelee, supprime le maillon actuel (redirection) et 
 * le suivant (nom du fichier)
 * incompatible pour le moment avec deux fois la meme redirection des deux cotes
 * d'un && ou d'un ||
 * -> condition dans le while? un && current->next->token != TOKEN_AND
 * (+ != TOKEN_OR)
*/
void	is_there_a_redirection(t_args **list)
{
	t_args	*current;
	t_args	*prev;

	current = *list;
	prev = NULL;
	while (current != NULL && current->next != NULL)
	{
		if (yes_there_is_one(current->token) != 0)
		{
			c_est_ma_direction(current->token, &current);
			current = j_ai_peter_les_plombs(&current);
		}
		else
		{
			prev = current;
			current = current->next;
		}
		if (prev == NULL)
			*list = current;
		else
			prev->next = current;
	}
}
