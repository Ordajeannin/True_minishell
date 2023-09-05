/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalic <asalic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 13:26:42 by ajeannin          #+#    #+#             */
/*   Updated: 2023/09/04 15:02:10 by asalic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_error;

void	code_error(int code)
{
	g_error = code;
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
