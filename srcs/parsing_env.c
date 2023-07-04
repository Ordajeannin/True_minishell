/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajeannin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 12:54:23 by ajeannin          #+#    #+#             */
/*   Updated: 2023/07/04 13:26:29 by ajeannin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Permet de copier l'env du shell initial,
 * et d'initialiser notre structure.
 * Toutes les varables seront initialisees et modifiables au besoin
 * (Renvoie un message d'erreur si env est null, a voir si on modifie
 * ca plus tard. Par defaut, fin de minishell)
*/
int	handle_env(char **env, t_env *envcpy)
{
	if (env == NULL)
		return (msg(ERROR_NOENV));
	envcpy->home = getenv("HOME");
	envcpy->pwd = getenv("PWD");
	envcpy->user = getenv("USER");
	envcpy->shell = "minishell";
	envcpy->path = getenv("PATH");
	envcpy->lang = getenv("LANG");
	envcpy->term = getenv("TERM");
	envcpy->hostname = getenv("HOSTNAME");
	return (0);
}
