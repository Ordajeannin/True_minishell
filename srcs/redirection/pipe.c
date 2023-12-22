/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkorsako <pkorsako@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 15:16:42 by ajeannin          #+#    #+#             */
/*   Updated: 2023/12/22 20:38:54 by pkorsako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Si l'input contient n pipe, creera n + 1 sublists
 * et les stockera dans une nouvelle liste chainee, **stock
 * en vu d'etre execute simultanement par execute_command
*/
void	create_sublists(t_args *list, t_shell *shell, t_args **env_list)
{
	t_args_list	*stock;

	stock = stock_sublist(&list);
	if (stock->next != NULL)
		execute_command(&stock, shell);
	else
		args_handle(list, shell, env_list);
}

/*
 * Permet de normer combine_input_with_new_one
 * (oui, c est un comble)
 * Dans les faits, va s'assurer que le nouvel input ne soi pas vide
 * ou constitue d'espaces uniquement
 * (si c est le cas, repropose un prompt et ne cat pas)
*/
char	*call_readline(char *prompt)
{
	char	*input;
	char	*ptr;
	int		flag;

	flag = 1;
	while (1)
	{
		input = readline(prompt);
		if (!input)
			return (NULL);
		ptr = input;
		while (*ptr)
		{
			if (*ptr++ != ' ')
				flag = 0;
		}
		if (!flag)
			break ;
	}
	return (input);
}

/*
 * Permet de normer check_if_there_is_a_lost_pipe
*/
char	*combine_input_with_new_one(char *input, int lenght)
{
	char	*new_input;
	char	*combined;

	new_input = call_readline("> ");
	if (!new_input)
		return (NULL);
	combined = (char *)ft_malloc(lenght + ft_strlen(new_input) + 2, ALLOC);
	if (!combined)
	{
		perror("malloc");
		return (NULL);
	}
	ft_strcpy(combined, input);
	ft_strcat(combined, " ");
	ft_strcat(combined, new_input);
	return (combined);
}
