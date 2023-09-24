/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_libc2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajeannin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 15:19:59 by ajeannin          #+#    #+#             */
/*   Updated: 2023/07/11 23:31:07 by ajeannin         ###   ########.fr       */
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
