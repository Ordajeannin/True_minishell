/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_of_args.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalic <asalic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 17:55:04 by ajeannin          #+#    #+#             */
/*   Updated: 2023/10/05 15:55:02 by ajeannin         ###   ########.fr       */
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
		if (current->str)
			free(current->str);
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

	new_arg = ft_calloc(1, sizeof(t_args));
	if (!new_arg)
		return (NULL);
	new_arg->str = str;
	new_arg->token = token;
	new_arg->next = NULL;
	return (new_arg);
}

t_args *copy_list(t_args* source)
{
    t_args* new_head = NULL;
    t_args* current = source;
    t_args* tail = NULL;
    
    while (current != NULL)
	{
        if (new_head == NULL)
		{
            new_head = ft_calloc(1, sizeof(t_args));
            new_head->str = current->str;
			new_head->token = current->token;
            new_head->next = NULL;
            tail = new_head;
        }
		else
		{
            tail->next = ft_calloc(1, sizeof(t_args));
            tail = tail->next;
            tail->str = current->str;
			tail->token = current->token;
        }
        current = current->next;
    }
	free(tail->next);
    return (new_head);
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
		*list = copy_list(new_arg);
	else
	{
		current = *list;
		while (current->next != NULL)
			current = current->next;
		current->next = copy_list(new_arg);
	}
	free(new_arg);
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
	if (update_args(list) == 1)
		return (1);
//	printf("------------------- AVANT HEREDOC --------------------\n");
//	print_args_list(list);
	if (handle_heredoc(list) == 1)
		return (1);
//	print_args_list(list);
	if (update_args2(list, e_list) == 1)
		return (1);
//	printf("----------------- POST-SUBSTITUTION -------------------\n");
//	print_args_list(list);
	was_unclosed_quotes(list);
	if (token)
		free(token);
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
		free(shell->input);
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
			free(shell->input);
			return (1);
		}
		current = current->next;
		i ++;
	}
	shell->input[i] = NULL;
	return (0);
}
