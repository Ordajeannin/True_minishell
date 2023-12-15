/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkorsako <pkorsako@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/17 17:18:10 by asalic            #+#    #+#             */
/*   Updated: 2023/12/12 13:08:58 by pkorsako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Permet de normer change_error
*/
static int	help_change_error(t_shell **shell, int value, int flag)
{
	(*shell)->error = value;
	return (flag);
}

/*
 * Permet de normer change_error
*/
static int	help_change_error2(t_args **current, char **result, t_shell **shell, 
		int value)
{
	(*current)->str = ft_strdup(*result);
	(*shell)->error = value;
	return (1);
}

/*
 * Change env pour $?.
 * Mets a jour les cas d'erreurs de $?
*/
int	change_error(t_args **env_list, t_shell *shell, int value)
{
	t_args	*current;
	char	*current_name;
	char	*result;
	char 	*nb_char;

	nb_char = ft_itoa(value);
	if (!nb_char)
		return (2);
	result = ft_strjoin("?=", nb_char);
	if (!result)
		return (help_change_error(&shell, value, 2));
	current = *env_list;
	while (current)
	{
		current_name = ft_strdupto_n(current->str, '=');
		if (!current_name)
			return (help_change_error(&shell, value, 2));
		if (ft_strcmp(current_name, "?") == 0 && ft_strlen(current_name) == 1)
			return (help_change_error2(&current, &result, &shell, value));
		current = current->next;
	}
	printf("change erro return 0\n");
	return (help_change_error(&shell, value, 0));
}

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
	// if (ft_strncmp(command, "It's env", ft_strlen(command)) == 0)
	// 	ft_env(list, env_list, shell);
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
static int	help_next_execution(int status,
		int flag)
{
	if (flag == 1)
	{
		errno = WEXITSTATUS(status);
		if (g_error != 0)
			set_error_nb(g_error, YES);
			// change_error(env_list, shell, g_error);
		else
			set_error_nb(handle_error(errno), YES);
			// change_error(env_list, shell, handle_error(errno));
		return (1);
	}
	if (flag == 2)
	{
		ft_printf("Segmentation Fault (core dumped)\n");
		set_error_nb(139, YES);
		// change_error(env_list, shell, 139);
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
			////////////awena///////////
			if (!change_error(env_list, shell, g_error))
				return (1);
		}
		// else if (!change_error(env_list, shell, 0))
			// return (1);
	}
	set_error_nb(0, YES);
	return (0);
}

int help_all_cmd(t_shell *shell, char *command, t_args **env_list)
{
	char **env_tab;

	env_tab = NULL;
	env_tab = dup_double_string(env_list);
	if (!env_tab)
		return (1);
	execve(command, shell->input, env_tab);
	printf("%s : %s\n", shell->input[0], strerror(errno));
	shell->error = handle_error(errno);
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
	if (pid_child == -1) //Semble quitter le programme -> free
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
