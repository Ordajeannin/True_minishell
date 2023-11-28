/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help_execve1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajeannin <ajeannin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 13:32:23 by ajeannin          #+#    #+#             */
/*   Updated: 2023/10/09 13:50:17 by ajeannin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Permet de normer all_command
 * premiere partie de l'execution du processus fils
*/
int	help_all_cmd01(char **env_tab, t_args **env_list, char **command,
		t_shell **shell)
{
	env_tab = dup_double_string(env_list);
	if (!env_tab)
	{
		free(*command);
		return (1);
	}
	execve(*command, (*shell)->input, env_tab);
	return (0);
}

/*
 * Permet de normer all_command
 * deuxime partie de l'execution du processus fils
*/
void	help_all_cmd02(t_shell **shell, t_args **list, t_args **env_list)
{
	ft_printf("%s : %s\n", (*shell)->input[0], strerror(errno));
	(*shell)->error = handle_error(errno);
	free_everything(*shell, *list, *env_list);
	exit(handle_error(errno));
}

/*
 * Permet de normer all_cmd
 * libere la memoire de env_tab
*/
void	if_env_tab(char **env_tab)
{
	if (env_tab)
	{
		while (*env_tab != NULL)
		{
			free(*env_tab);
			(*env_tab)++;
		}
	}
}
