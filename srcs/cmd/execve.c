/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalic <asalic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/17 17:18:10 by asalic            #+#    #+#             */
/*   Updated: 2023/09/05 10:59:41 by asalic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* 
 * Debut de all_cmd.
 * Cherche si la commande existe ou non a partir de PATH.
 * Exception: si la commande est env, redirection vers builtins.
*/
static char	*error_cmd(t_args *arg, t_shell *shell, t_args *list,
	t_args **env_list)
{
	char	*command;

	command = is_path_or_cmd(shell->cmd_paths, arg->str, shell);
	if (command == NULL)
	{
		if (access(list->str, F_OK) == 0)
			return (list->str);
	}
	if (ft_strncmp(command, "It's env", ft_strlen(command)) == 0)
		ft_env(list, env_list, shell);
	return (command);
}

/* 
 * Preparation du bios avant la creation du processus enfant.
 * Check si la commande est valide ou que son path est valide.
 * Mets tous les arguments dans un shell->input (char **).
 * CTRL-\ actif pour les commandes bloquantes comme cat
*/
static char	*bfore_execution(t_args *arg, t_shell *shell, t_args **list,
	t_args **env_list)
{
	char	*command;

	command = error_cmd(arg, shell, *list, env_list);
	if (ft_strncmp(command, "It's env", ft_strlen(command)) == 0)
		return (NULL);
	loop_args(shell, list);
	signal(SIGQUIT, signal_handler);
	return (command);
}

/*
 * Processus parents en attente
 * Attends la fin du processus enfant et renvoie une erreur si
 * le processus enfant en a detecte une.
 * Exception: pour ./minishell, -1 a la VE de SHLVL
*/
static int	next_execution(pid_t pid_child, char *command, t_shell *shell,
	t_args **env_list)
{
	int		status;

	waitpid(pid_child, &status, 0);
	signal(SIGQUIT, SIG_IGN);
	if (ft_strcmp("./minishell", command) == 0 && ft_strlen(command) == 11)
		ft_less_shell(shell, env_list);
	if (WEXITSTATUS(status) != 0)
	{
		errno = WEXITSTATUS(status);
		ft_printf("ERRNO : %d\n", errno);
		shell->error = errno;
		return (1);
	}
	return (0);
}

/* 
 * Execution des commandes dependantes de PATH.
 * Creation d'un sous-processus pour execve.
 * Check si la commande est valide.
 * Check si ses arguments sont valides.
 * Si non, return (1) et arret de l'execution de l'invite de commandes.
*/
int	all_cmd(t_args *arg, t_shell *shell, t_args **list, t_args **env_list)
{
	pid_t	pid_child;
	char	*command;

	command = bfore_execution(arg, shell, list, env_list);
	if (command == NULL)
		return (0);
	pid_child = fork();
	if (pid_child == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else if (pid_child == 0)
	{
		execve(command, shell->input, dup_double_string(env_list));
		ft_printf("%s : %s\n", command, strerror(errno));
		exit(errno);
	}
	else
	{
		if (next_execution(pid_child, command, shell, env_list) == 1)
			return (1);
	}
	return (0);
}
