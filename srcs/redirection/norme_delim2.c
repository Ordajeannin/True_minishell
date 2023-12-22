/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   norme_delim2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajeannin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/22 17:18:40 by ajeannin          #+#    #+#             */
/*   Updated: 2023/12/22 17:25:54 by ajeannin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_result(int *pipes)
{
	char	*result;
	char	*tmp;

	tmp = ft_strdup("");
	result = ft_strdup("");
	while (tmp)
	{
		tmp = get_next_line(pipes[0]);
		if (tmp)
			result = ft_strjoin(result, tmp);
	}
	return (result);
}

void	child_of_heredoc(int *pipes, t_args **stock)
{
	close(pipes[0]);
	close_pipefd(pipes, 0);
	signal(SIGINT, &signal_heredoc);
	if (handle_multi_heredoc(stock, pipes) == 1)
	{
		printf("syntax error near unexpected token\n");
		exit(2);
	}
	exit(0);
}

int	parent_of_heredoc(int *pipes, t_args **stock, pid_t pid)
{
	int	status;

	status = 0;
	close(pipes[1]);
	waitpid(pid, &status, 0);
	if (WEXITSTATUS(status) == 2 || WEXITSTATUS(status) == 130)
	{
		close(pipes[0]);
		set_error_nb(WEXITSTATUS(status), YES);
		return (2);
	}
	if (*stock != NULL)
		tempfile(get_result(pipes));
	close(pipes[0]);
	return (0);
}

//+ **stock pour la partie 2, mais certainemen a passer dans les args
//+ protection fork+pipe
int	handle_heredoc_part2(t_args **stock)
{
	pid_t	pid;
	int		pipes[2];

	if (pipe(pipes) == -1)
	{
		perror("pipe");
		ft_malloc(0, FREE);
		exit(EXIT_FAILURE);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		ft_malloc(0, FREE);
		exit(EXIT_FAILURE);
	}
	if (pid == 0)
		child_of_heredoc(pipes, stock);
	else
		if (parent_of_heredoc(pipes, stock, pid) == 2)
			return (2);
	return (0);
}
