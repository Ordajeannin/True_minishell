/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_of_args.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkorsako <pkorsako@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 17:55:04 by ajeannin          #+#    #+#             */
/*   Updated: 2023/12/04 18:45:38 by pkorsako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Fonction utilitaire, libere chaque maillon de la chaine d'arguments
 * conserve *liste et le reset a NULL
*/
void	clear_args_list(t_args **list)
{
// 	t_args	*current;
// 	t_args	*next;

// 	current = *list;
// 	while (current != NULL)
// 	{
// 		next = current->next;
// 		// if (current->str)
// 		// 	free(current->str);
// 		// free(current);
// 		current = next;
// 	}
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

/* 
 * Permets de copier un liste d'args
*/
t_args *copy_list(t_args *source)
{
    t_args* new_head;
    t_args* current;
    t_args* tail;
    
	new_head = NULL;
	tail = NULL;
	current = source;
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
			// tail->next = NULL;
        }
        current = current->next;
    }
	// free(tail->next);
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
		// if(current->next)
		// 	free(current->next);
		current->next = copy_list(new_arg);
	}
	// free(new_arg);
}

/*
 * Permet de norme from_input_to_list_of_args
*/
static int	help_fitloa(t_args **list, t_args **e_list)
{
	if (update_args(list) == 1)
		return (1);
	if (handle_heredoc(list) == 1)
		return (1);
	if (update_args2(list, e_list) == 1)
		return (1);
	return (0);
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
	if (help_fitloa(list, e_list) == 1)
		return (1);
	was_unclosed_quotes(list);
	return (0);
}

/*
 * Permet de norme loop_args
*/
static int	help_loop_args(t_shell **shell, int *i, int flag)
{
	if (flag == 1)
	{
		// while ((*shell)->input[*i])
		// 	free((*shell)->input[(*i)++]);
		// free((*shell)->input);
		*i = 0;
	}
	else if (flag == 2)
	{
		// while (*i >= 0)
		// 	free((*shell)->input[(*i)--]);
		// free((*shell)->input);
		return (1);
	}
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
		help_loop_args(&shell, &i, 1);
	shell->input = ft_calloc((len_list + 1), sizeof(char *));
	if (!shell->input)
		return (1);
	while (current)
	{
		shell->input[i] = ft_strdup(current->str);
		if (!shell->input[i])
			help_loop_args(&shell, &i, 2);
		current = current->next;
		i ++;
	}
	shell->input[i] = NULL;
	return (0);
}
