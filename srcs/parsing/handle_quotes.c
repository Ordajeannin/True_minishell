/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajeannin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/13 16:26:02 by ajeannin          #+#    #+#             */
/*   Updated: 2023/12/20 08:06:21 by ajeannin         ###   ########.fr       */
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
	{
		if (quotes > 20 || quotes == 0)
			return (quotes);
		else
			return (ABORT_MISSION);
	}
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
static void	split_str_if_quotes(t_args *current, t_args **sublist)
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
		if (i > 0)
			add_arg(sublist, substring(str, prev, i - 1), 12910);
		if (i == 0 && !str[i + 1])
		{
			is_quote(0, "RESET");
			add_arg(sublist, "NULL\0", 42);
			break ;
		}
		prev = i + 1;
		i++;
		while (is_quote(str[i], "SEARCH") < 20 && str[i])
			i++;
		if (i > prev)
			add_arg(sublist, substring(str, prev, i - 1), is_quote(0, "VALUE"));
		prev = i + 1;
		i++;
		if (is_quote(0, "VALUE") == 42)
		{
			is_quote(0, "RESET");
			add_arg(sublist, "NULL\0", 42);
			break ;
		}
		is_quote(0, "RESET");
	}
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
	printf("---------------------------------------------------------------\n");
	printf("_____________________________________\n");
	printf("_____________ MAIN LIST _____________\n");
	printf("_____________________________________\n");
	print_args_list(list);
	printf("_____________________________________\n");
	while (current)
	{
		next = current->next;
		current = looking_for_quotes(current, e_list);
		add_arg(&stock, current->str, current->token);
		current = next;
	}
	*list = stock;
	delete_null_nodes(list);
	printf("_____________ NEW LIST ______________\n");
	printf("_____________________________________\n");
	print_args_list(list);
	printf("_____________________________________\n");
	printf("_____________ EXECUTION _____________\n");
	printf("_____________________________________\n\n");
	return (0);
}
