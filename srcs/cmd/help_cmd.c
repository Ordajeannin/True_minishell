/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalic <asalic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/11 12:12:28 by asalic            #+#    #+#             */
/*   Updated: 2023/09/18 08:53:58 by asalic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Check si la chaine de caractere est bien remplie de nombre
 * Que des caracteres numeriques
*/
int	is_numeric(char *str)
{
	int	i;

	i = 0;
	if (str[0] == '-')
		i ++;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (1);
		i++;
	}
	return (0);
}

/*
 * Ajout nouveau maillon dans env.
 * Boucle jusqu'a la fin de la liste.
 * Puis ajoute la string.
 * (BUG: sauf qu'il l'ajoute de maniere random ou il veut dans l'env ;-;)
*/
void	add_env(t_args **env_list, char *str)
{
	t_args	*new_var;
	t_args	*current;

	new_var = malloc(sizeof(t_args));
	new_var->str = ft_strdup(str);
	new_var->token = 0;
	new_var->next = NULL;
	if (*env_list)
	{
		current = *env_list;
		while (current->next != NULL)
			current = current->next;
		current->next = new_var;
	}
	else
		*env_list = new_var;
}

/* Compte nombre de maillon dans une liste */
int	len_targs(t_args *list)
{
	int	len;

	len = 0;
	while (list)
	{
		len ++;
		list = list->next;
	}
	return (len);
}

/* Dupplique un (char **) */
char	**dup_double_string(t_args **e_list)
{
	char	**result;
	t_args	*current;
	int		i;

	current = *e_list;
	result = ft_calloc(len_targs(current), sizeof(char *));
	if (result == NULL)
		exit(EXIT_FAILURE);
	i = 0;
	while (current)
	{
		result[i] = ft_strdup(current->str);
		if (result[i] == NULL)
		{
			while (i >= 0)
				free(result[i--]);
			free(result);
			exit(EXIT_FAILURE);
		}
		i ++;
		current = current->next;
	}
	result[i] = NULL;
	return (result);
}
