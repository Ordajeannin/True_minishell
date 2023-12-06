/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   empty_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkorsako <pkorsako@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 09:44:52 by asalic            #+#    #+#             */
/*   Updated: 2023/12/06 17:15:59 by ajeannin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*read_bytes_user(int *pipefd, pid_t child, int status,
	t_args **env_list, t_shell *shell)
{
	int		len_bytes;
	char	buf[1024];
	char	*username;

	close(pipefd[1]);
	len_bytes = read(pipefd[0], buf, sizeof(buf));
	if (len_bytes > 0)
		username = ft_strdupto_n(buf, '\n');
	waitpid(child, &status, 0);
	if (WEXITSTATUS(status) != 0)
	{
		errno = WEXITSTATUS(status);
		change_error(env_list, shell, handle_error(errno));
		return (NULL);
	}
	return (username);
}

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
 * Si env -i ./minishell
 * Fait appel a whoami pour avoir l'user
*/
char	*get_username(t_args **env_list, t_shell *shell)
{
	pid_t	child;
	int		pipefd[2];
	int		status;

	status = 0x0;
	if (pipe(pipefd) == -1)
	{
		perror("pipe error\n");
		exit(EXIT_FAILURE);
	}
	child = fork();
	if (child == -1)
	{
		perror("fork error\n");
		exit(EXIT_FAILURE);
	}
	else if (child == 0)
	{
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		execve("/bin/whoami", create_path_cmd(), NULL);
		ft_printf("%s : %s\n", "/bin/whoami", strerror(errno));
		exit(handle_error(errno));
	}
	return (read_bytes_user(pipefd, child, status, env_list, shell));
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
