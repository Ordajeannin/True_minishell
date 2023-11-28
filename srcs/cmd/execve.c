/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalic <asalic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/17 17:18:10 by asalic            #+#    #+#             */
/*   Updated: 2023/11/28 23:43:05 by ajeannin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Permet de normer change_error
*/
static int	help_change_error1(char **nb_char, t_shell **shell, int value)
{
	free(*nb_char);
	(*shell)->error = value;
	return (2);
}

/*
 * Permet de normer change_error
*/
static int	help_change_error2(char **nb_char, char **result, t_shell **shell,
		int value)
{
	free(*nb_char);
	free(*result);
	(*shell)->error = value;
	return (2);
}

/*
 * Permet de normer change_error
*/
static void	help_change_error3(t_args **current, char **result, t_shell **shell, 
		int value)
{
	if ((*current)->str)
		free((*current)->str);
	(*current)->str = ft_strdup(*result);
	(*shell)->error = value;
}

/*
 * Permet de normer change_error
*/
static int	help_change_error4(char **current_name, char **nb_char, 
		char **result)
{
	free(*current_name);
	free(*nb_char);
	free(*result);
	return (1);
}

/*
 * Permet de normer change_error
*/
static int	help_change_error5(t_shell **shell, int value, char **nb_char,
		char **result)
{
	(*shell)->error = value;
	free(*nb_char);
	free(*result);
	return (0);
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
	// char	*tmp;

	// ft_printf("env_list 1 : %s | value : %d\n", *env_list, value);
	nb_char = ft_itoa(value);
	if (!nb_char)
		return (2);
	result = ft_strjoin("?=", nb_char);
	// tmp = NULL;
	if (!result)
		return (help_change_error1(&nb_char, &shell, value));
//	{
//		free(nb_char);
//		shell->error = value;
//		return (2);
//	}
	current = *env_list;
	while (current)
	{
		// ft_printf("je suis la bicth 3\n");
		current_name = ft_strdupto_n(current->str, '=');
		if (!current_name)
			return (help_change_error2(&nb_char, &result, &shell, value));
//		{
//			free(nb_char);
//			free(result);
//			shell->error = value;
//			return (2);
//		}
		if (ft_strcmp(current_name, "?") == 0 && ft_strlen(current_name) == 1)
		{
			help_change_error3(&current, &result, &shell, value);
			return (help_change_error4(&current_name, &nb_char, &result));
//			if (current->str)
//				free(current->str);
//			current->str = ft_strdup(result);
//			shell->error = value;
//			free(current_name);
//			free(nb_char);
//			free(result);
//			return (1);
		}
		free(current_name);
		current = current->next;
	}
	return (help_change_error5(&shell, value, &nb_char, &result));
//	shell->error = value;
//	free(nb_char);
//	free(result);
//	return (0);
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

	command = is_path_or_cmd(shell->cmd_paths, arg->str, shell, env_list);
	if (command == NULL)
	{
		if (access(list->str, X_OK | F_OK) == 0)
			return (list->str);
		return (NULL);
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
static int	help_next_execution(t_args **env_list, t_shell *shell, int status,
		int flag)
{
	if (flag == 1)
	{
		errno = WEXITSTATUS(status);
		if (g_error != 0)
			change_error(env_list, shell, g_error);
		else
			change_error(env_list, shell, handle_error(errno));
		return (1);
	}
	if (flag == 2)
	{
		ft_printf("Segmentation Fault (core dumped)\n");
		change_error(env_list, shell, 139);
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
//	if (WEXITSTATUS(status) != 0)
//	{
//		errno = WEXITSTATUS(status);
//		if (g_error != 0)
//			change_error(env_list, shell, g_error);
//		else
//			change_error(env_list, shell, handle_error(errno));
//		return (1);
//	}
	if (WEXITSTATUS(status) != 0)
		return (help_next_execution(env_list, shell, status, 1));
	else if (WTERMSIG(status) == SIGSEGV)
		return (help_next_execution(env_list, shell, status, 2));
//	{
//		ft_printf("Segmentation Fault (core dumped)\n");
//		change_error(env_list, shell, 139);
//		return (1);
//	}
	else
	{
		if (g_error != 0)
		{
			if (!change_error(env_list, shell, g_error))
				return (1);
		}
		else if (!change_error(env_list, shell, 0))
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
	char	**env_tab;

	env_tab = NULL;
	command = bfore_execution(arg, shell, list, env_list);
	if (command == NULL)
		return (1);
	pid_child = fork();
	if (pid_child == -1)
	{
		perror("fork");
		free(command);
		free_everything(shell, *list, *env_list);
		exit(EXIT_FAILURE);
	}
	else if (pid_child == 0)
	{
		// start awena
		env_tab = dup_double_string(env_list);
		if (!env_tab)
		{
			free(command);
			free_everything(shell, *list, *env_list);
			return (1);
		}
		execve(command, shell->input, env_tab);
		ft_printf("%s : %s\n", shell->input[0], strerror(errno));
		free(command);
		shell->error = handle_error(errno);
		free_everything(shell, *list, *env_list);
		exit(handle_error(errno));
	}
	if (env_tab)
	{
		while (*env_tab != NULL)
		{
			free(*env_tab);
			(*env_tab) ++;
		}
	// fin awena
	// start moi (norme)
		//if (help_all_cmd01(env_tab, env_list, &command, &shell) == 1)
			//return (1);
////		env_tab = dup_double_string(env_list);
////		if (!env_tab)
////	{
////			free(command);
////			return (1);
////		}
////		execve(command, shell->input, env_tab);
		//help_all_cmd02(&shell, list, env_list);
////		ft_printf("%s : %s\n", shell->input[0], strerror(errno));
////		shell->error = handle_error(errno);
////		free_everything(shell, *list, *env_list);
////		exit(handle_error(errno));
	}
	//if_env_tab(env_tab);
//	if (env_tab)
//	{
//		while (*env_tab != NULL)
//		{
//			free(*env_tab);
//			(*env_tab) ++;
//		}
//	}
	// fin moi (norme)
	if (next_execution(pid_child, env_list, shell) == 1)
		return (1);
	return (0);
}

