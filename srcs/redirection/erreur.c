/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   erreur.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajeannin <ajeannin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 19:58:53 by ajeannin          #+#    #+#             */
/*   Updated: 2023/12/21 17:20:33 by ajeannin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Verifie si l'argument suivant une redirection est au bon format
 * (premier caractere non-alphanum + absence de pipe en debut d'input)
*/
int	is_correct_format(t_args **list)
{
	t_args	*current;

	current = *list;
	if (current != NULL && current->token == 4)
	{
		perror("syntax error near unexpected token\n");
		return (-1);
	}
	while (current != NULL)
	{
		if (current->token == 4 || current->token == 5 || current->token == 6
			|| current->token == 7 || current->token == 8)
		{
			if (current->next == NULL
				|| (current->next != NULL
					&& is_alphanum(current->next->str[0]) != 0))
			{
				perror("syntax error near unexpected token\n");
				return (-1);
			}
		}
		current = current->next;
	}
	return (0);
}
