/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_token.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkorsako <pkorsako@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 18:29:25 by ajeannin          #+#    #+#             */
/*   Updated: 2023/12/18 18:50:29 by ajeannin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Permet de gagner de la place pour delete_null_nodes (input == NULL)
 * Soutien tokenize_args, pour reperer un $? / "$?" (input != NULL)
 * (seul, ou dans une string)
*/
static int	help_delete_or_token(t_args **prev, t_args **current, char	*input)
{
	int	i;

	i = 0;
	if (input == NULL)
	{
		*prev = *current;
		*current = (*current)->next;
	}
	else
	{
		while (input[i] != '\0')
		{
			if (input[i] == '$')
			{
				if (input[i + 1] == '?')
					return (1);
			}
			i++;
		}
	}
	return (0);
}

/*
 * Permet de supprimer les arguments qui se voulaient etre des
 * variables d'environnement, mais mal formatee
*/
void	delete_null_nodes(t_args **list)
{
	t_args	*current;
	t_args	*prev;

	if (*list == NULL)
		return ;
	current = *list;
	prev = NULL;
	while (current != NULL)
	{
		if (current->str == NULL && current->token != 23)
		{
			if (prev == NULL)
				*list = current->next;
			else
				prev->next = current->next;
			current = current->next;
		}
		else
			help_delete_or_token(&prev, &current, NULL);
	}
}

/*
 * Analyse une chaine de caractere issue d'input
 * Pour y associer un token
 * :warning: CMD, OPTION, FPATH et RPATH ne sont pas encore traites (cf 12910)
*/
int	tokenize_args(char *input, int flag)
{
	if (help_delete_or_token(NULL, NULL, input) == 1 && flag == 23)
		return (TOKEN_INTERDOT_D_QUOTES);
	else if (flag == 23)
		return (flag);
	else if (input[0] == '&' && input[1] == '&' && input[2] == '\0')
		return (TOKEN_AND);
	else if (input[0] == '|' && input[1] == '|' && input[2] == '\0')
		return (TOKEN_OR);
	else if (input[0] == '|' && input[1] == '\0')
		return (TOKEN_PIPE);
	else if (input[0] == '<' && input[1] == '\0')
		return (TOKEN_INFILE);
	else if (input[0] == '<' && input[1] == '<' && input[2] == '\0')
		return (TOKEN_DELIM);
	else if (input[0] == '>' && input[1] == '\0')
		return (TOKEN_OUTFILE);
	else if (input[0] == '>' && input[1] == '>' && input[2] == '\0')
		return (TOKEN_APPEND);
	else if (help_delete_or_token(NULL, NULL, input) == 1)
		return (TOKEN_INTERDOT);
	else
		return (12910);
}

/*
static void print_split(char **tmp)
{
	int i = 1;

	while (*tmp)
	{
//		printf("tmp[%d] : %s\n", i, *tmp);
		i++;
		tmp++;
	}
}
*/

/*
 * Permet de gerer le remplacement des variables d'environnement,
 * si correctement formate
 * Il faut proteger !!!
*/
int	process_not_s_quotes(t_args *node, t_args **env_list)
{
	char	**tmp;
	int		i;
	char	*tmp_node;

	tmp = ft_split_arg(node->str);
	if (!tmp)
		return (1);
	node->str = NULL;
	tmp_node = NULL;
	i = 0;
//	printf("----------------------------------------\n");
//	print_split(tmp);
//	printf("----------------------------------------\n");
	while (tmp[i])
	{
		tmp[i] = is_env_var(tmp[i], env_list);
		if (tmp[i] != NULL && node->str != NULL)
		{
			tmp_node = ft_strdup(node->str);
			node->str = ft_strjoin(tmp_node, tmp[i]);
			if (!node->str)
				return (1);
		}
		else if (tmp[i] != NULL && node->str == NULL)
		{
			node->str = ft_strdup(tmp[i]);
			if (!node->str)
				return (1);
		}
		i++;
	}
	i = 0;
	return (0);
}

/*
 * Remplace les $var APRES la gestion des heredocs
*/
int	update_args2(t_args **list, t_args **env_list)
{
	t_args	*current;

	delete_null_nodes(list);
	current = *list;
//	printf("hey, are you here?\n");
//	printf("1\n");
	while (current != NULL)
	{
		if (current->token != TOKEN_S_QUOTES)
		{
			if (process_not_s_quotes(current, env_list) == 1)
				return (1);
		}
		current = current->next;
	}
//	printf("2\n");
//	delete_null_nodes(list);
//	printf("3\n");
	return (0);
}

int	update_args(t_args **list)
{
	t_args	*current;
	char	help[2];

	current = *list;
	help[0] = '\0';
	help[1] = '\0';
	while (current != NULL)
	{
		if (current->str != NULL && current->token < 20)
			current->token = tokenize_args(current->str, 0);
		if (current->str != NULL && current->token == 23)
			current->token = tokenize_args(current->str, 23);
		if (current->str == NULL && current->token == 23)
		{
			current->str = ft_strdup(help);
			if (!current->str)
				return (1);
		}
		current = current->next;
	}
	return (0);
}
