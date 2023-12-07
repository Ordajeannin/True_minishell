/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkorsako <pkorsako@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 12:54:23 by ajeannin          #+#    #+#             */
/*   Updated: 2023/12/07 17:07:05 by pkorsako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* 
 * Check si getenv est NULL
 * Si oui, renvoit une chaine vide
 * Renvoit un char * de la valeur de la ve enregistree
*/
static char *get_env_var(const char *name)
{
	char	*env_var;

	env_var = getenv(name);
	if (! env_var)
		env_var = "";
	return (ft_strdup(env_var));
}

/* 
 * Creer les maillons shell->str en fonction de getenv
 * Check cas d'erreurs
*/
int	handle_env(char **env, t_shell *shell)
{
	if (env == NULL)
		return (msg(ERROR_NOENV));
	shell->home = get_env_var("HOME");
	shell->pwd = get_env_var("PWD");
	shell->is_pwd = get_env_var("PWD");
	shell->is_oldpwd = get_env_var("OLDPWD");
	shell->oldpwd = get_env_var("OLDPWD");
	shell->path = get_env_var("PATH");
	shell->shlvl = get_env_var("SHLVL");
	shell->error = 0;
	if (shell->path != NULL && shell->path[0] != '\0')
	{
		shell->cmd_paths = ft_split(shell->path + 5, ':');
		if (! shell->cmd_paths)
			return (-1);
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

	command = NULL;
	while (paths && *paths)//j'ai enlever "shell->path" 
	{
		temp = ft_strjoin(*paths, "/");
		if (! temp)
			return (NULL);
		command = ft_strjoin(temp, cmd);
		if (! command)
			return (NULL);
		if (access(command, X_OK | F_OK) == 0)
			return (command);
		paths++;
	}
	ft_printf("%s : Command not found\n", cmd);
	change_error(env_list, shell, 127);
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
	else if (access(cmd, X_OK | F_OK) == 0)
	{
		// if (cmd[ft_strlen(cmd) - 1] == 'v' && 
		// 	cmd[ft_strlen(cmd) - 2] == 'n' && 
		// 	cmd[ft_strlen(cmd) - 3] == 'e')
		// 	return ("It's env");
		return (cmd);
	}
	return (cmd);
}
