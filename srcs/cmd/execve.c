/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkorsako <pkorsako@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/17 17:18:10 by asalic            #+#    #+#             */
/*   Updated: 2023/12/22 14:52:24 by ajeannin         ###   ########.fr       */
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

	shell->cmd_paths = NULL;
	if (find_a("PATH", *env_list))
		shell->cmd_paths = ft_split(find_a("PATH", *env_list)->str + 5, ':');
	command = is_path_or_cmd(shell->cmd_paths, arg->str, shell, env_list);
	if (command == NULL)
	{
		if (access(list->str, X_OK | F_OK) == 0)
			return (list->str);
		return (NULL);
	}
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
	if (command == NULL)
		return (NULL);
	if (ft_strncmp(command, "It's env", ft_strlen(command)) == 0)
		return (NULL);
	if (loop_args(shell, list) == 1)
		return (NULL);
	if (ft_strcmp("./minishell", command) != 0)
	{
		signal(SIGQUIT, signal_handler);
		g_error = 2;
	}
	return (command);
}

/*
 * Permet de normer next_execution
*/
static int	help_next_execution(int status, int flag)
{
	if (flag == 1)
	{
		errno = WEXITSTATUS(status);
		if (g_error != 0)
			set_error_nb(g_error, YES);
		else
			set_error_nb(handle_error(errno), YES);
		return (1);
	}
	if (flag == 2)
	{
		ft_printf("Segmentation Fault (core dumped)\n");
		set_error_nb(139, YES);
		return (1);
	}
	return (0);
}

/*
 * Processus parents en attente
 * Attends la fin du processus enfant et renvoie une erreur si
 * le processus enfant en a detecte une.
 * Exception: pour ./minishell, -1 a la VE de SHLVL
*/
static int	next_execution(pid_t pid_child, t_args **env_list, t_shell *shell)
{
	int		status;

	(void)env_list;
	(void)shell;
	waitpid(pid_child, &status, 0);
	signal(SIGQUIT, SIG_IGN);
	if (g_error == 2)
		g_error = 0;
	if (WEXITSTATUS(status) != 0)
		return (help_next_execution(status, 1));
	else if (WTERMSIG(status) == SIGSEGV)
		return (help_next_execution(status, 2));
	else
	{
		if (g_error != 0)
		{
			set_error_nb(g_error, YES);
			return (1);
		}
	}
	set_error_nb(0, YES);
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
		return (1);
	pid_child = fork();
	if (pid_child == -1)
	{
		perror("fork");
		ft_malloc(0, FREE);
		exit(EXIT_FAILURE);
	}
	else if (pid_child == 0)
	{
		if (help_all_cmd(shell, command, env_list) == 1)
			return (1);
		exit(handle_error(errno));
	}
	if (next_execution(pid_child, env_list, shell) == 1)
		return (1);
	return (0);
}
