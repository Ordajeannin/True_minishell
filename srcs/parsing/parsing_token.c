/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_token.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalic <asalic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 18:29:25 by ajeannin          #+#    #+#             */
/*   Updated: 2023/10/05 17:32:00 by asalic           ###   ########.fr       */
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
	while (tmp[i])
	{
		tmp[i] = is_env_var(tmp[i], env_list);
		if (tmp[i] != NULL && node->str != NULL)
		{
			tmp_node = ft_strdup(node->str);
			free(node->str);
			node->str = ft_strjoin(tmp_node, tmp[i]);
			free(tmp_node);
			if (!node->str)
			{
				while (i >= 0 && tmp[i])
					free(tmp[i--]);
				free(tmp);
				return (1);
			}
		}
		else if (tmp[i] != NULL && node->str == NULL)
		{
			if (node->str)
				free(node->str);
			node->str = ft_strdup(tmp[i]);
			if (!node->str)
			{
				while (i >= 0 && tmp[i])
					free(tmp[i--]);
				free(tmp);
				return (1);
			}
		}
		else
		{
			while (--i >= 0 && tmp[i])
				free(tmp[i]);
			// free(tmp);
		}
		i++;
	}
	i = 0;
	if (tmp[i])
	{
		while (i >= 0 && tmp[i])
			free(tmp[i--]);
		free(tmp);
	}
	return (0);
}

/*
void	process_not_s_quotes(t_args *node, t_args **env_list)
{
	char	**tmp;
	char	*verif;
	char	*is_ve;
	char	*tmp_node;
	int		i;

	tmp = ft_split_arg(node->str);
	if (! tmp)
		return (1);
	node->str = NULL;
	tmp_node = NULL;
	i = 0;
	while (tmp[i])
	{
		is_ve = is_env_var(tmp[i], env_list, flag);
		if (!is_ve)
		{
			if (tmp[i])
			{
				while (i >= 0 && tmp[i])
					free(tmp[i--]);
				free(tmp);
			}
			return (1);
		}
		verif = ft_strdup(tmp[i]);
		if (!verif)
		{
			free(is_ve);
			if (tmp[i])
			{
				while (i >= 0 && tmp[i])
					free(tmp[i--]);
				free(tmp);
			}
			return (1);
		}
		tmp[i] = ft_strdup(is_ve);
		if (!tmp[i])
		{
			free(is_ve);
			free(verif);
			while (--i >= 0 && tmp[i])
				free(tmp[i]);
			free(tmp);
			return (1);
		}
		if (tmp[i] != NULL && ft_strcmp(verif, tmp[i]) == 0 && tmp[i][0] == '$')
			node->token = TOKEN_TEMP_VAR;
		if (tmp[i] != NULL && node->str != NULL)
		{
			tmp_node = ft_strdup(node->str);
			node->str = ft_strjoin(tmp_node, tmp[i]);
			free(tmp_node);
			if (!node->str)
			{
				free(is_ve);
				free(verif);
				while (i >= 0 && tmp[i])
					free(tmp[i--]);
				free(tmp);
				return (1);
			}
		}
		if (tmp[i] != NULL && node->str == NULL)
		{
			free(node->str);
			node->str = ft_strdup(tmp[i]);
			if (!node->str)
			{
				free(is_ve);
				free(verif);
				while (i >= 0 && tmp[i])
					free(tmp[i--]);
				free(tmp);
				return (1);
			}
		}
		i++;
		free(verif);
		free(is_ve);
	}
	while (--i >= 0 && tmp[i])
		free(tmp[i]);
	free(tmp);
	return (0);
}
*/
/*
 * Permet de remplacer les variables d'environnement par leurs valeurs.
 * :warning: uniquement celles existantes / qui ne sont pas entre single quotes!
 * ensuite, tokenize les arguments
 * {
			if (process_not_s_quotes(current, env_list, 1) == 1)
				return (1);
	}
*/

/*
 * Remplace les $var APRES la gestion des heredocs
*/
int	update_args2(t_args **list, t_args **env_list)
{
	t_args	*current;
	
	current = *list;
	while (current != NULL)
	{
		if (current->token != TOKEN_S_QUOTES)
		{
			if (process_not_s_quotes(current, env_list) == 1)
				return (1);
		}
		current = current->next;
	}
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
//		if (current->token != TOKEN_S_QUOTES)
//		{
//			if (process_not_s_quotes(current, env_list) == 1)
//				return (1);
//		}
		if (current->str != NULL && current->token < 20)
			current->token = tokenize_args(current->str, 0);
		if (current->str != NULL && current->token == 23)
			current->token = tokenize_args(current->str, 23);
		if (current->str == NULL && current->token == 23)
		{
			if (current->str != NULL)
				free(current->str);
			current->str = ft_strdup(help);
			if (!current->str)
				return (1);
		}
		current = current->next;
	}
	delete_null_nodes(list);
	return (0);
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
