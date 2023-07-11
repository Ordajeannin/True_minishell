/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalic <asalic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 13:30:00 by asalic            #+#    #+#             */
/*   Updated: 2023/07/10 18:17:11 by asalic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Recupere str jusqu'a c.
 * Creation d'un tableau a partir de str jusqu'a atteindre le caractere c.
 * Compter d'abord le nombre de caractere a ajouter a tab.
 * Puis, ajout a tab.
*/
char	*ft_strdupto_n(char *str, char c)
{
	char	*tab;
	int		i;
	int		len;

	i = 0;
	len = 0;
	while (str[i] && str[i] != c)
	{
		len ++;
		i ++;
	}
	tab = malloc((len +1) * sizeof(char));
	if (!tab)
		return (NULL);
	i = 0;
	while (str[i] && str[i] != c)
	{
		tab[i] = str[i];
		i ++;
	}
	tab[i] = '\0';
	return (tab);
}

/* 
 * Recupere str a partir de c.
*/
char	*ft_strdup_from(char *str, char c)
{
	char	*tab;
	int		i;
	int		start;
	int		len;

	i = 0;
	len = 0;
	while (str[i] && str[i] != c)
		i ++;
	if (str[i] == c)
		i ++;
	start = i;
	while (str[i++])
		len ++;
	tab = malloc((len +1) * sizeof(char));
	if (!tab)
		return (NULL);
	i = start;
	len = 0;
	while (str[i])
		tab[len++] = str[i++];
	tab[len] = '\0';
	return (tab);
}
