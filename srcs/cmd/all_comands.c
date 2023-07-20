/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   all_comands.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalic <asalic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/17 17:18:10 by asalic            #+#    #+#             */
/*   Updated: 2023/07/18 17:05:06 by asalic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* 
 * Debut de all_cmd.
 * Cherche si la commande existe ou non a partir de PATH.
*/
char	*error_cmd(t_args *arg, t_shell *shell, t_args *list)
{
	char	*command;

	command = extract_cmd_path(shell->cmd_paths, arg->str, shell);
	if (command == NULL)
	{
		if (access(list->str, 0) == 0)
			return (list->str);
		ft_printf("%d\n", shell->error);
		return (NULL);
	}
	return (command);
}

/* 
 * Execution des commandes dependantes de PATH.
 * Creation d'un sous-processus pour execve.
 * Check si la commande est valide.
 * Check si ses arguments sont valides.
 * Si non, return (1) et arret de l'execution de l'invite de commandes.
*/
int	all_cmd(t_args *arg, t_shell *shell, t_args **list)
{
	pid_t	pid_child;
	char	*command;
	int		status;

	command = error_cmd(arg, shell, *list);
	if (command == NULL)
		return (1);
	loop_args(shell, list);
	pid_child = fork();
	if (pid_child == 0)
	{
		execve(command, shell->input, NULL);
		perror("Problem:");
		exit(EXIT_FAILURE);
	}
	else
	{
		waitpid(pid_child, &status, 0);
		if (WEXITSTATUS(status) != 0)
			return (1);
	}
	return (0);
}
