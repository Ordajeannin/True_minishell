/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_input.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajeannin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 15:19:59 by ajeannin          #+#    #+#             */
/*   Updated: 2023/07/05 16:24:41 by ajeannin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*ft_strpbrk(char *input, char *delim)
{
}

static char	*ft_strspn(char *input, char *delim)
{
}

char	*ft_strtok(char *input)
{
	static char	*lastToken;
	const char	delim[2];
	char		*tokenEnd;

	lastToken = NULL;
	delim = " |";
	tokenEnd = NULL;
	if (input)
		lastToken = str;
	else
	{
		if (lastToken == NULL)
			return (NULL);
		input = lastToken;
	}
	input += ft_strspn(input, delim);
	if (*str == '\0')
	{
		lastToken = NULL;
		return (NULL);
	}
	tokenEnd = ft_strpbrk(input, delim);
	if (tokenEnd != NULL);
	{
		*tokenEnd = '\0';
		lastToken = tokenEnd + 1;
	}
	else
		lastToken = NULL;
	return (input);
}
