/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_of_args.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalic <asalic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 17:55:04 by ajeannin          #+#    #+#             */
/*   Updated: 2023/09/05 11:02:03 by asalic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Fonction utilitaire, libere chaque maillon de la chaine d'arguments
 * conserve *liste et le reset a NULL
*/
void	clear_args_list(t_args **list)
{
	t_args	*current;
	t_args	*next;

	current = *list;
	while (current != NULL)
	{
		next = current->next;
		free(current);
		current = next;
	}
	*list = NULL;
}

/*
 * Fonction utilitaire, cree un maillon 
 * a partir du contenu de l'argument et du token attribue
*/
static t_args	*create_arg(char *str, int token)
{
	t_args	*new_arg;

	new_arg = (t_args *)malloc(sizeof(t_args));
	if (!new_arg)
		return (NULL);
	new_arg->str = str;
	new_arg->token = token;
	new_arg->next = NULL;
	return (new_arg);
}

/*
 * Fonction utilitaire, ajoute un maillon a la fin de la chaine
 * appel a create_arg pour creer le maillon
*/
void	add_arg(t_args **list, char *str, int token)
{
	t_args	*new_arg;
	t_args	*current;

	new_arg = create_arg(str, token);
	if (!new_arg)
		return ;
	if (*list == NULL)
		*list = new_arg;
	else
	{
		current = *list;
		while (current->next != NULL)
			current = current->next;
		current->next = new_arg;
	}
}

/*
 * Permet d'extraire les tokens de input sur base des delimitateurs
 * Puis identifie ces tokens
*/
void	from_input_to_list_of_args(char *input, t_args **list, t_args **e_list)
{
	char	*delim[11];
	char	*token;

	delim[0] = " ";
	delim[1] = "\t";
	delim[2] = "<<";
	delim[3] = "<";
	delim[4] = ">>";
	delim[5] = ">";
	delim[6] = "||";
	delim[7] = "|";
	delim[8] = "\'";
	delim[9] = "\"";
	delim[10] = NULL;
	token = ft_strtok(input, delim, list);
	while (token != NULL)
		token = ft_strtok(NULL, delim, list);
	update_args(list, e_list);
	was_unclosed_quotes(list);
}

/* 
 * PROTECTION DE MALLOC! 
 * Adaptation de list pour execve
 * Boucle qui remplit shell->input
*/
void	loop_args(t_shell *shell, t_args **list)
{
	t_args	*current;
	int		len;
	int		len_list;
	int		i;

	len = 0;
	len_list = 0;
	current = *list;
	while (current)
	{
		len_list ++;
		current = current->next;
	}
	i = 0;
	shell->input = ft_calloc(len_list, sizeof(char *));
	current = *list;
	while (current)
	{
		len = ft_strlen(current->str);
		shell->input[i] = ft_calloc(len, sizeof(char));
		shell->input[i] = current->str;
		current = current->next;
		i ++;
	}
	shell->input[i] = NULL;
}
