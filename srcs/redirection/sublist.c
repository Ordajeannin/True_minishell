/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sublist.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajeannin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 15:05:55 by ajeannin          #+#    #+#             */
/*   Updated: 2023/09/28 17:32:08 by ajeannin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Permet d'acceder au dernier maillon de la chaine
*/
t_args_list	*ft2_lstlast(t_args_list *lst)
{
	if (lst)
	{
		while (lst->next)
			lst = lst->next;
	}
	return (lst);
}

/*
 * Permet d'ajouter un maillon a la fin de la chaine
*/
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

/*
 * Permet de print chaque sublist, contenues dans
 * la liste chainee stock.
 * Fonction de debuggage
*/
void	print_sublists(t_args_list *stock)
{
	t_args_list	*current_sublist;
	t_args		*current_arg;

	current_sublist = stock;
	while (current_sublist)
	{
		current_arg = current_sublist->head;
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

/*
 * Permet de normer stock_sublist
 * Dans les faits, boucle en parcourant **list (current),
 * et cree des sous-chaines avec les maillons situes entre 
 * [head] et [TOKEN_PIPE]
 * ou 
 * [TOKEN_PIPE] et [TOKEN_PIPE]
 * le head de chaque sous-chaine devenant un maillon de **stock
*/
static void	help_stock(t_args_list **stock, t_args **start, t_args **current)
{
	t_args		*prev;
	t_args_list	*new_node;

	prev = NULL;
	while (*current)
	{
		if ((*current)->token == TOKEN_PIPE)
		{
			new_node = malloc(sizeof(t_args_list));
			if (!new_node)
			{
				perror("malloc");
				return ;
			}
			new_node->head = *start;
			new_node->next = NULL;
			ft2_lstadd_back(stock, new_node);
			prev->next = NULL;
			*start = (*current)->next;
			*current = *start;
		}
		prev = *current;
		*current = (*current)->next;
	}
}

/*
 * Permet de creer, depuis **list, une 2eme liste chainee contenant
 * toutes les sous chaines de list en se basant sur l'occurence de TOKEN_PIPE
 * Help_stock creee les sous-chaines precedant un pipe, la suite du code
 * creee la derniere sous chaine.
 * on aura :
 * 
 * [stock 0] -> [sublist 0.0] -> [0.1] -> [0.2] -> etc
 *     |
 *     v
 * [stock 1] -> [sublist 1.0] -> [1.1] -> [1.2] -> etc
 *     |
 *     v
 * [stock 2] -> [sublist 2.0] -> [2.1] -> etc
 *	   |
 *	   v
 *	  etc
 *
 */
t_args_list	*stock_sublist(t_args **list)
{
	t_args		*start;
	t_args		*current;
	t_args_list	*stock;
	t_args_list	*new_node;

	start = (*list);
	current = (*list);
	stock = NULL;
	help_stock(&stock, &start, &current);
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

/*
 * Last Stable Version
 * A garder si fonctions normees instables!
*/
/*
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
				return (NULL);
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
*/
