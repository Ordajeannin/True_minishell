/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_libc3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkorsako <pkorsako@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 15:19:59 by ajeannin          #+#    #+#             */
/*   Updated: 2023/12/20 07:27:48 by ajeannin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Permet de... gagner de la place pour ft_strcspn
*/
static void	ft_help2(char **r, t_shell *shell, size_t *count,
		const char **input)
{
	if (ft_strcmp("\"", *r) != 0)
		(*count) += delimit_to_token(*r, &(shell->list), input);
	if (ft_strcmp(">>", *r) == 0 || ft_strcmp("<<", *r) == 0)
		(*count)++;
	if (ft_strcmp("||", *r) == 0)
		(*count)++;
}

/*
 * Permet de... gagner de la place pour ft_strspn
*/
static void	ft_help1(char **a, t_shell *shell, size_t *count,
		const char **input)
{
	size_t	i;

	i = delimit_to_token(*a, &(shell->list), input);
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
size_t	ft_strspn(const char *input, char **accept, t_shell *shell)
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
				ft_help1(a, shell, &count, &input);
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

static int	process_while_loop(const char *input, char **reject,
						t_shell *shell, size_t *count)
{
	char	**r;
	int		rejected;

	r = reject;
	rejected = 0;
	while (*r != NULL && is_quote(0, "VALUE") == 0)
	{
		if (ft_strncmp(input, *r, ft_strlen(*r)) == 0)
		{
			ft_help2(r, shell, count, &input);
			rejected = 1;
			break ;
		}
		r++;
	}
	return (rejected);
}

size_t	ft_strcspn(const char *input, char **reject, t_shell *shell)
{
	size_t	count;
	int		rejected;

	count = 0;
	rejected = 0;
	is_quote(0, "RESET");
	while (*input)
	{
		if (is_quote(*input, "SEARCH") > 20)
			is_quote(0, "RESET");
		rejected = process_while_loop(input, reject, shell, &count);
		if (rejected)
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
/*
size_t	ft_strcspn(const char *input, char **reject, t_shell *shell)
{
	char		**r;
	size_t		count;
	int			rejected;

	count = 0;
	is_quote(0, "RESET");
	while (*input)
	{
		r = reject;
		rejected = 0;
		if (is_quote(*input, "SEARCH") > 20)
			is_quote(0, "RESET");
		while (*r != NULL && is_quote(0, "VALUE") == 0)
		{
			if (ft_strncmp(input, *r, ft_strlen(*r)) == 0)
			{
				ft_help2(r, shell, &count, &input);
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
*/
