/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_things.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkorsako <pkorsako@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 16:28:28 by asalic            #+#    #+#             */
/*   Updated: 2023/12/05 18:45:15 by ajeannin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Permet d'afficher les details extraits pour chaque arguments, issue de input
 * Parcourt la liste chainee, et affiche :
 * 1) Le "rang" de l'argument
 * 2) Son contenu
 * 3) Comment nous l'avons interprete
 *
 * Fonction purement utilitaire, ne pas garder dans le rendu final
*/
void	print_args_list(t_args **list)
{
	t_args	*current;
	int		i;

	current = *list;
	i = 0;
	while (current != NULL)
	{
		if (current->str != NULL)
			printf("\nargument %d\nstring: %s\ntoken: %d\n\n",
				i, current->str, current->token);
		else
			printf("\nargument %d\nstring: NULL\ntoken: %d\n\n",
				i, current->token);
		i++;
		current = current->next;
	}
}

char	*prompt_cmd(t_shell *shell)
{
	char	*prompt;
	char	*pwd;

	prompt = NULL;
	pwd = NULL;
	if (shell->pwd != NULL)
		prompt = ft_strjoin(shell->pwd, ">");
	else
		prompt = ft_strjoin(pwd, ">");
	return (prompt);
}
