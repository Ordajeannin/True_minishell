/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalic <asalic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 13:30:00 by asalic            #+#    #+#             */
/*   Updated: 2023/07/04 14:04:58 by asalic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 *Recupere str jusqu'a c.
 *Creation d'un tableau a partir de str jusqu'a atteindre le caractere c.
 *Compter d'abord le nombre de caractere a ajouter a tab.
 *Puis, ajout a tab.
*/
char	*until_char(char *str, int c)
{
	int		i;
	int		j;
	char	*tab;

	while (str[i] != c)
		i ++;
	if (!str[i])
		return (NULL);
	tab = malloc((i + 1) * sizeof(char));
	if (!tab)
		return (NULL);
	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] == c)
			break ;
		tab[j++] = str[i++];
	}
	tab[j] = '\0';
	return (tab);
}
