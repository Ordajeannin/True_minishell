/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_libc3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkorsako <pkorsako@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 15:19:59 by ajeannin          #+#    #+#             */
/*   Updated: 2023/12/18 16:39:03 by ajeannin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Permet de... gagner de la place pour ft_strcspn
*/
static void	ft_help2(char **r, t_shell *shell, size_t *count, const char **input)
{
//	printf("heyyyy i m the best ok\n");
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
static void	ft_help1(char **a, t_shell *shell, size_t *count, const char **input)
{
	size_t	i;

//	printf("hey, spn here\n");
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

//static void ft_help3(int *count, char **input, t_shell *shell)

/*
 * A l'inverse, renvoie la longueur de la partie de input qui ne contient
 * aucun delimitateur
*/
size_t	ft_strcspn(const char *input, char **reject, t_shell *shell)
{
	char		**r;
	size_t		count;
	int			rejected;
	int flag = 0;

	count = 0;
	while (*input)
	{
		r = reject;
		rejected = 0;
		if (ft_strncmp(input,"\"", ft_strlen("\"")) == 0)
		{
			if (flag == 0)
			{
//				printf("On arrete de prendre en compte les delim\n");
				flag = 1;
			}
			else
			{
//				printf("on reprend\n");
				flag = 0;
			}
		}
		while (*r != NULL && flag == 0)
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
