/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_quote_flag.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajeannin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/22 12:44:26 by ajeannin          #+#    #+#             */
/*   Updated: 2023/12/22 13:03:57 by ajeannin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	help_is_quote(int quotes)
{
	if (quotes > 20 || quotes == 0)
		return (quotes);
	else
		return (ABORT_MISSION);
}

/*
 * Permet d'avoir un flag gerant ouverture/fermeture/valeurs des quotes
*/
int	is_quote(char c, char *flag)
{
	static int	quotes = 0;

	if (ft_strcmp(flag, "RESET") == 0)
	{
		quotes = 0;
		return (quotes);
	}
	if (ft_strcmp(flag, "VALUE") == 0)
		return (help_is_quote(quotes));
	if (quotes == 0)
	{
		if (c == '\'')
			quotes = 1;
		if (c == '\"')
			quotes = 2;
	}
	else
	{
		if (c == '\'' && quotes == 1)
			quotes = TOKEN_S_QUOTES;
		if (c == '\"' && quotes == 2)
			quotes = TOKEN_D_QUOTES;
	}
	return (quotes);
}
