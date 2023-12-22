/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_str_if_quotes.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajeannin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/22 13:15:30 by ajeannin          #+#    #+#             */
/*   Updated: 2023/12/22 14:10:03 by ajeannin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	help_ssiq(int *i, int *prev, char *str, t_args **sublist)
{
	if (*i > 0)
		add_arg(sublist, substring(str, *prev, *i - 1), 12910);
	if (i == 0 && !str[*i + 1])
	{
		is_quote(0, "RESET");
		add_arg(sublist, "NULL\0", 42);
		return (1);
	}
	*prev = *i + 1;
	(*i)++;
	while (is_quote(str[*i], "SEARCH") < 20 && str[*i])
		(*i)++;
	if (*i > *prev)
		add_arg(sublist, substring(str, *prev, *i - 1), is_quote(0, "VALUE"));
	*prev = *i + 1;
	(*i)++;
	if (is_quote(0, "VALUE") == 42)
	{
		is_quote(0, "RESET");
		add_arg(sublist, "NULL\0", 42);
		return (1);
	}
	return (0);
}

/*
 * Permet de creer une sublist a partir d'un unique maillon
 * Ne rentre dans la boucle que si >= un quote a ete trouve dans current->str
 *
 * On parcourt la chaine jusqu'a une s/d quote, X
 * si quote && progression :
 * creation d'un arg pour le contenu precedant la quote
 *
 * Puis, on parcourt la chaine jusqu'a un 2eme X ou fin (ignorant l'autre quote)
 * creation d'un arg, indiquant si la quote a pu etre fermee
 *
 * On reset notre flag pour les s/d quotes, puis on boucle
*/
void	split_str_if_quotes(t_args *current, t_args **sublist)
{
	int		i;
	int		prev;
	char	*str;

	i = 0;
	prev = 0;
	str = ft_strdup(current->str);
	if (ft_strchr(current->str, 39) == NULL
		&& ft_strchr(current->str, 34) == NULL)
		return (add_arg(sublist, current->str, current->token));
	while (str[i])
	{
		while (is_quote(str[i], "SEARCH") == 0 && str[i])
			i++;
		if (help_ssiq(&i, &prev, str, sublist) == 1)
			break ;
		is_quote(0, "RESET");
	}
}
