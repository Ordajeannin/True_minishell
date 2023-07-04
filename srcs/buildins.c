/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buildins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalic <asalic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 12:37:52 by asalic            #+#    #+#             */
/*   Updated: 2023/07/04 14:10:32 by asalic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* 
 * Fonction a l'image de la commande echo.
 * Affiche la chaine de caractere envoyee en parametre tant que
 * le caractere n'est pas " ou '. 
 * (Ajouter les details de args_handle).
*/
void	ft_echo(char *args)
{
	int	i;

	if (!args)
		write (1, "\n", 1);
	else
	{
		i = 1;
		while (args[i] && (args[i] != '"' && args[i] != 39))
			write (1, &args[i++], 1);
		write (1, "\n", 1);
	}
}
