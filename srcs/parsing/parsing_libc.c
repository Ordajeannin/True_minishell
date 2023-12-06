/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_libc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalic <asalic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 15:19:59 by ajeannin          #+#    #+#             */
/*   Updated: 2023/12/06 17:05:21 by ajeannin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Compare 2 string
*/
int	ft_strcmp(const char *s1, const char *s2)
{
	if (!s1 || !s2)
		return (1);
	while (*s1 && (*s1 == *s2))
	{
		s1++;
		s2++;
	}
	return (*(unsigned char *)s1 - *(unsigned char *)s2);
}

/*
 * Recherche la premiere occurence d'un caractere c
 * Renvoie un pointeur vers le caractere si rencontre, NULL sinon
*/
char	*ft_strchr(const char *s, int c)
{
	while (*s && *s != (char)c)
		s++;
	if (*s == (char)c)
		return ((char *)s);
	else
		return (NULL);
}

/*
 * Recherche la premiere occurence d'un caractere parmis une liste
 * Renvoie un pointeur vers le premier caractere rencontre, NULL sinon
*/
char	*ft_strpbrk(const char *s, const char *charset)
{
	while (*s)
	{
		if (ft_strchr(charset, *s) != NULL)
			return ((char *)s);
		s++;
	}
	return (NULL);
}
