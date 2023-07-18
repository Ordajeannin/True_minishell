/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_libc3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajeannin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 15:19:59 by ajeannin          #+#    #+#             */
/*   Updated: 2023/07/18 18:31:07 by ajeannin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Permet de... gagner de la place pour ft_strcspn
*/
static void	ft_help2(char **r, t_args **list, size_t *count)
{
	delimit_to_token(*r, list);
	if (ft_strcmp(">>", *r) == 0 || ft_strcmp("<<", *r) == 0)
		(*count)++;
}

/*
 * Permet de... gagner de la place pour ft_strspn
*/
static void	ft_help1(char **a, t_args **list, size_t *count, const char **input)
{
	delimit_to_token(*a, list);
	if (ft_strcmp(">>", *a) == 0 || ft_strcmp("<<", *a) == 0)
	{
		(*count)++;
		(*input)++;
	}
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
				ft_help2(r, list, &count);
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
