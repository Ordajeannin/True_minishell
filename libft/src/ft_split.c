/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalic <asalic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/09 00:59:01 by ajeannin          #+#    #+#             */
/*   Updated: 2023/09/29 11:50:43 by asalic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static	int	count_words(const char *str, char c)
{
	int	i;
	int	count;
	int	breaking_point;

	count = 0;
	i = 0;
	breaking_point = 0;
	while (str[i])
	{
		if (str[i] != c && breaking_point == 0)
		{
			breaking_point = 1;
			count++;
		}
		else if (str[i] == c)
			breaking_point = 0;
		i++;
	}
	return (count);
}

static	char	*word_dup(const char *str, int start, int finish)
{
	char	*word;
	int		i;

	i = 0;
	word = ft_calloc((finish - start + 1), sizeof(char));
	if (! word)
		return (NULL);
	while (start < finish)
		word[i++] = str[start++];
	word[i] = '\0';
	return (word);
}

static	char	**helpsplit(int index, char const *s, char c)
{
	size_t	i;
	size_t	j;
	int		len;
	char	**new;

	i = 0;
	j = 0;
	len = count_words(s, c);
	if (len == 0)
		return (NULL);
	new = ft_calloc((len + 1), sizeof(char *));
	if (! new)
		return (NULL);
	while (i <= ft_strlen(s))
	{
		if (s[i] != c && index < 0)
			index = i;
		else if ((s[i] == c || i == ft_strlen(s)) && index >= 0)
		{
			new[j++] = word_dup(s, index, i);
			index = -1;
		}
		i++;
	}
	new[j] = 0;
	return (new);
}

char	**ft_split(char const *s, char c)
{
	int	index;

	index = -1;
	if (!s || s[0] == '\0')
		return (NULL);
	return (helpsplit(index, s, c));
}
