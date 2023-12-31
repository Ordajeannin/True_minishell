/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkorsako <pkorsako@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/13 16:26:02 by ajeannin          #+#    #+#             */
/*   Updated: 2023/12/22 14:10:39 by ajeannin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Permet de join les node->str de la sublist.
 * Si erreur pour l'un des node, le node final en heritera
*/
static t_args	*join_nodes(t_args **sublist)
{
	t_args	*current;
	char	*str;
	int		token;

	current = *sublist;
	str = NULL;
	token = 0;
	while (current)
	{
		str = ft_strjoin(str, current->str);
		if (token == 42 || current->token == 42)
			token = 42;
		else if (token != current->token)
			token += current->token;
		current = current->next;
	}
	return (create_arg(str, token));
}

/*
 * Renvoie une portion de chaine, from index i to n
*/
char	*substring(char *str, int i, int n)
{
	int		len;
	int		sub_len;
	char	*result;

	if (str == NULL || i < 0 || n < 0 || n < i)
		return (NULL);
	len = ft_strlen(str);
	if (i >= len)
		return (NULL);
	if (n >= len)
		n = len - 1;
	sub_len = n - i + 1;
	result = (char *)ft_malloc((sub_len + 1) * sizeof(char), 1);
	if (result == NULL)
		return (NULL);
	ft_strncpy(result, str + i, sub_len);
	result[sub_len] = '\0';
	return (result);
}

/*
 * Au Final, tout se gere la :
 *
 * 1) Recoit un maillon en input
 * 2) Va creer une sublist chainee a partir de ce maillon,
 *    en fonction des quotes
 * 3) va gerer les substitutions des VE pour chacun de ces maillons
 *    independemment
 * 4) retourne un unique maillon, un join des str de la sublist
 *
 * Exemple :
 * Input   :   '$USER'"$USER"
 * sublist :   $USER  ->  $USER
 * V.E     :   $USER  ->  ajeannin
 * Return  :   $USERajeannin
*/
static t_args	*looking_for_quotes(t_args *current, t_args **e_list)
{
	t_args	*sublist;

	sublist = NULL;
	split_str_if_quotes(current, &sublist);
	if (update_args2(&sublist, e_list) == 1)
		return (NULL);
	return (join_nodes(&sublist));
}

/*
 * Permet de gerer les maillons contenant des simples/doubles quotes,
 * et la substitution des variables d'environnement.
 * Va creer une liste chainee secondaire, avec les maillons updated.
 * liste pointera vers elle.
 *
 * 1) is_quote(RESET) est une protection, la fonction est appelee plus tot et sa
 *    reintialisation est ... chiante, dans certain cas
 * 2) parcours la liste chainee pour effectuer looking_for_quotes
 *    sur chaque maillon
 * 3) ajoute le maillon updated a la nouvelle liste
 * 4) redirection du pointeur
 *    *list = stock
 * 5) un peu de clean ne fait pas de mal (en cas de "" par exemple)
 *    delete_null_nodes
*/
int	handle_quotes(t_args **list, t_args **e_list)
{
	t_args	*stock;
	t_args	*next;
	t_args	*current;

	stock = NULL;
	current = *list;
	is_quote(0, "RESET");
	while (current)
	{
		next = current->next;
		current = looking_for_quotes(current, e_list);
		add_arg(&stock, current->str, current->token);
		current = next;
	}
	*list = stock;
	delete_null_nodes(list);
	return (0);
}
