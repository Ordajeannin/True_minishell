/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajeannin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/22 14:48:40 by ajeannin          #+#    #+#             */
/*   Updated: 2023/12/22 14:49:14 by ajeannin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	help_all_cmd(t_shell *shell, char *command, t_args **env_list)
{
	char	**env_tab;

	env_tab = NULL;
	env_tab = dup_double_string(env_list);
	if (!env_tab)
		return (1);
	execve(command, shell->input, env_tab);
	printf("%s : %s\n", shell->input[0], strerror(errno));
	shell->error = handle_error(errno);
	return (0);
}
