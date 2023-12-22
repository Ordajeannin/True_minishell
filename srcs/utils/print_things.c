/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_things.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkorsako <pkorsako@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 16:28:28 by asalic            #+#    #+#             */
/*   Updated: 2023/12/21 21:18:53 by ajeannin         ###   ########.fr       */
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

/*
 * just a quick function to get a shorter path for the prompt
 * display /../../.. in the state
 * -> modify "n" in the last while condition, and in the next if
 * include first '/' or not? ft_strjoin in the return
*/
static char	*ft_getcwd2(char *src)
{
	char	*result;
	int		i;
	int		n;
	int		flag;

	result = src;
	i = -1;
	n = 0;
	flag = 0;
	if (src == NULL)
		return (NULL);
	while (src[++i])
		if (src[i] == '/')
			n++;
	while (*result && n > 2)
	{
		if (*result == '/')
			n--;
		result++;
		flag = 1;
	}
	if (n == 2 && flag == 1)
		return (ft_strjoin("/", ft_strdup(result)));
	return (ft_strdup(result));
}

char	*prompt_cmd(t_shell *shell)
{
	(void)shell;
	return (ft_strjoin(ft_strjoin("Minishell:",
				ft_getcwd2(ft_getcwd())), ">"));
}
