/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_things.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkorsako <pkorsako@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 16:28:28 by asalic            #+#    #+#             */
/*   Updated: 2023/12/07 19:41:28 by pkorsako         ###   ########.fr       */
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
	(void)shell;
	
	return (ft_strjoin(ft_strjoin("minishell :", ft_getcwd()), ":>"));
}

// static void	print_member(const char *name, const void *member, int type)
// {
// 	if (type == 's')
// 		printf("This is the %s : %s\n", name, (char *)member);
// 	else if (type == 'i')
// 		printf("This is the %s : %i \n", name, *(int *)member);
// 	else if (type == 'a')
// 		printf("This is the %s : %p \n", name, member);
// }

// void print_shell(t_shell *shell)
// {
// 	print_member("home", shell->home, 's');
// 	print_member("path", shell->path, 's');
// 	print_member("cmd_paths", shell->cmd_paths, 'a');
// }
