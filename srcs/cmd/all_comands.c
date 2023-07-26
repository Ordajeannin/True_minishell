/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   all_comands.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalic <asalic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/17 17:18:10 by asalic            #+#    #+#             */
/*   Updated: 2023/07/26 12:56:48 by asalic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* 
 * Debut de all_cmd.
 * Cherche si la commande existe ou non a partir de PATH.
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
		ft_printf("%d\n", shell->error);
		return (NULL);
	}
	else if (ft_strncmp(command, "It's env", ft_strlen(command)) == 0)
		ft_env(list, env_list, shell);
	return (command);
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
	int		status;

	command = error_cmd(arg, shell, *list, env_list);
	if (command == NULL)
		return (1);
	if (ft_strncmp(command, "It's env", ft_strlen(command)) == 0)
		return (0);
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
