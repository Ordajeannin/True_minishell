/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_token.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajeannin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 18:29:25 by ajeannin          #+#    #+#             */
/*   Updated: 2023/07/07 18:42:06 by ajeannin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Fonction permettant de rajouter un espace a la fin d'un argument
 * (update en verifiant le token?)
static char	*add_space(char	*str)
{
	char	*result;
	int		len;

	len = ft_strlen(str);
	result = malloc(sizeof(char) * len + 1);
	if (!result)
		return (NULL);
	ft_strncpy(result, str, len);
	result[len] = ' ';
	result[len + 1] = '\0';
	return (result);
}
*/

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
static void	delete_null_nodes(t_args **list)
{
	t_args	*current;
	t_args	*prev;
	t_args	*temp;

	if (*list == NULL)
		return ;
	current = *list;
	prev = NULL;
	while (current != NULL)
	{
		if (current->str == NULL && current->token != 23)
		{
			temp = current;
			if (prev == NULL)
				*list = current->next;
			else
				prev->next = current->next;
			current = current->next;
			free(temp);
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
 * Permet de gerer le remplacement des variables d'environnement,
 * si correctement formate
*/
static void	process_not_s_quotes(t_args *node, t_args **env_list)
{
	char	**tmp;
	int		i;

	tmp = ft_split_arg(node->str);
	node->str = NULL;
	i = 0;
	while (tmp[i])
	{
		tmp[i] = is_env_var(tmp[i], env_list);
		if (tmp[i] != NULL && node->str != NULL)
			node->str = ft_strjoin(node->str, tmp[i]);
		if (tmp[i] != NULL && node->str == NULL)
			node->str = tmp[i];
		i++;
	}
}

/*
 * Permet de remplacer les variables d'environnement par leurs valeurs.
 * :warning: uniquement celles existantes / qui ne sont pas entre single quotes!
 * ensuite, tokenize les arguments
*/
void	update_args(t_args **list, t_args **env_list)
{
	t_args	*current;
	char	help[2];

	current = *list;
	help[0] = '\0';
	help[1] = '\0';
	while (current != NULL)
	{
		if (current->token != TOKEN_S_QUOTES)
			process_not_s_quotes(current, env_list);
		if (current->str != NULL && current->token < 20)
			current->token = tokenize_args(current->str, 0);
		if (current->str != NULL && current->token == 23)
			current->token = tokenize_args(current->str, 23);
		if (current->str == NULL && current->token == 23)
			current->str = help;
		current = current->next;
	}
	delete_null_nodes(list);
}

/*
 * Permet de verifier si un argument contient une variable d'environnement
 * correctement formatee, et de la remplacer
 * Permet egalement d'associer un token a une string pour chaque maillon
 * de la liste chainee input
 * A CONSERVER TEMPORAIREMENT, si probleme avec le raccourcissement
*/
/*
void	update_args(t_args **list, t_args **env_list)
{
	t_args	*current;
	char	**tmp;
	int		i;

	current = *list;
	tmp = NULL;
	while (current != NULL)
	{
		i = 0;
		if (current->token != TOKEN_S_QUOTES)
		{
			tmp = ft_split_arg(current->str);
			current->str = NULL;
			while (tmp[i])
			{
				tmp[i] = is_env_var(tmp[i], env_list);
				if (tmp[i] != NULL && current->str != NULL)
					current->str = ft_strjoin(current->str, tmp[i]);
				if (tmp[i] != NULL && current->str == NULL)
					current->str = tmp[i];
				i++;
			}
		}
		if (current->str != NULL && current->token < 20)
			current->token = tokenize_args(current->str);
		current = current->next;
	}
	delete_null_nodes(list);
}
*/
