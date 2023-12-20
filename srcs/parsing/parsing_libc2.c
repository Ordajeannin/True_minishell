/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_libc2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajeannin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 15:19:59 by ajeannin          #+#    #+#             */
/*   Updated: 2023/12/20 06:33:17 by ajeannin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Copie une chaine de caracteres source vers une chaine specifiee
 * Limite la copie jusqu a 'n' caracteres
 * retourne un pointeur vers la destination
*/
char	*ft_strncpy(char *dest, const char *src, int n)
{
	char	*original_dest;

	original_dest = dest;
	while (*src && n > 0)
	{
		*dest++ = *src++;
		n--;
	}
	while (n > 0)
	{
		*dest++ = '\0';
		n--;
	}
	return (original_dest);
}

char	*ft_strcpy(char *dest, const char *src)
{
	char	*ptr;

	ptr = dest;
	while (*src)
	{
		*dest = *src;
		dest++;
		src++;
	}
	*dest = '\0';
	return (ptr);
}

/*
 * Recherche la derniere occurence d'un caractere specifie dans une string
 * Retourne un pointeur vers la derniere occurence du caractere dans la chaine
 * ou NULL si absence du caractere
*/
char	*ft_strrchr(const char *str, int ch)
{
	const char	*last_occurrence;

	last_occurrence = NULL;
	while (*str)
	{
		if (*str == ch)
			last_occurrence = str;
		str++;
	}
	if (*str == ch)
		last_occurrence = str;
	return ((char *)last_occurrence);
}

/*
 * Implementation de strcat
*/
char	*ft_strcat(char *str1, const char *str2)
{
	char	*str;

	str = str1;
	while (*str)
		str++;
	while (*str2)
	{
		*str = *str2;
		str++;
		str2++;
	}
	*str = '\0';
	return (str);
}

/*
 * alloue une sous chaine, et l'extrait de str
*/
char	*word_dup(char *str, int start, int finish)
{
	char	*word;
	int		i;

	i = 0;
	word = ft_calloc(finish - start + 1, sizeof(char));
	if (!word)
		return (NULL);
	while (start < finish)
		word[i++] = str[start++];
	word[i] = '\0';
	return (word);
}
