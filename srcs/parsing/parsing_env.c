/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalic <asalic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 12:54:23 by ajeannin          #+#    #+#             */
/*   Updated: 2023/09/28 14:36:47 by asalic           ###   ########.fr       */
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

/*
static char *get_env_var(const char *name)
{
	char	*env_var;
	char	*var_value;

	env_var = getenv(name);
	if (! env_var)
		env_var = "";
	return (ft_strdup(env_var));
}
*/

int	handle_env(char **env, t_shell *shell)
{
	if (env == NULL)
		return (msg(ERROR_NOENV));
	// AWENA PROTECT US !!!!!!!!!
	shell->home = ft_strdup(getenv("HOME"));
	shell->pwd = ft_strdup(getenv("PWD"));
	shell->is_pwd = ft_strdup(getenv("PWD"));
	shell->is_oldpwd = ft_strdup(getenv("OLDPWD"));
	shell->oldpwd = ft_strdup(getenv("OLDPWD"));
	shell->user = ft_strdup(getenv("USER"));
	shell->shell = ft_strdup(getenv("SHELL"));
	shell->path = ft_strdup(getenv("PATH"));
	shell->lang = ft_strdup(getenv("LANG"));
	shell->term = ft_strdup(getenv("TERM"));
	shell->hostname = ft_strdup(getenv("HOSTNAME"));
	shell->shlvl = ft_strdup(getenv("SHLVL"));
	if (shell->path != NULL && shell->path[0] != '\0')
	{
		shell->cmd_paths = ft_split(shell->path + 5, ':');
		if (! shell->cmd_paths)
			return (1);
	}
	return (0);
}

/*
 * Recherche si la commande est accessible depuis un des paths
 * On test chaque path l'un apres l'autre avec access
 * Au premier concluant, on renvoie le path complet
 * Sinon, renvoie NULL
*/
static char	*extract_cmd_path(char **paths, char *cmd, t_shell *shell,
	t_args **env_list)
{
	char	*temp;
	char	*command;

	while (shell->path && *paths)
	{
		temp = ft_strjoin(*paths, "/");
		if (! temp)
			return (NULL);
		command = ft_strjoin(temp, cmd);
		free(temp);
		if (! command)
			return (NULL);
		if (access(command, X_OK) == 0)
			return (command);
		free(command);
		paths++;
	}
	ft_printf("%s : Command not found\n", cmd);
	change_error(env_list, 127);
	return (NULL);
}

/*Deux possibilites:
	-> Soit cmd est un path (avec des /) et on teste directement le path
	-> Soit cmd n'est pas un path mais juste une commande, on boucle sur
	tous les paths possibles et on regarde si l'executable existe ou non. 
*/

char	*is_path_or_cmd(char **paths, char *cmd, t_shell *shell,
	t_args **env_list)
{
	char	*command;

	if (ft_strchr(cmd, '/') == NULL)
	{
		command = extract_cmd_path(paths, cmd, shell, env_list);
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
	return (cmd);
}
