/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_libc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalic <asalic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 15:19:59 by ajeannin          #+#    #+#             */
/*   Updated: 2023/12/20 07:28:18 by ajeannin         ###   ########.fr       */
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

/*
 * Implementation de realloc
 * //plus utile
 * inscrit au niveau de la protection de new_size, why?
*/
void	*ft_realloc(void *ptr, size_t old_size, size_t new_size)
{
	void	*new_ptr;

	if (ptr == NULL)
		return (ft_malloc(new_size, ALLOC));
	if (new_size == 0)
		return (NULL);
	new_ptr = ft_malloc(new_size, ALLOC);
	if (new_ptr == NULL)
		return (NULL);
	if (old_size < new_size)
		ft_memcpy(new_ptr, ptr, old_size);
	else
		ft_memcpy(new_ptr, ptr, new_size);
	return (new_ptr);
}
