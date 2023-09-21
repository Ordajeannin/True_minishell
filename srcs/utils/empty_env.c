/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   empty_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalic <asalic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 09:44:52 by asalic            #+#    #+#             */
/*   Updated: 2023/09/21 14:35:40 by asalic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*read_bytes_user(int *pipefd, pid_t child, int status,
	t_args **env_list)
{
	int		len_bytes;
	char	buf[1024];
	char	*username;

	close(pipefd[1]);
	len_bytes = read(pipefd[0], buf, sizeof(buf));
	if (len_bytes > 0)
	{
		username = malloc(len_bytes * sizeof(char));
		if (!username)
			return (NULL);
		username = ft_strdupto_n(buf, '\n');
	}
	waitpid(child, &status, 0);
	if (WEXITSTATUS(status) != 0)
	{
		errno = WEXITSTATUS(status);
		change_error(env_list, handle_error(errno));
		return (NULL);
	}
	return (username);
}

char	**create_path_cmd(void)
{
	char	**cmd;

	cmd = malloc(2 * sizeof(char *));
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
char	*get_username(t_shell *shell, t_args **env_list)
{
	pid_t	child;
	int		pipefd[2];
	int		status;

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
	return (read_bytes_user(pipefd, child, status, env_list));
}
