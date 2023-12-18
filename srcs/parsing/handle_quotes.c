/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajeannin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/13 16:26:02 by ajeannin          #+#    #+#             */
/*   Updated: 2023/12/18 19:03:54 by ajeannin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static int	is_quote(char c, char *flag)
{
	static int quotes = 0;

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

char* substring(char* str, int i, int n)
{
	int len;
	int sub_len;
	char *result;

    if (str == NULL || i < 0 || n < 0 || n < i)
        return (NULL);
    len = ft_strlen(str);
    if (i >= len)
        return (NULL);
    if (n >= len)
		n = len - 1;
    sub_len = n - i + 1;
    result = (char*)ft_malloc((sub_len + 1) * sizeof(char), 1);
    if (result == NULL)
        return (NULL);
    ft_strncpy(result, str + i, sub_len);
    result[sub_len] = '\0';
    return (result);
}

static void	split_str_if_quotes(t_args *current, t_args **sublist)
{
	int i;
	int prev;
	char *str;

	i = 0;
	prev = 0;
	str = ft_strdup(current->str);
	if (ft_strchr(current->str, 39) == NULL && ft_strchr(current->str, 34) == NULL)
		return (add_arg(sublist, current->str, current->token));
	while (str[i])
	{
		while (is_quote(str[i], "SEARCH") == 0 && str[i])
			i++;
		if (i > 0)
		{
			add_arg(sublist, substring(str, prev, i - 1), 12910);
//			printf("we had %s, %d\n", substring(str, prev, i - 1), is_quote(0, "VALUE"));
		}
		if (i == 0 && !str[i + 1])
		{
			is_quote(0, "RESET");
			add_arg(sublist, "NULL\0", 42);
//			printf("we had %s\n", "NULL1\0");
			break ;
		}
		prev = i + 1;
		i++;
		while (is_quote(str[i], "SEARCH") < 20 && str[i])
			i++;
		if (i > prev)
		{
			add_arg(sublist, substring(str, prev, i - 1), is_quote(0, "VALUE"));
//			printf("we had %s, %d\n", substring(str, prev, i - 1), is_quote(0, "VALUE"));
		}
		prev = i + 1;
		i++;
		if (is_quote(0, "VALUE") == 42)
		{
			is_quote(0, "RESET");
			add_arg(sublist, "NULL\0", 42);
//			printf("we had %s\n", "NULL2\0");
			break ;
		}
		is_quote(0, "RESET");
	}
//	print_args_list(sublist);
}

static t_args	*looking_for_quotes(t_args *current, t_args **e_list)
{
	t_args *sublist;

	sublist = NULL;
	split_str_if_quotes(current, &sublist);
//	printf("it's okay buddy...\n");
	if (update_args2(&sublist, e_list) == 1)
		return (NULL);
//	printf("---Sublist---\n");
//	print_args_list(&sublist);
//	printf("-------------\n");
	return (join_nodes(&sublist));
}

int handle_quotes(t_args **list, t_args **e_list)
{
	t_args *stock;
	t_args *next;
	t_args *current;

	stock = NULL;
	current = *list;
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


