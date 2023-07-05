/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalic <asalic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 12:54:23 by ajeannin          #+#    #+#             */
/*   Updated: 2023/07/05 17:42:34 by asalic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Permet de copier l'env du shell initial,
 * et d'initialiser notre structure.
 * Toutes les varables seront initialisees et modifiables au besoin
 * PATH est split pour pouvoir manipuler chaque path individuellement
 * -> **cmd_paths
 * (Renvoie un message d'erreur si env est null, a voir si on modifie
 * ca plus tard. Par defaut, fin de minishell)
*/
int	handle_env(char **env, t_shell *shell)
{
	if (env == NULL)
		return (msg(ERROR_NOENV));
	shell->home = getenv("HOME");
	shell->pwd = getenv("PWD");
	shell->oldpwd = getenv("OLDPWD");
	shell->user = getenv("USER");
	shell->shell = "minishell";
	shell->path = getenv("PATH");
	shell->lang = getenv("LANG");
	shell->term = getenv("TERM");
	shell->hostname = getenv("HOSTNAME");
	if (shell->path != NULL && shell->path[0] != '\0')
		shell->cmd_paths = ft_split(shell->path + 5, ':');
	return (0);
}

/*
 * Recherche si la commande est accessible depuis un des paths
 * On test chaque path l'un apres l'autre avec access
 * Au premier concluant, on renvoie le path complet
 * Sinon, renvoie NULL
*/
char	*extract_cmd_path(char **paths, char *cmd)
{
	char	*temp;
	char	*command;

	while (*paths)
	{
		temp = ft_strjoin(*paths, "/");
		command = ft_strjoin(temp, cmd);
		free(temp);
		if (access(command, 0) == 0)
			return (command);
		free(command);
		paths++;
	}
	strerror(errno);
	return (NULL);
}
