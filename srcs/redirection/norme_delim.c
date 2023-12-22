/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   norme_delim.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajeannin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/22 15:36:06 by ajeannin          #+#    #+#             */
/*   Updated: 2023/12/22 17:40:19 by ajeannin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	help_help_hhp1(t_args **current, t_args **prev)
{
	(*current) = NULL;
	if ((*prev) != NULL)
		(*prev)->next = NULL;
}

int	help_hhp1(t_args **stock, t_args **current, t_args **prev, t_args **input)
{
	if ((*current)->next != NULL)
	{
		add_arg(stock, (*current)->next->str, TOKEN_DELIM);
		if (prev == NULL)
			*input = (*current)->next;
		else
			(*prev)->next = (*current)->next->next;
		if ((*current)->next != NULL)
			(*current) = (*current)->next->next;
		else
			help_help_hhp1(current, prev);
	}
	else
	{
		add_arg(stock, NULL, -66);
		if ((*prev) != NULL)
			(*prev)->next = NULL;
		else
			*input = NULL;
		return (1);
	}
	return (0);
}

int	handle_heredoc(t_args **input)
{
	t_args	*current;
	t_args	*prev;
	t_args	*stock;

	current = *input;
	prev = NULL;
	stock = NULL;
	if (input == NULL || *input == NULL)
		return (0);
	while (current != NULL)
	{
		if (current->token == TOKEN_DELIM)
		{
			if (help_hhp1(&stock, &current, &prev, input) == 1)
				break ;
		}
		else
		{
			prev = current;
			current = current->next;
		}
	}
	if (handle_heredoc_part2(&stock) == 2)
		return (2);
	return (0);
}
