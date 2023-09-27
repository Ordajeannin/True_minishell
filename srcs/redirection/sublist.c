/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sublist.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajeannin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 15:05:55 by ajeannin          #+#    #+#             */
/*   Updated: 2023/09/27 16:10:01 by ajeannin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_args_list	*ft2_lstlast(t_args_list *lst)
{
	if (lst)
	{
		while (lst->next)
			lst = lst->next;
	}
	return (lst);
}

void	ft2_lstadd_back(t_args_list **lst, t_args_list *new)
{
	t_args_list	*scd;

	if (lst)
	{
		if (*lst == NULL)
			*lst = new;
		else
		{
			scd = ft2_lstlast(*(lst));
			scd->next = new;
		}
	}
}

void	print_sublists(t_args_list *stock)
{
	t_args_list	*current_sublist = stock;

	while (current_sublist)
	{
		t_args	*current_arg = current_sublist->head;
		printf("Sublist: ");
		while (current_arg)
		{
			printf("%s ", current_arg->str);
			current_arg = current_arg->next;
		}
		printf("\n");
		current_sublist = current_sublist->next;
	}
}

t_args_list	*stock_sublist(t_args **list)
{
	t_args		*current = (*list);
	t_args		*start = (*list);
	t_args		*prev = NULL;
	t_args_list	*stock = NULL;
	t_args_list	*new_node = NULL;

	while (current)
	{
		if (current->token == TOKEN_PIPE)
		{
			new_node = malloc(sizeof(t_args_list));
			if (!new_node)
			{
				perror("malloc");
				return NULL;
			}
			new_node->head = start;
			new_node->next = NULL;
			ft2_lstadd_back(&stock, new_node);
			prev->next = NULL;
			start = current->next;
			current = start;
		}
		prev = current;
		current = current->next;
	}
	new_node = malloc(sizeof(t_args_list));
	if (!new_node)
	{
		perror("malloc");
		return (NULL);
	}
	new_node->head = start;
	new_node->next = NULL;
	ft2_lstadd_back(&stock, new_node);
	return (stock);
}

