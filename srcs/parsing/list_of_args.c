/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_of_args.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkorsako <pkorsako@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 17:55:04 by ajeannin          #+#    #+#             */
/*   Updated: 2023/12/21 20:16:23 by pkorsako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Fonction utilitaire, libere chaque maillon de la chaine d'arguments
 * conserve *liste et le reset a NULL
*/
void	clear_args_list(t_args **list)
{
	*list = NULL;
}

/*
 * Fonction utilitaire, cree un maillon
 * a partir du contenu de l'argument et du token attribue
*/
t_args	*create_arg(char *str, int token)
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
t_args	*copy_list(t_args *source)
{
	t_args	*new_head;
	t_args	*current;
	t_args	*tail;

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
		}
		current = current->next;
	}
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
}

/*
 * La fonction commence a etre complexe, on va faire un point :
 * 1) Creation de la liste avec le parsing principal, en fonction des delims
 *    -> ft_strtok
 * 2) Mise a jour des tokens (la fonction semble outdated, vraiment necessaire?)
 *    -> update_args
 * 3) Verifie les arguments suivants les redirections/heredocs
 *    on ne veut (en gros) que des alphanum
 *    -> is_correct_format
 * 4) Gestion des heredocs, assez early pour une histoire de VE (cat << $USER)
 *    -> handle_heredoc
 * 5) Mise a jour de la liste, pour gerer les s/d quotes + substitution VE
 *    l'idee etant de passer de : str"hello'world'"$USER'$USER'
 *    a : strhello'world'ajeannin$USER
 *    (ouais, a peine chiant)
 *    -> handle_quotes
 * 6) Verifie la presence ou non de quotes non-fermee (auquel cas, msg d'erreur)
*/
int	from_input_to_list_of_args(char *input, t_shell *shell, t_args **e_list)
{
	char	*delim[9];
	char	*token;
	int		fitloa_ret;

	delim[0] = " ";
	delim[1] = "\t";
	delim[2] = "<<";
	delim[3] = "<";
	delim[4] = ">>";
	delim[5] = ">";
	delim[6] = "||";
	delim[7] = "|";
	delim[8] = NULL;
	token = ft_strtok(input, delim, shell);
	while (token != NULL)
		token = ft_strtok(NULL, delim, shell);
	if (update_args(&(shell->list)) == 1)
		return (1);
	if (is_correct_format(&(shell->list)) == -1)
		return (2);
	fitloa_ret = handle_heredoc(&(shell->list));
	if (fitloa_ret)
		return (fitloa_ret);
	if (handle_quotes(&(shell->list), e_list) == 1)
		return (1);
	was_unclosed_quotes(&(shell->list));
	return (0);
}

/*
 * PROTECTION DE MALLOC!
 * Adaptation de list pour execve
 * Boucle qui remplit shell->input
 * protection ft_strdup?
*/
int	loop_args(t_shell *shell, t_args **list)
{
	t_args	*current;
	int		len_list;
	int		i;

	current = *list;
	len_list = len_targs(current);
	i = 0;
	shell->input = ft_calloc((len_list + 1), sizeof(char *));
	if (!shell->input)
		return (1);
	while (current)
	{
		shell->input[i] = ft_strdup(current->str);
		current = current->next;
		i ++;
	}
	shell->input[i] = NULL;
	return (0);
}
