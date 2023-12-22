/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkorsako <pkorsako@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 13:26:42 by ajeannin          #+#    #+#             */
/*   Updated: 2023/12/22 20:50:58 by ajeannin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	set_error_nb(int error, int update)
{
	static int	error_nb;

	if (update == NO)
		return (error_nb);
	error_nb = error;
	return (-1);
}

/*
 * Gestionnaire d'erreurs
*/
int	handle_error(int code_err)
{
	int	final_error;

	final_error = 0;
	if (code_err == 13 || code_err == 12)
		final_error = 126;
	else if (code_err == 11)
		final_error = 127;
	else
		final_error = code_err;
	return (final_error);
}

/*
 * Permet d'afficher un message d'erreur specifique,
 * fourni lors de l'appel de la fonction
 * renvoie -1
 * (possibilite d'utiliser exit() au lieu de return)
*/
int	msg(char *msg)
{
	perror(msg);
	return (ERROR);
}
