/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkorsako <pkorsako@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 13:26:42 by ajeannin          #+#    #+#             */
/*   Updated: 2023/12/11 15:20:06 by pkorsako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_error;

int	set_error_nb(int error, int update)//set et stocke la derniere erreur, appeller avec yes pour update et no pour recuperer l'erreur
{
	static int	error_nb;

	if (update == NO)
		return (error_nb);
	error_nb = error;
	return (-1);
}

void	code_error(int code)
{
	g_error = code;
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
