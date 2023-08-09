/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalic <asalic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 12:54:23 by ajeannin          #+#    #+#             */
/*   Updated: 2023/08/09 15:39:35 by asalic           ###   ########.fr       */
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
	shell->is_pwd = getenv("PWD");
	shell->is_oldpwd = getenv("OLDPWD");
	shell->oldpwd = getenv("OLDPWD");
	shell->user = getenv("USER");
	shell->shell = getenv("SHELL");
	shell->path = getenv("PATH");
	shell->lang = getenv("LANG");
	shell->term = getenv("TERM");
	shell->hostname = getenv("HOSTNAME");
	shell->shlvl = getenv("SHLVL");
	shell->error = 0;
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
static char	*extract_cmd_path(char **paths, char *cmd, t_shell *shell)
{
	char	*temp;
	char	*command;

	if (shell->path == NULL)
		return (NULL);
	while (*paths)
	{
		temp = ft_strjoin(*paths, "/");
		command = ft_strjoin(temp, cmd);
		free(temp);
		if (access(command, X_OK) == 0)
			return (command);
		free(command);
		paths++;
	}
	strerror(errno);
	shell->error = errno;
	return (NULL);
}

/*Deux possibilites:
	-> Soit cmd est un path (avec des /) et on teste directement le path
	-> Soit cmd n'est pas un path mais juste une commande, on boucke sur
	tous les paths possibles et on regarde si l'executable existe ou non. 
*/

char	*is_path_or_cmd(char **paths, char *cmd, t_shell *shell)
{
	char	*command;

	if (strchr(cmd, '/') == NULL)
	{
		command = extract_cmd_path(paths, cmd, shell);
		return (command);
	}
	else if (access(cmd, X_OK) == 0)
	{
		if (cmd[ft_strlen(cmd) - 1] == 'v' && \
			cmd[ft_strlen(cmd) - 2] == 'n' && \
			cmd[ft_strlen(cmd) - 3] == 'e')
			return ("It's env");
		return (cmd);
	}
	return (NULL);
}
