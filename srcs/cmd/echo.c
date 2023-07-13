/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalic <asalic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 12:37:52 by asalic            #+#    #+#             */
/*   Updated: 2023/07/13 17:18:09 by asalic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* 
 * Cherche s2 dans s1.
 * Parcours s1 jusqu'a trouver s2 a la suite.
*/
int	find_opt(char *s1, char *s2)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (s1[i])
	{
		if (s1[i] == s2[j])
		{
			j ++;
			if (j == ft_strlen(s2))
				return (1);
		}
		i ++;
	}
	return (0);
}

/* 
 * Affiche caractere par caractere la liste.
 * Suite de ft_echo.
*/
void	iter_echo(t_args *list)
{
	int	i;

	i = 0;
	while (list->str[i])
	{
		if (list->str[i] == 34 || list->str[i] == 39)
			i ++;
		write (1, &list->str[i], 1);
		i ++;
	}
}

/*
 * Fonction a l'image de 'echo'.
 * Affiche caractere par caractere les arguments en ignorant les quotes.
 * (Attention: cas particuliers, quotes a l'interieur de d'autres).
*/
void	ft_echo(t_args *list, t_shell *shell)
{
	int	bools;

	bools = 0;
	list = list->next;
	if (list == NULL)
		return ;
	if (find_opt(list->str, "-n"))
	{
		ft_printf("HOLA\n");
		list = list->next;
		bools = 1;
	}
	while (list && list->token != 3)
	{
		iter_echo(list);
		list = list->next;
	}
	if (bools == 0)
		write (1, "\n", 1);
	shell->error = 0;
}
