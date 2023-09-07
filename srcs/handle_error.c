/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalic <asalic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 13:26:42 by ajeannin          #+#    #+#             */
/*   Updated: 2023/09/05 17:40:18 by asalic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_error;

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
	if (code_err == 13)
		final_error = 126;
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

/* Appel a $?.
 * Renvoie la derniere erreur enregistree.
 * S'il y a erreur, elle renvoit 0(= pas d'erreurs)
 * Sinon, elle renvoit le numero de l'erreur.
*/
int	var_error(t_shell *shell)
{
	ft_printf("%d\n", shell->error);
	shell->error = 0;
	return (0);
}
