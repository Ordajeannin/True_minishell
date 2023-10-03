/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_of_args.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalic <asalic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 17:55:04 by ajeannin          #+#    #+#             */
/*   Updated: 2023/10/03 12:07:21 by asalic           ###   ########.fr       */
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
		// free(current->str);
		free(current);
		current = next;
	}
	*list = NULL;
}

// static void	free_maillon(t_args *node)
// {
// 	if (node)
// 	{
// 		if (node->str)
// 			free(node->str);
// 		free(node);
// 	}
// }

/*
 * Fonction utilitaire, cree un maillon 
 * a partir du contenu de l'argument et du token attribue
*/
static t_args	*create_arg(char *str, int token)
{
	t_args	*new_arg;

	new_arg = ft_calloc(1, sizeof(t_args));
	if (!new_arg)
		return (NULL);
	new_arg->str = str;
	new_arg->token = token;
	new_arg->next = NULL;
	return (new_arg);
}

// static t_args *copy_list(t_args* source)
// {
//     t_args* new_head = NULL;
//     t_args* current = source;
//     t_args* tail = NULL;
    
//     while (current != NULL)
// 	{
//         if (new_head == NULL)
// 		{
//             new_head = ft_calloc(1, sizeof(t_args));
//             new_head->str = current->str;
// 			new_head->token = current->token;
//             new_head->next = NULL;
//             tail = new_head;
//         }
// 		else
// 		{
//             tail->next = ft_calloc(1, sizeof(t_args));
//             tail = tail->next;
//             tail->str = current->str;
// 			tail->token = current->token;
//             tail->next = NULL;
//         }
//         current = current->next;
//     }
//     return (new_head);
// }
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
int	from_input_to_list_of_args(char *input, t_args **list, t_args **e_list)
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
	if (update_args(list, e_list) == 1)
		return (1);
	was_unclosed_quotes(list);
	return (0);
}

/* 
 * PROTECTION DE MALLOC! 
 * Adaptation de list pour execve
 * Boucle qui remplit shell->input
*/
int	loop_args(t_shell *shell, t_args **list)
{
	t_args	*current;
	int		len_list;
	int		i;

	current = *list;
	len_list = len_targs(current);
	i = 0;
	if (shell->input)
	{
		while (shell->input[i])
			free(shell->input[i++]);
		i = 0;
	}
	shell->input = ft_calloc((len_list + 1), sizeof(char *));
	if (!shell->input)
		return (1);
	while (current)
	{
		shell->input[i] = ft_strdup(current->str);
		if (!shell->input[i])
		{
			while (i >= 0)
				free(shell->input[i--]);
			return (1);
		}
		current = current->next;
		i ++;
	}
	shell->input[i] = NULL;
	return (0);
}
