/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_libc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalic <asalic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 15:19:59 by ajeannin          #+#    #+#             */
/*   Updated: 2023/09/28 11:29:40 by asalic           ###   ########.fr       */
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
	// ft_printf("diff = %d\n", *(unsigned char *)s1 - *(unsigned char *)s2);
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

/*
 * Permet de calculer la partie de input composee uniquement de delimitateurs
*/
/*
size_t	ft_strspn(const char *input, char **accept, t_args **list)
{
	char		**a;
	size_t		count;
	int			accepted;

	count = 0;
	while (*input)
	{
		a = accept;
		accepted = 0;
		while (*a != NULL)
		{
			if (ft_strncmp(input, *a, ft_strlen(*a)) == 0)
			{
				delimit_to_token(*a, list);
				accepted = 1;
				if (ft_strcmp(">>", *a) == 0 || ft_strcmp("<<", *a) == 0)
				{
					count++;
					input++;
				}
				break ;
			}
			a++;
		}
		if (!accepted)
			return (count);
		count++;
		input++;
	}
	return (count);
}
*/

/*
 * A l'inverse, renvoie la longueur de la partie de input qui ne contient
 * aucun delimitateur
*/
/*
size_t	ft_strcspn(const char *input, char **reject, t_args **list)
{
	char		**r;
	size_t		count;
	int			rejected;

	count = 0;
	while (*input)
	{
		r = reject;
		rejected = 0;
		while (*r != NULL)
		{
			if (ft_strncmp(input, *r, ft_strlen(*r)) == 0)
			{
				delimit_to_token(*r, list);
				rejected = 1;
				if (ft_strcmp(">>", *r) == 0 || ft_strcmp("<<", *r) == 0)
					count++;
				break ;
			}
			r++;
		}
		if (rejected)
			return (count);
		count++;
		input++;
	}
	return (count);
}
*/
