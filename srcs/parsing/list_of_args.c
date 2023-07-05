/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_of_args.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajeannin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 17:55:04 by ajeannin          #+#    #+#             */
/*   Updated: 2023/07/04 19:49:42 by ajeannin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Fonction utilitaire, libere chaque maillon de la chaine d'arguments
 * conserve *liste et le reset a NULL
*/
void	clear_args_list(t_args **list)
{
	t_args	*current;
	t_args	*next;

	current = *list;
	while (current != NULL)
	{
		next = current->next;
		free(current);
		current = next;
	}
	*list = NULL;
}

/*
 * Fonction utilitaire, cree un maillon 
 * a partir du contenu de l'argument et du token attribue
*/
static t_args	*create_arg(char *str, int token)
{
	t_args	*new_arg;

	new_arg = (t_args *)malloc(sizeof(t_args));
	if (!new_arg)
		return (NULL);
	new_arg->str = str;
	new_arg->token = token;
	new_arg->next = NULL;
	return (new_arg);
}

/*
 * Fonction utilitaire, ajoute un maillon a la fin de la chaine
 * appel a create_arg pour creer le maillon
*/
static void	add_arg(t_args **list, char *str, int token)
{
	t_args	*new_arg;
	t_args	*current;

	new_arg = create_arg(str, token);
	if (!new_arg)
		return ;
	if (*list == NULL)
		*list = new_arg;
	else
	{
		current = *list;
		while (current->next != NULL)
			current = current->next;
		current->next = new_arg;
	}
}

/*
 * Analyse une chaine de caractere issue d'input
 * puis creer un maillon pour l'argument, avec contenu et token
 * :warning: CMD, OPTION, FPATH et RPATH ne sont pas encore traites (cf 12910)
*/
void	from_input_to_list_of_args(char **input, t_args **list)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (input[i][0] == '&' && input[i][1] == '&' && input[i][2] == '\0')
			add_arg(list, input[i], TOKEN_AND);
		else if (input[i][0] == '|' && input[i][1] == '\0')
			add_arg(list, input[i], TOKEN_PIPE);
		else if (input[i][0] == '<' && input[i][1] == '\0')
			add_arg(list, input[i], TOKEN_INFILE);
		else if (input[i][0] == '<' && input[i][1] == '<'
				&& input[i][2] == '\0')
			add_arg(list, input[i], TOKEN_DELIM);
		else if (input[i][0] == '>' && input[i][1] == '\0')
			add_arg(list, input[i], TOKEN_OUTFILE);
		else if (input[i][0] == '>' && input[i][1] == '>'
				&& input[i][2] == '\0')
			add_arg(list, input[i], TOKEN_APPEND);
		else
			add_arg(list, input[i], 12910);
		i++;
	}
}
