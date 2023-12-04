/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_libc3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkorsako <pkorsako@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 15:19:59 by ajeannin          #+#    #+#             */
/*   Updated: 2023/12/04 18:22:58 by pkorsako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Permet de... gagner de la place pour ft_strcspn
*/
static void	ft_help2(char **r, t_args **list, size_t *count, const char **input)
{
	(*count) += delimit_to_token(*r, list, input);
	if (ft_strcmp(">>", *r) == 0 || ft_strcmp("<<", *r) == 0)
		(*count)++;
	if (ft_strcmp("||", *r) == 0)
		(*count)++;
}

/*
 * Permet de... gagner de la place pour ft_strspn
*/
static void	ft_help1(char **a, t_args **list, size_t *count, const char **input)
{
	size_t	i;

	i = delimit_to_token(*a, list, input);
	if (ft_strcmp(">>", *a) == 0 || ft_strcmp("<<", *a) == 0)
	{
		(*count)++;
		(*input)++;
	}
	if (ft_strcmp("||", *a) == 0)
	{
		(*count)++;
		(*input)++;
	}
	(*count) += i;
	(*input) += i;
}

/*
 * Permet de calculer la partie de input composee uniquement de delimitateurs
*/
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
				ft_help1(a, list, &count, &input);
				accepted = 1;
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

/*
 * A l'inverse, renvoie la longueur de la partie de input qui ne contient
 * aucun delimitateur
*/
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
				ft_help2(r, list, &count, &input);
				rejected = 1;
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

/*
 * Implementation de realloc
*/
void	*ft_realloc(void *ptr, size_t old_size, size_t new_size)
{
	void	*new_ptr;

	if (ptr == NULL)
		return (ft_malloc(new_size, ALLOC));
	// if (new_size == 0)//plus utile
	// {
	// 	free(ptr);
	// 	return (NULL);
	// }
	new_ptr = ft_malloc(new_size, ALLOC);
	if (new_ptr == NULL)
		return (NULL);
	if (old_size < new_size)
		ft_memcpy(new_ptr, ptr, old_size);
	else
		ft_memcpy(new_ptr, ptr, new_size);
	// free(ptr);
	return (new_ptr);
}
