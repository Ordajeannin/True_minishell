/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   empty_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkorsako <pkorsako@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 09:44:52 by asalic            #+#    #+#             */
/*   Updated: 2023/12/21 20:49:37 by ajeannin         ###   ########.fr       */
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

/*
 * Set l'env manuellement lorsque env -i ./minishell est lance
 * Set $PWD, $_, $PATH, $?, $SHLVL
 * N'affiche cependant que $PWD, $_ et $SHLVL
*/
int	set_empty_env(t_shell *shell, t_args **env_list)
{
	char	buf[1024];
	char	*test;

	test = getcwd(buf, sizeof(buf));
	shell->pwd = test;
	shell->is_pwd = shell->pwd;
	shell->oldpwd = NULL;
	shell->is_oldpwd = NULL;
	shell->home = "";
	shell->shlvl = "2";
	shell->path = ft_strjoin("/usr/local/sbin:/usr/local/bin:", \
		"/usr/sbin:/usr/bin:/sbin:/bin");
	if (shell->path != NULL && shell->path[0] != '\0')
	{
		shell->cmd_paths = ft_split(shell->path + 5, ':');
		if (! shell->cmd_paths)
			return (-1);
	}
	add_arg(env_list, "SHLVL=1", 0);
	add_arg(env_list, ft_strjoin("PWD=", shell->is_pwd), 0);
	add_arg(env_list, "_=./minishell", 0);
	add_arg(env_list, "?=0", 0);
	add_arg(env_list, ft_strjoin("PATH=", shell->path), 0);
	return (0);
}
