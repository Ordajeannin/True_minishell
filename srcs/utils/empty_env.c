/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   empty_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkorsako <pkorsako@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 09:44:52 by asalic            #+#    #+#             */
/*   Updated: 2023/12/19 15:48:06 by pkorsako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**create_path_cmd(void)
{
	char	**cmd;

	cmd = ft_malloc(2 * sizeof(char *), ALLOC);
	if (!cmd)
		return (NULL);
	cmd[0] = ft_strdup("/bin/whoami");
	cmd[1] = NULL;
	return (cmd);
}
